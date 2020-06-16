type t = list((string, string));

let removeTrailing = str =>
  switch (Js.String.get(str, Js.String.length(str) - 1)) {
  | ";" => Js.String.slice(~from=0, ~to_=-1, str)
  | _ => str
  };

let make = rawCookie => {
  let rawCookieList = rawCookie |> Js.String.split("; ") |> Array.to_list;

  rawCookieList
  |> List.map(rawCookie => {
       let cookieArr = rawCookie |> Js.String.split("=");
       let key = cookieArr->Array.get(0);
       let value =
         try(cookieArr->Array.get(1)) {
         | _ => ""
         };

       switch (value) {
       | "" => (key |> removeTrailing, value |> removeTrailing)
       | _ => (key, value |> removeTrailing)
       };
     });
};

let get = (key, cookies) => {
  let cookie = cookies |> List.find_opt(((k, _)) => {k == key});
  switch (cookie) {
  | Some((_, v)) => Some(v)
  | None => None
  };
};

let show = cookies => {
  cookies
  |> List.map(((k, v)) => {k ++ "=" ++ v})
  |> Array.of_list
  |> Js.Array.joinWith("; ");
};