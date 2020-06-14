type t('a) = {
  publish: (topicT, messageT('a)) => unit,
  subscribe: topicT => unit,
  unsubscribe: topicT => bool,
}
and topicT = string
and messageT('a) = 'a;

let make = (adapter, ws) => {
  publish: (topic, rawMessage) => {
    let message = Js.Json.stringifyAny(rawMessage);

    switch (adapter, message) {
    | (Some(adapter), Some(msg)) => adapter.publish(topic, msg)
    | (None, Some(msg)) => ws |> Websocket.publish2(topic, msg)
    | (_, None) => Js.log("invalid message")
    };
  },
  subscribe: topic => {
    switch (adapter) {
    | Some(adapter) => adapter.subscribe(topic)
    | None => ws |> Websocket.subscribe(topic)
    };
  },
  unsubscribe: topic => {
    switch (adapter) {
    | Some(adapter) => adapter.unsubscribe(topic)
    | None => ws |> Websocket.unsubscribe(topic)
    };
  },
};

let makeForHttp = (adapter, app) => {
  publish: (topic, rawMessage) => {
    let message = Js.Json.stringifyAny(rawMessage);

    switch (adapter, message) {
    | (Some(adapter), Some(msg)) => adapter.publish(topic, msg)
    | (None, Some(msg)) => app |> Uws.publish2(topic, msg)
    | (_, None) => Js.log("invalid message")
    };
  },
  subscribe: topic => {
    switch (adapter) {
    | Some(adapter) => adapter.subscribe(topic)
    | None =>
      Js.log("Subscribe is not available in HTTP, considering use an adapter")
    };
  },
  unsubscribe: topic => {
    switch (adapter) {
    | Some(adapter) => adapter.unsubscribe(topic)
    | None =>
      Js.log(
        "Unsubscribe is not available in HTTP, considering use an adapter",
      );
      false;
    };
  },
};

module EventEmitter = {
  type t;
  [@bs.module "events"] [@bs.new] external make: unit => t = "EventEmitter";
  [@bs.send.pipe: t] external on: (string, unit => unit) => unit = "on";
  [@bs.send.pipe: t] external emit: (string, string) => unit = "emit";
} /* }*/ /* let fromEventEmitter = (e: EventEmitter.t) => */ /*   let subscribe = topic => */ /*   }*/ /*   pubsub*/ /*   let pubsub = {publish, subscribe, unsubscribe}*/ /*     e |> EventEmitter.on(topic, () => ())*/ /*   let publish = (topic, message) => e |> EventEmitter.emit(topic, message)*/;