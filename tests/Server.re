type resDataT = {message: string};

let handler = (App.{route, req, res}) => {
  switch (route) {
  | Get([]) => res |> Http.Response.json({message: "hello world"})
  | _ => res |> Uws_Response.end1("Not found")
  };
};

App.make(~port=3030, ~handler, ());