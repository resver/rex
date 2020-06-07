type resDataT = {message: string};

let handler = (App.{route, req, res}) => {
  switch (route) {
  | Get([]) =>
    res
    |> Http.Response.status((200, "OK"))
    |> Http.Response.json({message: "hello world"})
  | Get(["user", userId]) =>
    res
    |> Http.Response.status((200, "OK"))
    |> Http.Response.json({message: "hello user " ++ userId})
  | _ => res |> Uws_Response.end1("Not found")
  };
};

App.make(~port=3030, ~handler, ());