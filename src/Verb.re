type t =
  | Get
  | Post
  | Put
  | Patch
  | Delete
  | Options
  | Head
  | Trace
  | Connect
  | Unknown(string);

let make =
  fun
  | "get" => Get
  | "post" => Post
  | "put" => Put
  | "patch" => Patch
  | "delete" => Delete
  | "options" => Options
  | "head" => Head
  | "connect" => Connect
  | "trace" => Trace
  | str => Unknown(str);