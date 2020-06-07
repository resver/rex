let json = (data, req) => {
  let jsonString =
    switch (data |> Js.Json.stringifyAny) {
    | Some(data) => data
    | None => "Error parsing json"
    };
  req
  |> Uws.Response.writeHeader("Content-Type", "application/json")
  |> Uws.Response.end1(jsonString);
};

let status = ((code, message), req) => {
  req |> Uws.Response.writeStatus(code->string_of_int ++ " " ++ message);
};