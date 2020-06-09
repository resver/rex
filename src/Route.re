type pathT = list(string);
type t =
  | Get(pathT)
  | Post(pathT)
  | Put(pathT)
  | Patch(pathT)
  | Delete(pathT)
  | Options(pathT)
  | Head(pathT)
  | Connect(pathT)
  | Trace(pathT)
  | Any(pathT);

let pathToList =
  fun
  | ""
  | "/" => []
  | raw => {
      /* remove the preceeding /, which every pathname seems to have */
      let raw = Js.String.sliceToEnd(~from=1, raw);
      /* remove the trailing /, which some pathnames might have. Ugh */
      let raw =
        switch (Js.String.get(raw, Js.String.length(raw) - 1)) {
        | "/" => Js.String.slice(~from=0, ~to_=-1, raw)
        | _ => raw
        };
      raw |> Js.String.split("/") |> Array.to_list;
    };

let make = (~method, ~path) => {
  let pathList = pathToList(path);

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