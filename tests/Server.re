type resDataT = {message: string};

module Default = {
  let home = res => res |> Response.sendJson({message: "hello world"});
};

let rootHandler = (HttpHandler.{route, res, body}) => {
  switch (route) {
  | Get([]) => Default.home(res)
  | _ => Response.(res |> sendJson({message: "Not found"}))
  };
};

let userHandler = (HttpHandler.{route, res, body}) => {
  switch (route) {
  | Get([]) => Default.home(res)
  | _ => Response.(res |> sendJson({message: "Not found"}))
  };
};

let httpHandlers = [("user", userHandler), ("", rootHandler)];

let wsHandler =
  WebsocketHandler.make(
    ~onUpgrade=(req, res) => Js.log("Connected"),
    ~onMessage=
      (body, ws) => {
        switch (body) {
        | Json(json) =>
          Js.log(json);
          Js.log(1);
        | _ => Js.log("s")
        }
      },
    (),
  );

App.make(~port=3030, ~httpHandlers, ());