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

let makeApp = (handlers: list(t), app: Uws.t) => {
  app
  |> Uws.any("/*", (res, req) => {
       let rawPath = req |> Request.getUrl();

       let method = req |> Request.getMethod() |> Method.make;

       let query = req |> Request.getQuery() |> Qs.parse;

       // pick one handler from list of handlers
       let (rawNamespace, handler) =
         handlers
         |> List.find(((rawNamespace, _): t) => {
              let normalizedPath =
                Path.(rawPath |> removePreceeding |> removeTrailing);

              let normalizedNamespace =
                Path.(rawNamespace |> removePreceeding |> removeTrailing);

              Js.log([|normalizedPath, normalizedNamespace|]);

              normalizedPath |> Js.String.startsWith(normalizedNamespace);
            });

       Js.log(Path.(rawNamespace |> removePreceeding |> removeTrailing));

       let route = Route.make(~method, ~rawPath, ~rawNamespace);
       let handlerFromBody = body => handler({route, req, res, body, query});
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
     });
};