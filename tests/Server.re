type resDataT = {message: string};

// type arrayBufferT
module UBuffer = {
  [@bs.scope "Buffer"] [@bs.val] external from: 'a => 'b = "from";
  [@bs.scope "Buffer"] [@bs.val] external concat: (('a, 'b)) => 'c = "from";
};

let handler = (App.{route, req, res, body}) => {
  switch (route) {
  | Get([]) =>
    res
    |> Http.Response.status((200, "OK"))
    |> Http.Response.json({message: "hello world"})
  | Get(["user", userId]) =>
    res
    |> Http.Response.status((200, "OK"))
    |> Http.Response.json({message: "hello user " ++ userId})
  | Post([]) =>
    let data =
      switch (body) {
      | Some(body) => body |> Http.Response.Buffer.toString("utf8")
      | None => ""
      };
    res
    |> Http.Response.status((200, "OK"))
    |> Http.Response.json({message: data});
  | _ => res |> Http.Response.json({message: "Not found"})
  };
};

App.make(~port=3030, ~handler, ());