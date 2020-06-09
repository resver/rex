let makeConfig = Uws.makeConfig;
let uws = Uws.uws;

let listen = Uws.listen;
let listenWithHost = Uws.listenWithHost;

type httpHandlerT = {
  route: Http.Route.t,
  req: Uws.Request.t,
  res: Uws.Response.t,
  query: Js.Json.t,
  body: Body.t,
  publish2: (string, string) => unit,
};

type wsHandlerT = {
  config: string,
  onMessage: string,
};

let make =
    (~port=3030, ~config=?, ~httpHandler, ~wsHandler=?, ~isSSL=false, ()) => {
  let app =
    switch (config, isSSL) {
    | (Some(config), true) => uws |> Uws.app(config)
    | (Some(config), false) => uws |> Uws.sslApp(config)
    | (None, _) => uws |> Uws.appWithoutConfig()
    };

  let httpApp =
    app
    |> Uws.any("/*", (res, req) => {
         let path = req |> Uws.Request.getUrl();
         let method = req |> Uws.Request.getMethod();

         let query = req |> Uws.Request.getQuery() |> Qs.parse;

         let route = Http.Route.make(~method, ~path);

         let publish2 = (topic, message) =>
           app |> Uws.publish2(topic, message);

         switch (method) {
         | "get"
         | "head" =>
           httpHandler({route, req, res, body: NoBody, query, publish2})
         | _ =>
           let contentType = req |> Uws_Request.getHeader("content-type");

           res
           |> Body.getBody(
                body => {
                  httpHandler({
                    route,
                    req,
                    res,
                    query,
                    body: Body.parseBody(body, contentType),
                    publish2,
                  });
                  ();
                },
                () => {Js.log("Not a body")},
              );
         };
       });

  let finalApp =
    switch (wsHandler) {
    | Some(wsHandler) => httpApp |> Uws.ws("/*", wsHandler)
    | None => httpApp
    };

  finalApp
  |> Uws.listen(port, _ => {
       Js.log("Server started on port " ++ port->string_of_int)
     });
};