open Response;
open HttpHandler;

open Verb;

//
let handler = ({verb, path, res}) => {
  switch (verb, path) {
  | (Get, []) => res |> send("hello world")
  | _ => res |> send("Not found")
  };
};

App.make(~port=3000, ~handler, ());