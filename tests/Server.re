type resDataT = {message: string};

module Default = {
  let home = res => res |> Response.json({message: "hello world"});
};

let httpHandler = (App.{route, res, body, publish2}) => {
  switch (route) {
  | Get([]) => Default.home(res)
  | Get(["user", userId]) =>
    res
    |> Http.Response.status((200, "OK"))
    |> Http.Response.json({message: "hello user " ++ userId})

  | Get(["hello"]) =>
    publish2("/hello", "world");

    res |> Response.status((200, "OK")) |> Response.json({message: "data"});

  | Post([]) =>
    switch (body) {
    | Json(json) => Js.log(json)
    | Text(string) => Js.log(string)
    | raw => Js.log(raw)
    };

    res |> Response.status((200, "OK")) |> Response.json({message: "data"});
  | _ => res |> Http.Response.json({message: "Not found"})
  };
};

let createWsHandler = () => {
  Uws.Websocket.makeWebsocketBehavior(
    ~upgrade=
      (res, req, ctx) => {
        Js.log("hello upgrade");
        Js.log(req |> Uws.Request.getHeader(""));
        res
        |> Response.upgrade(
             {"hello": "message"},
             req |> Request.getHeader("sec-websocket-key"),
             req |> Request.getHeader("sec-websocket-protocol"),
             req |> Request.getHeader("sec-websocket-extensions"),
             ctx,
           );
        ();
      },
    ~open_=
      (ws, req) => {
        ws |> Websocket.subscribe("/hello");
        ();
      },
    ~message=
      (ws, message, isBinary) => {
        Js.log(message);
        ();
      },
    (),
  );
};

App.make(~port=3030, ~httpHandler, ~wsHandler=createWsHandler(), ());