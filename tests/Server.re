type resDataT = {message: string};

module Default = {
  let home = res => res |> Http.Response.json({message: "hello world"});
};

let handler = (App.{route, req, res, body}) => {
  switch (route) {
  | Get([]) => Default.home(res)
  | Get(["user", userId]) =>
    res
    |> Http.Response.status((200, "OK"))
    |> Http.Response.json({message: "hello user " ++ userId})

  | Post([]) =>
    switch (body) {
    | Json(json) => Js.log(json)
    | Text(string) => Js.log(string)
    | raw => Js.log(raw)
    };

    res
    |> Http.Response.status((200, "OK"))
    |> Http.Response.json({message: "data"});
  | _ => res |> Http.Response.json({message: "Not found"})
  };
};

App.make(~port=3030, ~handler, ());