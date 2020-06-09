type resDataT = {message: string};

module Default = {
  let home = res => res |> Response.json({message: "hello world"});
};

let httpHandler = (HttpHandler.{route, res, body, pubsub}) => {
  switch (route) {
  | Get([]) => Default.home(res)
  | Get(["user", userId]) =>
    Response.(
      res |> status((200, "OK")) |> json({message: "hello user " ++ userId})
    )

  | Get(["hello"]) =>
    pubsub.publish("hello", "world");
    Response.(res |> status((200, "OK")) |> json({message: "data"}));

  | Post([]) =>
    switch (body) {
    | Json(json) => Js.log(json)
    | Text(string) => Js.log(string)
    | raw => Js.log(raw)
    };

    Response.(res |> status((200, "OK")) |> json({message: "data"}));
  | _ => Response.(res |> json({message: "Not found"}))
  };
};

let wsHandler =
  WebsocketHandler.make(
    ~onUpgrade=(req, res) => Js.log("upgrader"),
    ~onOpen=data => Js.log(data),
    (),
  );

App.make(~port=3030, ~httpHandler, ~wsHandler, ());