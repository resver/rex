// type wsT('a) = {publish: (string, 'a) => unit};

type t('a) = {
  path: Path.t,
  verb: Verb.t,
  req: Request.t,
  res: Response.t,
  query: Js.Json.t,
  body: Body.t,
  pubsub: PubSub.t('a),
};

let makeApp =
    (
      handler: t('a) => unit,
      onBeforeHttpHandler: option((Request.t, Response.t) => Response.t),
      app: Uws.t,
    ) => {
  app
  |> Uws.any("/*", (res, req) => {
       let path = req |> Request.getUrl() |> Path.make;
       let verb = req |> Request.getVerb() |> Verb.make;
       let query = req |> Request.getQuery() |> Qs.parse;

       let modifiedRes =
         switch (onBeforeHttpHandler) {
         | Some(onBeforeHandler) => onBeforeHandler(req, res)
         | None => res
         };

       let pubsub = app |> PubSub.makeForHttp;

       let handlerFromBody = body =>
         handler({req, res: modifiedRes, body, query, pubsub, verb, path});

       switch (verb) {
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