let makeConfig = Uws.makeConfig;
let uws = Uws.uws;

let listen = Uws.listen;
let listenWithHost = Uws.listenWithHost;

let make =
    (
      ~port=3000,
      ~onListen=_ => (),
      ~config=?,
      ~pubsubAdapter=?,
      ~onBeforeHandlers=?,
      ~handlers: option(list(HttpHandler.t('a)))=?,
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
    switch (handlers) {
    | Some(handlers) =>
      app |> HttpHandler.makeApp(handlers, onBeforeHandlers, pubsubAdapter)
    | None => app
    };

  let createWsApp = app =>
    switch (wsHandler) {
    | Some(handler) =>
      app |> WebsocketHandler.makeApp(handler, pubsubAdapter)
    | None => app
    };

  app |> createHttpApp |> createWsApp |> Uws.listen(port, onListen);
};