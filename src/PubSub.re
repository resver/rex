type t = {
  publish: (topicT, messageT) => unit,
  subscribe: topicT => unit,
}
and topicT = string
and messageT = string;

let make = (~publish=(_, _) => (), ~subscribe=_ => (), ()) => {
  let pubsub = {publish, subscribe};
  pubsub;
};