// An HttpResponse is valid until either onAborted callback
// or any of the .end/.tryEnd calls succeed.
// You may attach user data to this object.

// TODO: Buffer, not just string

type t;

type arrayBufferT = Js.Typed_array.array_buffer;

// Writes the HTTP status message such as "200 OK"
type statusT = string;
[@bs.send.pipe: t] external writeStatus: statusT => t = "writeStatus";

// Writes key and value to HTTP response.
type headerKeyT = string;
type headerValueT = string;
[@bs.send.pipe: t]
external writeHeader: (headerKeyT, headerValueT) => t = "writeHeader";

// Enters or continues chunked encoding mode.
// Writes part of the response. End with zero length write.
type chunkT = string;
[@bs.send.pipe: t] external write: chunkT => t = "write";

// Ends this response by copying the contents of body.
type bodyT = string;
[@bs.send.pipe: t] external end0: unit => unit = "end";
[@bs.send.pipe: t] external end1: bodyT => unit = "end";
[@bs.send.pipe: t] external endBuffer1: arrayBufferT => unit = "end";

// Ends this response, or tries to,
// by streaming appropriately sized chunks of body.
// Use in conjunction with onWritable.
// Returns tuple [ok, hasResponded].
type totalSizeT = string;
[@bs.send.pipe: t]
external tryEnd: (string, totalSizeT) => (bool, bool) = "tryEnd";
external tryEndBuffer: (arrayBufferT, totalSizeT) => (bool, bool) = "tryEnd";

// Immediately force closes the connection.
[@bs.send.pipe: t] external close: unit => t = "close";

// Returns the global byte write offset for this response.
// Use with onWritable.
[@bs.send.pipe: t] external getWriteOffset: unit => float = "getWriteOffset";

// Registers a handler for writable events.
// Continue failed write attempts in here.
// You MUST return true for success, false for failure.
// Writing nothing is always success, so by default you must return true.
type offsetT = float;
type writableHandlerT = offsetT => bool;
[@bs.send.pipe: t] external onWritable: writableHandlerT => 'a = "onWritable";

[@bs.send.pipe: t] external onAborted: (unit => unit) => 'a = "onAborted";

// Handler for reading data from POST and such requests.
// You MUST copy the data of chunk if isLast is not true.
// We Neuter ArrayBuffers on return, making it zero length.
type isLastT = bool;
[@bs.send.pipe: t]
external onData: ((arrayBufferT, isLastT) => 'a) => 'b = "onData";

// Returns the remote IP address
[@bs.send.pipe: t]
external getRemoteAddress: unit => arrayBufferT = "getRemoteAddress";

/* Corking a response is a performance improvement in both CPU and network, as you ready the IO system for writing multiple chunks at once.
 * By default, you're corked in the immediately executing top portion of the route handler. In all other cases, such as when returning from
 * await, or when being called back from an async database request or anything that isn't directly executing in the route handler, you'll want
 * to cork before calling writeStatus, writeHeader or just write. Corking takes a callback in which you execute the writeHeader, writeStatus and
 * such calls, in one atomic IO operation. This is important, not only for TCP but definitely for TLS where each write would otherwise result
 * in one TLS block being sent off, each with one send syscall.
 *
 * Example usage:
 *
 * res.cork(() => {
 *   res.writeStatus("200 OK").writeHeader("Some", "Value").write("Hello world!");
 * }); */
[@bs.send.pipe: t] external cork: (unit => unit) => unit = "cork";

[@bs.send.pipe: t]
external upgrade: ('a, string, string, string, 'b) => unit = "upgrade";