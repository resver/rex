type t('a) = {
  onUpgrade: option((Request.t, Response.t) => unit),
  onOpen: option(unit => unit),
  onMessage: option(unit => unit),
  config: option(configT),
}
and configT = {
  maxPayloadLength: option(int),
  idleTimeout: option(int),
  compression: option(int),
  maxBackpressure: option(int),
};

let (<$>) = Belt.Option.map;
let (>>=) = Belt.Option.flatMap;

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

let make = (~config=?, ~onUpgrade=?, ~onOpen=?, ~onMessage=?, ()) => {
  let handler = {config, onUpgrade, onOpen, onMessage};
  handler;
};

let makeApp = (handler, app) => {
  let config = handler.config;

  let wsBehavior =
    Websocket.makeWebsocketBehavior(
      ~maxPayloadLength=?config >>= (c => c.maxPayloadLength),
      ~idleTimeout=?config >>= (c => c.idleTimeout),
      ~compression=?config >>= (c => c.compression),
      ~maxBackpressure=?config >>= (c => c.maxBackpressure),
      ~upgrade=
        (res, req, ctx) => {
          switch (handler.onUpgrade) {
          | Some(onUpgrade) => onUpgrade(req, res)
          | None => ()
          };

          res
          |> Response.upgrade(
               {"app": "Rex"},
               req |> Request.getHeader("sec-websocket-key"),
               req |> Request.getHeader("sec-websocket-protocol"),
               req |> Request.getHeader("sec-websocket-extensions"),
               ctx,
             );
          ();
        },
      ~open_=
        (ws, req) => {
          switch (handler.onOpen) {
          | Some(onOpen) => onOpen()
          | None => ()
          }
        },
      (),
    );
  app |> Uws.ws("/*", wsBehavior);
};