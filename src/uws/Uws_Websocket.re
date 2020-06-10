type t;

type arrayBufferT = Js.Typed_array.array_buffer;

// Sends a message.
// Make sure to check getBufferedAmount() before sending.
// Returns true for success, false for built up backpressure
// that will drain when time is given.
type isBinaryT = bool;
type compressT = bool;
[@bs.send.pipe: t] external send1: string => bool = "send";
[@bs.send.pipe: t] external sendBuffer1: arrayBufferT => bool = "send";
[@bs.send.pipe: t]
external send3: (string, isBinaryT, compressT) => bool = "send";
external sendBuffer3: (arrayBufferT, isBinaryT, compressT) => bool = "send";

// Returns the bytes buffered in backpressure.
[@bs.send.pipe: t]
external getBufferedAmount: unit => float = "getBufferedAmount";

// Gracefully closes this WebSocket.
// Immediately calls close handler.
[@bs.send.pipe: t] external end0: unit => t = "end";
type codeT = int;
[@bs.send.pipe: t] external end2: (codeT, string) => t = "end";
[@bs.send.pipe: t] external endBuffer2: (codeT, arrayBufferT) => t = "end";

// Forcefully closes this WebSocket
[@bs.send.pipe: t] external close: unit => t = "close";

[@bs.send.pipe: t] external ping0: unit => t = "ping";
[@bs.send.pipe: t] external ping1: string => t = "ping";
[@bs.send.pipe: t] external pingBuffer1: arrayBufferT => t = "ping";

// Subscribe to a topic in MQTT syntax
[@bs.send.pipe: t] external subscribe: string => t = "subscribe";
[@bs.send.pipe: t] external subscribeBuffer: arrayBufferT => t = "subscribe";

// Unsubscribe from a topic.
// Returns true on success, if the WebSocket was subscribed.
[@bs.send.pipe: t] external unsubscribe: string => bool = "unsubscribe";
[@bs.send.pipe: t]
external unsubscribeBuffer: arrayBufferT => bool = "unsubscribe";

// Unsubscribe from all topics.
[@bs.send.pipe: t] external unsubscribeAll: unit => unit = "unsubscribeAll";

[@bs.send.pipe: t] external publish2: (string, string) => unit = "publish";
[@bs.send.pipe: t]
external publishBuffer2: (string, arrayBufferT) => unit = "publish";
[@bs.send.pipe: t]
external publish3: (string, string, isBinaryT) => unit = "publish";
[@bs.send.pipe: t]
external publishBuffer3: (string, arrayBufferT, isBinaryT) => unit = "publish";
[@bs.send.pipe: t]
external publish4: (string, string, isBinaryT, compressT) => unit = "publish";
[@bs.send.pipe: t]
external publishBuffer4: (string, arrayBufferT, isBinaryT, compressT) => unit =
  "publish";

[@bs.send.pipe: t]
external upgrade: (Uws_Response.t, Uws_Response.t, 'a) => unit = "publish";

[@bs.send.pipe: t] external cork: (unit => unit) => unit = "cork";

[@bs.send.pipe: t]
external getRemoteAddress: unit => arrayBufferT = "getRemoteAddress";

type websocketBehaviorT = {
  maxPayloadLength: option(int),
  idleTimeout: option(int),
  compression: option(int),
  maxBackpressure: option(int),
  [@bs.as "open"]
  open_: option((t, Uws_Request.t) => unit),
  upgrade:
    option((Uws_Response.t, Uws_Request.t, option(Js.Json.t)) => unit),
  message: option((t, arrayBufferT, isBinaryT) => unit),
  drain: option(t => unit),
  close: option((t, codeT, arrayBufferT) => unit),
  ping: option(t => unit),
  pong: option(t => unit),
};

let makeWebsocketBehavior =
    (
      ~maxPayloadLength=?,
      ~idleTimeout=?,
      ~compression=?,
      ~maxBackpressure=?,
      ~open_=?,
      ~message=?,
      ~upgrade=?,
      ~drain=?,
      ~close=?,
      ~ping=?,
      ~pong=?,
      (),
    ) => {
  let websocketBehavior = {
    maxPayloadLength,
    idleTimeout,
    compression,
    maxBackpressure,
    open_,
    upgrade,
    message,
    drain,
    close,
    ping,
    pong,
  };
  websocketBehavior;
};