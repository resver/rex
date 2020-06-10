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

let make = (~rawPath, ~rawNamespace) =>
  switch (rawPath) {
  | ""
  | "/" => []
  | raw =>
    // remove preceeding "/" and trailing
    let normalizedPath = raw |> removePreceeding |> removeTrailing;
    let normalizedNamespace =
      rawNamespace |> removePreceeding |> removeTrailing;

    // if begin with namespace, remove namespace
    let removedPath =
      normalizedPath |> Js.String.startsWith(normalizedNamespace)
        ? normalizedPath
          |> Js.String.sliceToEnd(~from=String.length(normalizedNamespace))
        : normalizedPath;

    // remove preceeding again, convert to list
    removedPath |> removePreceeding |> Js.String.split("/") |> Array.to_list;
  };