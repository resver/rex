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

let sendFile = (filePath, res) => {
  let isFileExist = Node.Fs.existsSync(filePath);
  let toArrayBuffer: 'a => arrayBufferT = [%bs.raw
    {|
      function toArrayBuffer(buffer) {
        return buffer.buffer.slice(buffer.byteOffset, buffer.byteOffset + buffer.byteLength);
      }
    |}
  ];

  Js.log(filePath);
  Js.log(isFileExist);

  isFileExist
    ? {
      let file = Node.Fs.readFileSync(filePath, `binary);
      let totalSize: string => int = [%bs.raw
        {| function getTotalSize(fileName) {
        const fs = require('fs');
        fs.statSync(fileName).size;
      } |}
      ];
      res |> end1(file);
    }
    : res
      |> setStatus((500, "Internal Server Error"))
      |> send("File is not exist: " ++ filePath);
};