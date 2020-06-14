include Uws_Request;

let getContentType = req => {
  let rawContentType = req |> getHeader("content-type");
  rawContentType |> Js.String.split("; ") |> Array.to_list;
};

let getAuthorization = req => {
  let rawAuthorization = req |> getHeader("authorization");
  Authorization.make(rawAuthorization);
};

let getCookies = req => {
  let rawCookie = req |> getHeader("cookie");
  Cookies.make(rawCookie);
};