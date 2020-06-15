type t = list(string);

let removePreceeding = str => {
  switch (Js.String.get(str, 0)) {
  | "/" => Js.String.sliceToEnd(~from=1, str)
  | _ => str
  };
};
let removeTrailing = str =>
  switch (Js.String.get(str, Js.String.length(str) - 1)) {
  | "/" => Js.String.slice(~from=0, ~to_=-1, str)
  | _ => str
  };

let toString =
  fun
  | [] => "/"
  | pathList => "/" ++ (pathList |> Array.of_list |> Js.Array.joinWith("/"));

let make = rawPath =>
  switch (rawPath) {
  | ""
  | "/" => []
  | raw =>
    // remove preceeding "/" and trailing
    let normalizedPath = raw |> removePreceeding |> removeTrailing;

    // remove preceeding again, convert to list
    normalizedPath |> Js.String.split("/") |> Array.to_list;
  };