open Jest;
open Authorization;
open Expect;

describe("Authorization.make", () => {
  test("make from Bearer", () =>
    expect(make("Bearer token123")) |> toEqual(Bearer("token123"))
  );
  test("make from Bearer without token", () =>
    expect(make("Bearer")) |> toEqual(Raw("Bearer"))
  );
  test("make from Basic", () =>
    expect(make("Basic token123==")) |> toEqual(Basic("token123=="))
  );
  test("make from Basic without token", () =>
    expect(make("Basic")) |> toEqual(Raw("Basic"))
  );
  test("make other", () =>
    expect(make("Test token123==")) |> toEqual(Raw("Test token123=="))
  );
  test("make no space", () =>
    expect(make("asdfasdf")) |> toEqual(Raw("asdfasdf"))
  );
  test("make empty", () =>
    expect(make("")) |> toEqual(Raw(""))
  );
});