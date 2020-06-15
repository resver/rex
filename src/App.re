let makeConfig = Uws.makeConfig;
let uws = Uws.uws;

let listen = Uws.listen;
let listenWithHost = Uws.listenWithHost;

let make =
    (
      ~port=3000,
      ~onListen=_ => (),
      ~config=?,
      ~pubSubAdapter=?,
      ~onBeforeHandler=?,
      ~handler: option(HttpHandler.t('a) => unit)=?,
      ~wsHandler: option(WebsocketHandler.t('ctx, 'a))=?,
      ~isSSL=false,
      (),
    ) => {
  let app =
    switch (config, isSSL) {
    | (Some(config), true) => uws |> Uws.app(config)
    | (Some(config), false) => uws |> Uws.sslApp(config)
    | (None, _) => uws |> Uws.appWithoutConfig()
    };

  let createHttpApp = app =>
    switch (handler) {
    | Some(handler) =>
      app |> HttpHandler.makeApp(handler, onBeforeHandler, pubSubAdapter)
    | None => app
    };

  let createWsApp = app =>
    switch (wsHandler) {
    | Some(handler) =>
      app |> WebsocketHandler.makeApp(handler, pubSubAdapter)
    | None => app
    };

  app |> createHttpApp |> createWsApp |> Uws.listen(port, onListen);
};