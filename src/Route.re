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

let make = (~method: Method.t, ~path, ~namespace) => {
  let pathList = Path.make(path);

  Js.log(namespace);
  Js.log(path);
  Js.log(pathList |> Array.of_list);

  let p =
    switch (namespace) {
    | "" => pathList
    | _ =>
      switch (pathList) {
      | [] => [] // is impossible
      | [_] => [] // delete first element
      | [_, ...tl] => tl
      }
    };

  Js.log(p |> Array.of_list);

  switch (method) {
  | Get => Get(p)
  | Post => Post(p)
  | Put => Put(p)
  | Patch => Patch(p)
  | Delete => Delete(p)
  | Options => Options(p)
  | Head => Head(p)
  | Connect => Connect(p)
  | Trace => Trace(p)
  | Unknown(_) => Any(p)
  };
};