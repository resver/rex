let makeConfig = Uws.makeConfig;
let uws = Uws.uws;

let listen = Uws.listen;
let listenWithHost = Uws.listenWithHost;

let make =
    (
      ~port=3030,
      ~onListen=_ => (),
      ~config=?,
      ~httpHandlers: option(list(HttpHandler.t('a)))=?,
      ~wsHandlers: option(list(WebsocketHandler.t('ctx, 'a)))=?,
      ~wsConfig=?,
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
    | Some(handlers) => app |> HttpHandler.makeApp(handlers)
    | None => app
    };

  let createWsApp = app =>
    switch (wsHandlers) {
    | Some(handlers) =>
      app |> WebsocketHandler.makeApp(handlers, ~config=?wsConfig)
    | None => app
    };

  app |> createHttpApp |> createWsApp |> Uws.listen(port, onListen);
};