type t('a) = {
  publish: (topicT, messageT('a)) => unit,
  subscribe: topicT => unit,
  unsubscribe: topicT => bool,
}
and topicT = string
and messageT('a) = 'a;

let make = ws => {
  publish: (topic, rawMessage) => {
    let message = Js.Json.stringifyAny(rawMessage);

    switch (message) {
    | Some(msg) => ws |> Websocket.publish2(topic, msg)
    | _ => Js.log("invalid message")
    };
  },
  subscribe: topic => ws |> Websocket.subscribe(topic),
  unsubscribe: topic => ws |> Websocket.unsubscribe(topic),
};

let makeForHttp = app => {
  publish: (topic, rawMessage) => {
    let message = Js.Json.stringifyAny(rawMessage);

    switch (message) {
    | Some(msg) => app |> Uws.publish2(topic, msg)
    | _ => Js.log("invalid message")
    };
  },
  subscribe: _ =>
    Js.log("Subscribe is not available in HTTP, considering use an adapter"),

  unsubscribe: _ => {
    Js.log(
      "Unsubscribe is not available in HTTP, considering use an adapter",
    );
    false;
  },
};