// mqtt topic syntax

type topicLevel =
  | String(string)
  | SingleLevelWildcard
  | MultiLevelWildcard;

type t = list(topicLevel);

let fromString = str =>
  str
  |> Js.String.split("/")
  |> Array.map(item =>
       switch (item) {
       | "+" => SingleLevelWildcard
       | "#" => MultiLevelWildcard
       | raw => String(raw)
       }
     );