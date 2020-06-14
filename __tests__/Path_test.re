open Jest;
open Path;
open Expect;
open! Expect.Operators;

describe("Remove preceeding /", () => {
  test("empty string", () =>
    expect(removePreceeding("")) === ""
  );
  test("just single char: /", () =>
    expect(removePreceeding("/")) === ""
  );
  test("has preecedding /test", () =>
    expect(removePreceeding("/test")) === "test"
  );
  test("has preecedding /test/test2", () =>
    expect(removePreceeding("/test/test2")) === "test/test2"
  );
});

describe("Remove trailing /", () => {
  test("empty string", () =>
    expect(removeTrailing("")) === ""
  );
  test("just single char: /", () =>
    expect(removeTrailing("/")) === ""
  );
  test("has preecedding test/", () =>
    expect(removeTrailing("test/")) === "test"
  );
  test("has preecedding test/test2/", () =>
    expect(removeTrailing("test/test2/")) === "test/test2"
  );
});