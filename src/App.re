let makeConfig = Uws.makeConfig;
let uws = Uws.uws;

let listen = Uws.listen;
let listenWithHost = Uws.listenWithHost;

let make =
    (
      ~port=3000,
      ~onListen=_ => (),
      ~config=?,
      ~onBeforeHttpHandlers=?,
      ~httpHandlers: option(list(HttpHandler.t('a)))=?,
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
    switch (httpHandlers) {
    | Some(handlers) =>
      app |> HttpHandler.makeApp(handlers, onBeforeHttpHandlers)
    | None => app
    };

  let createWsApp = app =>
    switch (wsHandler) {
    | Some(handler) => app |> WebsocketHandler.makeApp(handler)
    | None => app
    };

  app |> createHttpApp |> createWsApp |> Uws.listen(port, onListen);
};