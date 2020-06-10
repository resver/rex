type handlerT = {
  route: Route.t,
  req: Request.t,
  res: Response.t,
  query: Js.Json.t,
  body: Body.t,
  // pubsub: PubSub.t,
};

type namespaceT = string;
type t = (namespaceT, handlerT => unit);

let makeApp = ((namespace, handler), app) => {
  let namespacePath =
    switch (namespace) {
    | "/"
    | "*"
    | "" => "/*"
    | _ => "/" ++ namespace ++ "/*"
    };

  app
  |> Uws.any(
       namespacePath,
       (res, req) => {
         let path = req |> Request.getUrl();

         let method = req |> Request.getMethod() |> Method.make;

         let query = req |> Request.getQuery() |> Qs.parse;

         let route = Route.make(~method, ~path, ~namespace);

         let handlerFromBody = body =>
           handler({route, req, res, body, query});

         switch (method) {
         | Get
         | Head => handlerFromBody(Empty)
         | _ =>
           let contentType = req |> Request.getHeader("content-type");

           res
           |> Body.getFromBuffer(
                body => {handlerFromBody(Body.make(body, contentType))},
                () => {Js.log("Not a body")},
              );
         };
       },
     );
};