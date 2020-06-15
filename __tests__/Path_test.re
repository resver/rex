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
  test("empty string", () =>
    expect(make("")) |> toEqual([])
  );
  test("just single char: /", () =>
    expect(make("/")) |> toEqual([])
  );
  test("with preceeding /", () =>
    expect(make("/test")) |> toEqual(["test"])
  );
  test("with trailing /", () => {
    expect(make("test/")) |> toEqual(["test"])
  });
  test("with preceeding and trailing /", () =>
    expect(make("/test/")) |> toEqual(["test"])
  );
  test("with preceeding and trailing /, multilevel", () =>
    expect(make("/test/test2")) |> toEqual(["test", "test2"])
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