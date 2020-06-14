open Response;
open HttpHandler;

type resDataT = {message: string};

//
let rootController = ({route, res}) => {
  switch (route) {
  | Get([]) => res |> sendJson({message: "hello world"})
  | _ => res |> sendJson({message: "Not found"})
  };
};

let rootNamespace = "";
let handlers = [(rootNamespace, rootController)];

App.make(~port=3000, ~handlers, ());