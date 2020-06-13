open Response;
open HttpHandler;

type resDataT = {message: string};

//
let rootController = ({route, res, pubsub}) => {
  switch (route) {
  | Get([]) => res |> sendJson({message: "hello world"})
  | Get(["publish"]) =>
    pubsub.publish("hello", {message: "send ws"});
    res |> sendJson({message: "send ws"});
  | _ => res |> sendJson({message: "Not found"})
  };
};

//
let userController = ({route, res, body}) => {
  switch (route) {
  | Get([]) => res |> sendJson({message: "hello user"})
  | Get([userId]) => res |> sendJson({message: "hello " ++ userId})
  | _ => res |> sendJson({message: "Not found"})
  };
};

let httpHandlers = [("user", userController), ("", rootController)];

[@bs.deriving abstract]
type upgradeData = {token: option(string)};

let rootWebsocketHandler =
  WebsocketHandler.make(
    ~onOpen=
      ({path, pubsub}) => {
        Js.log(path);
        pubsub.subscribe("/");
        ();
      },
    ~onMessage=
      ({body, path}) => {
        Js.log(body);
        switch (body) {
        | Json(json) => Js.log2(path, json)
        | _ => Js.log("s")
        };
      },
    (),
  );

let wsHandlers = [("", rootWebsocketHandler)];

App.make(~port=3030, ~httpHandlers, ~wsHandlers, ());