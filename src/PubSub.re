type t('a) = {
  publish: (topicT, messageT('a)) => unit,
  subscribe: topicT => unit,
  unsubscribe: topicT => bool,
}
and topicT = string
and messageT('a) = 'a;

type adapterT('a) = {
  onPublish: (topicT, messageT('a), optWsT) => unit,
  onSubscribe: (topicT, optWsT) => unit,
  onUnsubscribe: (topicT, optWsT) => bool,
  onWsOpen: (adapterArg('a), optWsT) => unit,
}
and optWsT = option(Websocket.t)
and adapterArg('a) = {
  path: Path.t,
  query: Js.Json.t,
  pubsub: t('a),
  ws: Websocket.t,
};

let make = (adapter, ws) => {
  publish: (topic, rawMessage) => {
    let message = Js.Json.stringifyAny(rawMessage);

    switch (adapter, message) {
    | (Some(adapter), Some(msg)) =>
      Some(ws) |> adapter.onPublish(topic, msg)
    | (None, Some(msg)) => ws |> Websocket.publish2(topic, msg)
    | (_, None) => Js.log("invalid message")
    };
  },
  subscribe: topic => {
    switch (adapter) {
    | Some(adapter) => Some(ws) |> adapter.onSubscribe(topic)
    | None => ws |> Websocket.subscribe(topic)
    };
  },
  unsubscribe: topic => {
    switch (adapter) {
    | Some(adapter) => Some(ws) |> adapter.onUnsubscribe(topic)
    | None => ws |> Websocket.unsubscribe(topic)
    };
  },
};

let makeForHttp = (adapter, app) => {
  publish: (topic, rawMessage) => {
    let message = Js.Json.stringifyAny(rawMessage);

    switch (adapter, message) {
    | (Some(adapter), Some(msg)) => None |> adapter.onPublish(topic, msg)
    | (None, Some(msg)) => app |> Uws.publish2(topic, msg)
    | (_, None) => Js.log("invalid message")
    };
  },
  subscribe: topic => {
    switch (adapter) {
    | Some(adapter) => None |> adapter.onSubscribe(topic)
    | None =>
      Js.log("Subscribe is not available in HTTP, considering use an adapter")
    };
  },
  unsubscribe: topic => {
    switch (adapter) {
    | Some(adapter) => None |> adapter.onUnsubscribe(topic)
    | None =>
      Js.log(
        "Unsubscribe is not available in HTTP, considering use an adapter",
      );
      false;
    };
  },
};