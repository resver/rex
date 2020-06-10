type t = {
  publish: (topicT, messageT) => unit,
  subscribe: (topicT, Websocket.t) => unit,
}
and topicT = string
and messageT = string;

let make = (~publish=(_, _) => (), ~subscribe=(_, _) => (), ()) => {
  let pubsub = {publish, subscribe};
  pubsub;
};

module EventEmitter = {
  type t;
  [@bs.module "events"] [@bs.new] external make: unit => t = "EventEmitter";
  [@bs.send.pipe: t] external on: (string, unit => unit) => unit = "on";
  [@bs.send.pipe: t] external emit: (string, string) => unit = "emit";
};

let fromEventEmitter = (e: EventEmitter.t) => {
  let publish = (topic, message) => e |> EventEmitter.emit(topic, message);
  let subscribe = (topic, ws) => {
    e |> EventEmitter.on(topic, () => ());
  };
  let pubsub = {publish, subscribe};
  pubsub;
};