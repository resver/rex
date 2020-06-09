// mqtt topic syntax

type t = list(topicLevel)
and topicLevel =
  | String(string)
  | SingleLevelWildcard
  | MultiLevelWildcard;

let make = str =>
  str
  |> Js.String.split("/")
  |> Array.map(item =>
       switch (item) {
       | "+" => SingleLevelWildcard
       | "#" => MultiLevelWildcard
       | raw => String(raw)
       }
     );