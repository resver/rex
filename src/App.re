let makeConfig = Uws.makeConfig;
let uws = Uws.uws;

let listen = Uws.listen;
let listenWithHost = Uws.listenWithHost;

type wsHandlerT = {
  config: string,
  onMessage: Websocket.t,
};

let make =
    (
      ~port=3030,
      ~onListen=_ => (),
      ~config=?,
      ~httpHandlers: option(list(HttpHandler.t))=?,
      ~wsHandlers=?,
      ~isSSL=false,
      (),
    ) => {
  let app =
    switch (config, isSSL) {
    | (Some(config), true) => uws |> Uws.app(config)
    | (Some(config), false) => uws |> Uws.sslApp(config)
    | (None, _) => uws |> Uws.appWithoutConfig()
    };

  let httpApp =
    switch (httpHandlers) {
    | Some(httpHandlers) => app |> HttpHandler.makeApp(httpHandlers)
    | None => app
    };

  // let finalApp =
  //   switch (wsHandlers) {
  //   | Some(wsHandler) =>
  //     httpApp |> WebsocketHandler.makeApp(wsHandler)
  //   | None => httpApp
  //   };

  // finalApp |> Uws.listen(port, onListen);
  httpApp |> Uws.listen(port, onListen);
};