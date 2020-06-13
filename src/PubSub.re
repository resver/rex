type t('a) = {
  publish: (topicT, messageT('a)) => unit,
  subscribe: topicT => unit,
}
and topicT = string
and messageT('a) = 'a;

let make = ws => {
  publish: (topic, rawMessage) => {
    let message = Js.Json.stringifyAny(rawMessage);
    switch (message) {
    | Some(msg) => ws |> Websocket.publish2(topic, msg)
    | None => Js.log("invalid message")
    };
  },
  subscribe: topic => {
    ws |> Websocket.subscribe(topic);
  },
};

module EventEmitter = {
  type t;
  [@bs.module "events"] [@bs.new] external make: unit => t = "EventEmitter";
  [@bs.send.pipe: t] external on: (string, unit => unit) => unit = "on";
  [@bs.send.pipe: t] external emit: (string, string) => unit = "emit";
};

let fromEventEmitter = (e: EventEmitter.t) => {
  let publish = (topic, message) => e |> EventEmitter.emit(topic, message);
  let subscribe = topic => {
    e |> EventEmitter.on(topic, () => ());
  };
  let pubsub = {publish, subscribe};
  pubsub;
};