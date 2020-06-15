open Response;
open HttpHandler;

//
let handler = ({method, path, res}) => {
  switch (method, path) {
  | (Method.Get, []) => res |> send("hello world")
  | _ => res |> send("Not found")
  };
};

App.make(~port=3000, ~handler, ());