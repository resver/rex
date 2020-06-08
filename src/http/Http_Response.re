let json = (data, res) => {
  let jsonString =
    switch (data |> Js.Json.stringifyAny) {
    | Some(data) => data
    | None => "Error parsing json"
    };
  res
  |> Uws.Response.writeHeader("Content-Type", "application/json")
  |> Uws.Response.end1(jsonString);
};

let status = ((code, message), res) => {
  res |> Uws.Response.writeStatus(code->string_of_int ++ " " ++ message);
};

module Buffer = {
  [@bs.scope "Buffer"] [@bs.val] external from: 'a => 'b = "from";
  [@bs.scope "Buffer"] [@bs.val] external concat: (('a, 'b)) => 'c = "from";
  [@bs.send.pipe: 'a] external toString: string => string = "toString";
};

let getBody = (cb, err, res) => {
  let buffer = ref(None);
  res
  |> Uws.Response.onData((bytes, isLast) => {
       let chunk = bytes |> Buffer.from;
       buffer :=
         (
           switch (buffer == ref(None), isLast) {
           | (true, false) => Some(Buffer.concat((buffer, chunk)))
           | (false, false)
           | (false, true)
           | (true, true) => Some(chunk)
           }
         );

       if (isLast) {
         cb(buffer);
       };
     })
  |> Uws.Response.onAborted(err);
};