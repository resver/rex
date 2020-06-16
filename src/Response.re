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
  switch (data |> Js.Json.stringifyAny) {
  | Some(data) => res |> setContentType("application/json") |> send(data)
  | None =>
    res
    |> setStatus((500, "Internal Server Error"))
    |> send("Parsing Json Error")
  };
  ();
};

// TODO: make this async
// https://github.com/uNetworking/uWebSockets.js/blob/master/examples/VideoStreamer.js

[@bs.module "mime-types"] external mimeLookup: string => string = "lookup";

let sendFile = (filePath, res) => {
  let isFileExist = Node.Fs.existsSync(filePath);

  isFileExist
    ? {
      let file =
        try(Node.Fs.readFileSync(filePath, `binary)) {
        | _ => "Error reading file, or file is not exist" ++ filePath
        };
      res |> setContentType(mimeLookup(filePath)) |> end1(file);
    }
    : res
      |> setStatus((404, "Not Found"))
      |> send("File is not exist: " ++ filePath);
};