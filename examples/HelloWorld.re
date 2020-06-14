open Response;
open HttpHandler;

type resDataT = {message: string};

//
let rootController = ({route, res}) => {
  switch (route) {
  | Get([]) => res |> send("hello world")
  | _ => res |> send("Not found")
  };
};

let rootNamespace = "";
let handlers = [(rootNamespace, rootController)];

App.make(~port=3000, ~handlers, ());