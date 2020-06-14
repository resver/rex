type t = list((string, string));

let make = rawCookie => {
  let rawCookieList = rawCookie |> Js.String.split("; ") |> Array.to_list;

  rawCookieList
  |> List.map(rawCookie => {
       let cookieArr = rawCookie |> Js.String.split("=");
       let key =
         try(cookieArr->Array.get(0)) {
         | _ => ""
         };
       let value =
         try(cookieArr->Array.get(1)) {
         | _ => ""
         };
       (key, value);
     });
};

let get = (key, cookies) => {
  cookies |> List.find_opt(((k, _)) => {k == key});
};

let toString = cookies => {
  cookies
  |> List.fold_left(
       (l, r) => {
         let (k, v) = r;
         l ++ k ++ "=" ++ v;
       },
       "",
     );
};