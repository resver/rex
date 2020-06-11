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

let httpHandlers = [("", rootController)];

App.make(~port=3030, ~httpHandlers, ());