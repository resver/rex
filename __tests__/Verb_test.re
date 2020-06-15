open Jest;
open Verb;
open Expect;

describe("Method.make", () => {
  test("make", () =>
    expect([
      make("get"),
      make("post"),
      make("put"),
      make("patch"),
      make("delete"),
      make("options"),
      make("head"),
      make("connect"),
      make("trace"),
      make("hello"),
    ])
    |> toEqual([
         Get,
         Post,
         Put,
         Patch,
         Delete,
         Options,
         Head,
         Connect,
         Trace,
         Unknown("hello"),
       ])
  )
});