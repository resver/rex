include Uws_Websocket;

[@bs.get] external getRawPath: t => option(string) = "rawPath";
[@bs.get] external getRawNamespace: t => option(string) = "rawNamespace";
[@bs.get] external getRawQuery: t => option(string) = "rawQuery";