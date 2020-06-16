open Response;
open HttpHandler;

open Verb;

let handler = ({verb, path, res}) => {
  switch (verb, path) {
  | (Get, path) =>
    let dirname: option(string) = [%bs.node __dirname];
    switch (dirname) {
    | Some(dirname) =>
      res
      |> sendFile(
           dirname ++ "/../coverage/lcov-report" ++ (path |> Path.show),
         )
    | None => res |> send("file not exist")
    };
  | _ => res |> send("Not found")
  };
};

App.make(~port=3000, ~handler, ());