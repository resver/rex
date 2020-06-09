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

let make = (~method: Method.t, ~path) => {
  let pathList = Path.make(path);

  switch (method) {
  | Get => Get(pathList)
  | Post => Post(pathList)
  | Put => Put(pathList)
  | Patch => Patch(pathList)
  | Delete => Delete(pathList)
  | Options => Options(pathList)
  | Head => Head(pathList)
  | Connect => Connect(pathList)
  | Trace => Trace(pathList)
  | Unknown(_) => Any(pathList)
  };
};