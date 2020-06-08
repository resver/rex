include Uws_Response;

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