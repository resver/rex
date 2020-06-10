include Uws_Response;

let setContentType = (string, res) => {
  res |> writeHeader("Content-Type", string);
};

let setStatus = ((code, message), res) => {
  res |> writeStatus(code->string_of_int ++ " " ++ message);
};

let send = (data, res) => {
  res |> end1(data);
};

let sendText = (data, res) => {
  res |> setContentType("text/plain") |> send(data);
};

let sendJson = (data, res) => {
  let jsonString =
    switch (data |> Js.Json.stringifyAny) {
    | Some(data) => data
    | None => "Error parsing json"
    };
  res |> setContentType("application/json") |> send(jsonString);
};