type wsOpenContextT('a) = {
  path: Path.t,
  query: Js.Json.t,
  pubsub: PubSub.t('a),
};
type wsMessageContextT('a) = {
  body: Body.t,
  path: Path.t,
  query: Js.Json.t,
  pubsub: PubSub.t('a),
};

type t('ctx, 'a) = {
  onOpen: option(wsOpenContextT('a) => unit),
  onMessage: option(wsMessageContextT('a) => unit),
  config: option(configT),
}
and configT = {
  maxPayloadLength: option(int),
  idleTimeout: option(int),
  compression: option(int),
  maxBackpressure: option(int),
};
type wsUpgradeContextT = {
  rawPath: string,
  rawQuery: string,
};

let (<$>) = Belt.Option.map;
let (>>=) = Belt.Option.flatMap;
let (|?) = Belt.Option.getWithDefault;

let emitter = PubSub.EventEmitter.make();

let makeConfig =
    (
      ~maxPayloadLength=?,
      ~idleTimeout=?,
      ~compression=?,
      ~maxBackpressure=?,
      (),
    ) => {
  let config = {maxPayloadLength, idleTimeout, compression, maxBackpressure};
  config;
};

let make = (~onOpen=?, ~config=?, ~onMessage=?, ()) => {
  let handler = {onOpen, config, onMessage};
  handler;
};

let makeApp = (handler, pubsubAdapter, app) => {
  let config = handler.config;
  let wsBehavior =
    Websocket.makeWebsocketBehavior(
      ~maxPayloadLength=?config >>= (c => c.maxPayloadLength),
      ~idleTimeout=?config >>= (c => c.idleTimeout),
      ~compression=?config >>= (c => c.compression),
      ~maxBackpressure=?config >>= (c => c.maxBackpressure),
      ~upgrade=
        (res, req, ctx_) => {
          // upgrade http to websocket

          let rawPath = req |> Request.getUrl();
          let rawQuery = req |> Request.getQuery();

          res
          |> Response.upgrade(
               {rawQuery, rawPath},
               req |> Request.getHeader("sec-websocket-key"),
               req |> Request.getHeader("sec-websocket-protocol"),
               req |> Request.getHeader("sec-websocket-extensions"),
               ctx_,
             );

          ();
        },
      ~open_=
        (ws, _) => {
          let rawPath = ws |> Websocket.getRawPath;
          let rawQuery = ws |> Websocket.getRawQuery;

          switch (handler.onOpen) {
          | Some(onOpen) =>
            let path = Path.make(~rawPath=rawPath |? "", ~rawNamespace="");
            let query = rawQuery |? "" |> Qs.parse;

            let pubsub = ws |> PubSub.make(pubsubAdapter);

            onOpen({path, query, pubsub});
          | None => ()
          };
        },
      ~message=
        (ws, message, _) => {
          // Js.log(ws);
          let rawPath = ws |> Websocket.getRawPath;
          let rawQuery = ws |> Websocket.getRawQuery;

          switch (handler.onMessage) {
          | Some(onMessage) =>
            let path = Path.make(~rawPath=rawPath |? "", ~rawNamespace="");
            let query = rawQuery |? "" |> Qs.parse;
            let body = Body.make(message, "application/json");

            let pubsub = ws |> PubSub.make(pubsubAdapter);

            onMessage({body, path, query, pubsub});
          | None => ()
          };
        },
      (),
    );
  app |> Uws.ws("/*", wsBehavior);
};