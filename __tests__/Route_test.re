open Jest;
open Expect;

open Route;

describe("Make route", () => {
  test(
    "Get, Post, Put, Patch, Options, Head, Trace, Connect, unknown to path /",
    () => {
    let rawPath = "/";
    let rawNamespace = "";
    expect([
      make(~method=Method.Get, ~rawPath, ~rawNamespace),
      make(~method=Method.Post, ~rawPath, ~rawNamespace),
      make(~method=Method.Put, ~rawPath, ~rawNamespace),
      make(~method=Method.Patch, ~rawPath, ~rawNamespace),
      make(~method=Method.Delete, ~rawPath, ~rawNamespace),
      make(~method=Method.Options, ~rawPath, ~rawNamespace),
      make(~method=Method.Head, ~rawPath, ~rawNamespace),
      make(~method=Method.Trace, ~rawPath, ~rawNamespace),
      make(~method=Method.Connect, ~rawPath, ~rawNamespace),
      make(~method=Method.Unknown("unknown"), ~rawPath, ~rawNamespace),
    ])
    |> toEqual([
         Get([]),
         Post([]),
         Put([]),
         Patch([]),
         Delete([]),
         Options([]),
         Head([]),
         Trace([]),
         Connect([]),
         Unknown([], "unknown"),
       ]);
  });
  test("Get to path /test/test2", () => {
    let method = Method.Get;
    let rawPath = "/test/test2";
    let rawNamespace = "";
    expect(make(~method, ~rawPath, ~rawNamespace))
    |> toEqual(Get(["test", "test2"]));
  });

  test("Get to path hello/test/test2 with namespace hello", () => {
    let method = Method.Get;
    let rawPath = "hello/test/test2";
    let rawNamespace = "hello";
    expect(make(~method, ~rawPath, ~rawNamespace))
    |> toEqual(Get(["test", "test2"]));
  });
});