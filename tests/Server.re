open Response;
open HttpHandler;

type resDataT = {message: string};

//
let rootController = ({route, res, body}) => {
  switch (route) {
  | Get([]) => res |> sendJson({message: "hello world"})
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

// let wsHandler =
//   WebsocketHandler.make(
//     ~onUpgrade=(req, res) => Js.log("Connected"),
//     ~onMessage=
//       (body, ws) => {
//         switch (body) {
//         | Json(json) =>
//           Js.log(json);
//           Js.log(1);
//         | _ => Js.log("s")
//         }
//       },
//     (),
//   );

App.make(~port=3030, ~httpHandlers, ());