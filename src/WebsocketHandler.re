type wsOpenContextT('a) = {
  namespace: string,
  path: Path.t,
  query: Js.Json.t,
  pubsub: PubSub.t('a),
};
type wsMessageContextT('a) = {
  body: Body.t,
  namespace: string,
  path: Path.t,
  query: Js.Json.t,
  pubsub: PubSub.t('a),
};

type t('ctx, 'a) = (string, handlerT('ctx, 'a))
and handlerT('ctx, 'a) = {
  onOpen: option(wsOpenContextT('a) => unit),
  onMessage: option(wsMessageContextT('a) => unit),
};
type configT = {
  maxPayloadLength: option(int),
  idleTimeout: option(int),
  compression: option(int),
  maxBackpressure: option(int),
};
type wsUpgradeContextT = {
  rawNamespace: string,
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

let make = (~onOpen=?, ~onMessage=?, ()) => {
  let handler = {onOpen, onMessage};
  handler;
};

let makeApp = (handlers, ~config=?, app) => {
  let findHandlerByRawPath = rawPath =>
    try(
      handlers
      |> List.find(((rawNamespace, _): t('ctx, 'a)) => {
           let normalizedPath =
             Path.(rawPath |> removePreceeding |> removeTrailing);

           let normalizedNamespace =
             Path.(rawNamespace |> removePreceeding |> removeTrailing);

           let found =
             switch (normalizedNamespace, normalizedPath) {
             | ("", _) => true
             | (namespace, path) =>
               path == namespace
               || path
               |> Js.String.startsWith(namespace ++ "/")
             };
           found;
         })
    ) {
    | Not_found => ("", make())
    };

  let findHandlerByRawNamespace = rawNamespace =>
    try(
      handlers
      |> List.find(((namespace, _): t('ctx, 'a)) => {
           rawNamespace == namespace
         })
    ) {
    | Not_found => ("", make())
    };

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

          let (rawNamespace, _) = findHandlerByRawPath(rawPath);

          res
          |> Response.upgrade(
               {rawQuery, rawPath, rawNamespace},
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
          let rawNamespace = ws |> Websocket.getRawNamespace;
          let rawQuery = ws |> Websocket.getRawQuery;

          switch (rawNamespace) {
          | Some(rawNamespace) =>
            let (_, handler) = findHandlerByRawNamespace(rawNamespace);
            switch (handler.onOpen) {
            | Some(onOpen) =>
              let path = Path.make(~rawPath=rawPath |? "", ~rawNamespace);
              let query = rawQuery |? "" |> Qs.parse;
              let namespace =
                rawNamespace |> Path.removePreceeding |> Path.removeTrailing;

              let pubsub = ws |> PubSub.make(namespace);

              onOpen({path, query, namespace, pubsub});
            | None => ()
            };
          | None => ()
          };
        },
      ~message=
        (ws, message, _) => {
          // Js.log(ws);
          let rawPath = ws |> Websocket.getRawPath;
          let rawNamespace = ws |> Websocket.getRawNamespace;
          let rawQuery = ws |> Websocket.getRawQuery;

          switch (rawNamespace) {
          | Some(rawNamespace) =>
            let (_, handler) = findHandlerByRawNamespace(rawNamespace);
            switch (handler.onMessage) {
            | Some(onMessage) =>
              let path = Path.make(~rawPath=rawPath |? "", ~rawNamespace);
              let query = rawQuery |? "" |> Qs.parse;
              let body = Body.make(message, "application/json");
              let namespace =
                rawNamespace |> Path.removePreceeding |> Path.removeTrailing;

              let pubsub = ws |> PubSub.make(namespace);

              onMessage({body, path, query, namespace, pubsub});
            | None => ()
            };
          | None => ()
          };
        },
      (),
    );
  app |> Uws.ws("/*", wsBehavior);
};