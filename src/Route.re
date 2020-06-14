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
  | Unknown(Path.t, string);

let make = (~method: Method.t, ~rawPath, ~rawNamespace) => {
  let pathList = Path.make(~rawPath, ~rawNamespace);

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
  | Unknown(method) => Unknown(pathList, method)
  };
};