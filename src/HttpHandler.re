type t = {
  route: Route.t,
  req: Request.t,
  res: Response.t,
  query: Js.Json.t,
  body: Body.t,
  pubsub: PubSub.t,
};

let makeApp = (handler, app) => {
  app
  |> Uws.any("/*", (res, req) => {
       let path = req |> Request.getUrl();

       let method = req |> Request.getMethod() |> Method.make;

       let query = req |> Request.getQuery() |> Qs.parse;

       let route = Route.make(~method, ~path);

       let publish = (topic, message) => app |> Uws.publish2(topic, message);

       let pubsub = PubSub.make(~publish, ());

       let handlerFromBody = body =>
         handler({route, req, res, body, query, pubsub});

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