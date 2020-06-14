type t =
  | Bearer(string)
  | Basic(string)
  | Raw(string);

let make = rawAuthorization => {
  let authorization =
    rawAuthorization |> Js.String.split(" ") |> Array.to_list;
  switch (authorization) {
  | ["Bearer", token] => Bearer(token)
  | ["Basic", token] => Basic(token)
  | _ => Raw(rawAuthorization)
  };
};