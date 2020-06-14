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
    expect(make(~rawPath="test/", ~rawNamespace)) |> toEqual(["test"])
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

describe("Make string", () => {
  let rawNamespace = "";
  test("empty string", () =>
    expect(makeString(~rawPath="", ~rawNamespace)) |> toEqual("/")
  );
  test("just single char: /", () =>
    expect(makeString(~rawPath="/", ~rawNamespace)) |> toEqual("/")
  );
  test("with preceeding /", () =>
    expect(makeString(~rawPath="/test", ~rawNamespace)) |> toEqual("/test")
  );
  test("with trailing /", () => {
    expect(makeString(~rawPath="test/", ~rawNamespace)) |> toEqual("/test")
  });
  test("with preceeding and trailing /", () =>
    expect(makeString(~rawPath="/test/", ~rawNamespace)) |> toEqual("/test")
  );
  test("with preceeding and trailing /, multilevel", () =>
    expect(makeString(~rawPath="/test/test2", ~rawNamespace))
    |> toEqual("/test/test2")
  );
  test("with multilevel and namespace", () =>
    expect(makeString(~rawPath="hello/test/test2", ~rawNamespace="/hello"))
    |> toEqual("/test/test2")
  );
});

describe("Path to string", () => {
  test("empty list", () =>
    expect(toString([])) |> toEqual("/")
  );
  test("list length 1", () =>
    expect(toString(["test"])) |> toEqual("/test")
  );
  test("list length 2", () =>
    expect(toString(["test", "test2"])) |> toEqual("/test/test2")
  );
});