module Response = Uws_Response;
module Request = Uws_Request;
module Websocket = Uws_Websocket;

type t;

type uwsT;

type appConfigT = {
  [@bs.as "key_file_name"]
  keyFileName: option(string),
  [@bs.as "cert_file_name"]
  certFileName: option(string),
  passphrase: option(string),
  [@bs.as "dh_params_file_name"]
  dhParamsFileName: option(string),
  /** This translates to SSL_MODE_RELEASE_BUFFERS */
  [@bs.as "ssl_prefer_low_memory_usage"]
  sslPreferLowMemoryUsage: option(bool),
};

let makeConfig =
    (
      ~keyFileName=?,
      ~certFileName=?,
      ~passphrase=?,
      ~dhParamsFileName=?,
      ~sslPreferLowMemoryUsage=?,
      (),
    ) => {
  let config = {
    keyFileName,
    certFileName,
    passphrase,
    dhParamsFileName,
    sslPreferLowMemoryUsage,
  };
  config;
};

[@bs.module] external uws: uwsT = "uWebsockets.js";

[@bs.send.pipe: uwsT] external app: appConfigT => t = "App";
[@bs.send.pipe: uwsT] external sslApp: appConfigT => t = "SSLApp";
[@bs.send.pipe: uwsT] external appWithoutConfig: unit => t = "App";

type hostT = string;
type portT = int;
type listenCallbackT = unit => unit;

[@bs.send.pipe: t]
external listenWithHost: (hostT, portT, listenCallbackT) => t = "listen";
[@bs.send.pipe: t] external listen: (portT, listenCallbackT) => t = "listen";

type patternT = string;
type handlerT = (Response.t, Request.t) => unit;
[@bs.send.pipe: t] external get: (patternT, handlerT) => t = "get";
[@bs.send.pipe: t] external post: (patternT, handlerT) => t = "post";
[@bs.send.pipe: t] external options: (patternT, handlerT) => t = "options";
[@bs.send.pipe: t] external del: (patternT, handlerT) => t = "del";
[@bs.send.pipe: t] external patch: (patternT, handlerT) => t = "patch";
[@bs.send.pipe: t] external put: (patternT, handlerT) => t = "put";
[@bs.send.pipe: t] external head: (patternT, handlerT) => t = "head";
[@bs.send.pipe: t] external connect: (patternT, handlerT) => t = "connect";
[@bs.send.pipe: t] external trace: (patternT, handlerT) => t = "trace";
[@bs.send.pipe: t] external any: (patternT, handlerT) => t = "any";
//
[@bs.send.pipe: t]
external ws: (patternT, Websocket.websocketBehaviorT) => t = "ws";
[@bs.send.pipe: t] external publish2: (patternT, string) => t = "publish";
[@bs.send.pipe: t]
external publish4: (patternT, string, bool, bool) => t = "publish";