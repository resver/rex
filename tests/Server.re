open Uws;

let app = uws |> appWithoutConfig();

app
|> any("/*", (res, _) => {res |> Response.end1("hello")})
|> listen(3030, () => Js.log("hello"));