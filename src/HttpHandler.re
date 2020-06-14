// type wsT('a) = {publish: (string, 'a) => unit};

type handlerT('a) = {
  route: Route.t,
  req: Request.t,
  res: Response.t,
  query: Js.Json.t,
  body: Body.t,
  pubsub: PubSub.t('a),
  namespace: string,
};

type namespaceT = string;
type t('a) = (namespaceT, handlerT('a) => unit);

let makeApp =
    (
      handlers: list(t('a)),
      onBeforeHttpHandlers: option((Request.t, Response.t) => Response.t),
      app: Uws.t,
    ) => {
  app
  |> Uws.any("/*", (res, req) => {
       let rawPath = req |> Request.getUrl();
       let method = req |> Request.getMethod() |> Method.make;
       let query = req |> Request.getQuery() |> Qs.parse;

       let modifiedRes =
         switch (onBeforeHttpHandlers) {
         | Some(onBeforeHandler) => onBeforeHandler(req, res)
         | None => res
         };

       // pick one handler from list of handlers
       // by check prefix of path == namespace
       let (rawNamespace, handler) =
         try(
           handlers
           |> List.find(((rawNamespace, _): t('a)) => {
                let normalizedPath =
                  Path.(rawPath |> removePreceeding |> removeTrailing);

                let normalizedNamespace =
                  Path.(rawNamespace |> removePreceeding |> removeTrailing);

                let found =
                  switch (normalizedNamespace, normalizedPath) {
                  | ("", _) => true
                  | (namespace, path) =>
                    path == namespace
                    || path
                    |> Js.String.startsWith(namespace ++ "/")
                  };
                found;
              })
         ) {
         | Not_found => ("", (_ => ()))
         };

       let namespace =
         Path.(rawNamespace |> removePreceeding |> removeTrailing);

       let pubsub =
         PubSub.{
           publish: (path, rawMessage) => {
             let message = Js.Json.stringifyAny(rawMessage);
             let fullPath =
               switch (namespace) {
               | "" => path
               | str => namespace ++ "/" ++ str
               };
             switch (message) {
             | Some(msg) => app |> Uws.publish2(fullPath, msg)
             | None => Js.log("invalid message")
             };
           },
           subscribe: _ => {
             Js.log("Subscribe is not available in HTTP");
           },
         };

       let route = Route.make(~method, ~rawPath, ~rawNamespace);
       let handlerFromBody = body =>
         handler({
           route,
           req,
           res: modifiedRes,
           body,
           query,
           pubsub,
           namespace,
         });

       switch (method) {
       | Get
       | Head => handlerFromBody(Empty)
       | _ =>
         let contentType = req |> Request.getContentType |> List.hd;
         res
         |> Body.getFromBuffer(
              body => {handlerFromBody(Body.make(body, contentType))},
              () => {Js.log("Not a body")},
            );
       };
     });
};