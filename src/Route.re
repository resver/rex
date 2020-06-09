type t =
  | Get(Path.t)
  | Post(Path.t)
  | Put(Path.t)
  | Patch(Path.t)
  | Delete(Path.t)
  | Options(Path.t)
  | Head(Path.t)
  | Connect(Path.t)
  | Trace(Path.t)
  | Any(Path.t);

let make = (~method, ~path) => {
  let pathList = Path.fromString(path);

  switch (method) {
  | "get" => Get(pathList)
  | "post" => Post(pathList)
  | "put" => Put(pathList)
  | "patch" => Patch(pathList)
  | "delete" => Delete(pathList)
  | "options" => Options(pathList)
  | "head" => Head(pathList)
  | "connect" => Connect(pathList)
  | "trace" => Trace(pathList)
  | _ => Any(pathList)
  };
};