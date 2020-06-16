open Jest;
open Cookies;
open Expect;

describe("Cookies.make", () => {
  test("make to list of tuple", () =>
    expect(make("test=value; test2=value2"))
    |> toEqual([("test", "value"), ("test2", "value2")])
  );
  test("make with trailing ;", () =>
    expect(make("test=value; test2=value2;"))
    |> toEqual([("test", "value"), ("test2", "value2")])
  );
  test("make empty string", () =>
    expect(make("testEmpty;")) |> toEqual([("testEmpty", "")])
  );
});

describe("Cookies.show", () => {
  test("from list of tuple", () =>
    expect(show([("test", "value"), ("test2", "value2")]))
    |> toEqual("test=value; test2=value2")
  )
});

describe("Cookies.get", () => {
  test("get one of list", () =>
    expect(get("test2", [("test", "value"), ("test2", "value2")]))
    |> toEqual(Some("value2"))
  );
  test("get one of list, not found", () =>
    expect(get("test3", [("test", "value"), ("test2", "value2")]))
    |> toEqual(None)
  );
});