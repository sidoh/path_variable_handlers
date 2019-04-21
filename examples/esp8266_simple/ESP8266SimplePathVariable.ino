#include <Arduino.h>

#include <ESP8266WebServer.h>
#include <PathVariableHandler.h>

const char* WIFI_SSID = "ssid";
const char* WIFI_PASSWORD = "password";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // Handle requests of the form GET /things/:thing_id.
  // For example, the request `GET /things/abc` would bind `abc` to the request
  // variable `thing_id`.
  server.addHandler(
    new PathVariableHandler(
      "/things/:thing_id",
      HTTP_GET,
      [](const UrlTokenBindings* bindings) {
        String response = "Thing ID was: " + String(bindings->get("thing_id"));

        server.send(200, "text/plain", response);
      }
    )
  );

  // Handle requests of the form GET /things/:thing_id/sub_things/:sub_thing_id.
  // For example, the request `GET /things/abc/sub_things/123` would bind `abc`
  // to the request request variable `thing_id`, and `123` to `sub_thing_id`.
  server.addHandler(
    new PathVariableHandler(
      "/things/:thing_id/sub_things/:sub_thing_id",
      HTTP_GET,
      [](const UrlTokenBindings* bindings) {
        String response = "Thing ID was: "
          + String(bindings->get("thing_id"))
          + ", sub thing ID was: "
          + String(bindings->get("sub_thing_id"));

        server.send(200, "text/plain", response);
      }
    )
  );

  server.begin();
}

void loop() {
  server.handleClient();
}