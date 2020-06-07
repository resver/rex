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