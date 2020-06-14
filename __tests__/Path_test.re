open Jest;
open Path;
open Expect;
open! Expect.Operators;

describe("Remove preceeding /", () => {
  test("empty string", () =>
    expect(removePreceeding("")) === ""
  );
  test("just single character /", () =>
    expect(removePreceeding("/")) === ""
  );
  test("has preecedding /", () =>
    expect(removePreceeding("/test")) === "test"
  );
  test("has preecedding /, multilevel", () =>
    expect(removePreceeding("/test/test2")) === "test/test2"
  );
});

describe("Remove trailing /", () => {
  test("empty string", () =>
    expect(removeTrailing("")) === ""
  );
  test("just single character /", () =>
    expect(removeTrailing("/")) === ""
  );
  test("has trailing /", () =>
    expect(removeTrailing("test/")) === "test"
  );
  test("has trailing /, multilevel", () =>
    expect(removeTrailing("test/test2/")) === "test/test2"
  );
});

describe("Make path", () => {
  let rawNamespace = "";
  test("empty string", () =>
    expect(make(~rawPath="", ~rawNamespace)) |> toEqual([])
  );
  test("just single char: /", () =>
    expect(make(~rawPath="/", ~rawNamespace)) |> toEqual([])
  );
  test("with preceeding /", () =>
    expect(make(~rawPath="/test", ~rawNamespace)) |> toEqual(["test"])
  );
  test("with trailing /", () => {
    Js.log(make(~rawPath="test/", ~rawNamespace));
    expect(make(~rawPath="test/", ~rawNamespace)) |> toEqual(["test"]);
  });
  test("with preceeding and trailing /", () =>
    expect(make(~rawPath="/test/", ~rawNamespace)) |> toEqual(["test"])
  );
  test("with preceeding and trailing /, multilevel", () =>
    expect(make(~rawPath="/test/test2", ~rawNamespace))
    |> toEqual(["test", "test2"])
  );
  test("with multilevel and namespace", () =>
    expect(make(~rawPath="hello/test/test2", ~rawNamespace="/hello"))
    |> toEqual(["test", "test2"])
  );
});