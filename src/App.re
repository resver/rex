let makeConfig = Uws.makeConfig;
let uws = Uws.uws;

let listen = Uws.listen;
let listenWithHost = Uws.listenWithHost;

type handlerT = {
  route: Http.Route.t,
  req: Uws.Request.t,
  res: Uws.Response.t,
  body: option(Js.Typed_array.array_buffer),
};

let make = (~port=3030, ~config=?, ~handler, ~isSSL=false, ()) => {
  let app =
    switch (config, isSSL) {
    | (Some(config), true) => uws |> Uws.app(config)
    | (Some(config), false) => uws |> Uws.sslApp(config)
    | (None, _) => uws |> Uws.appWithoutConfig()
    };

  app
  |> Uws.any("/*", (res, req) => {
       let path = req |> Uws.Request.getUrl();
       let method = req |> Uws.Request.getMethod();

       let query = req |> Uws.Request.getQuery();

       let route = Http.Route.make(~method, ~path);

       res
       |> Http.Response.getBody(
            data => {
              let body = data^;
              handler({route, req, res, body});
            },
            () => {Js.log("Not a body")},
          );
     })
  |> Uws.listen(port, _ => {
       Js.log("Server started on port " ++ port->string_of_int)
     });
};