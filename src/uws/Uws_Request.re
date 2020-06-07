// An HttpRequest is stack allocated
// and only accessible during the callback invocation.

type t;

type arrayBufferT = Js.Typed_array.array_buffer;

// Returns the lowercased header value or empty string.
[@bs.send.pipe: t] external getHeader: string => string = "getHeader";
[@bs.send.pipe: t]
external getHeaderBuffer: arrayBufferT => string = "getHeader";

// Returns the parsed parameter at index.
// Corresponds to route.
[@bs.send.pipe: t] external getParameter: int => string = "getParameter";

[@bs.send.pipe: t] external getUrl: unit => string = "getUrl";

[@bs.send.pipe: t] external getMethod: unit => string = "getMethod";

[@bs.send.pipe: t] external getQuery: unit => string = "getQuery";

// Loops over all headers.
type headerKeyT = string;
type headerValueT = string;
[@bs.send.pipe: t]
external forEach: (headerKeyT, headerValueT => unit) => unit = "forEach";

// Setting yield to true is to say that
// this route handler did not handle the route,
// causing the router to continue looking for
// a matching route handler, or fail.
[@bs.send.pipe: t] external setYield: bool => t = "setYield";