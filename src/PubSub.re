type topicT = string;
type messageT = string;

type t = {
  publish: (topicT, messageT) => unit,
  subscribe: topicT => unit,
};