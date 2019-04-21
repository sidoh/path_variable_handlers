# Path Variable Handlers

This is a library for handling HTTP and REST paths containing variables. Many RESTful APIs contain resources that have variables in their paths (e.g., /things/:thing_id).  This library exposes a way to easily process such resource paths in a low-effort way.

Compatible with ESP8266WebServer for ESP8266 and ESPAsyncWebServer for ESP32.

## Examples

Examples are found in the `./examples` directory.  This is the easiest way to get started.

## Compatible Hardware

This library is built on top of handler bindings tied to the espressif platform:

* ESP266: uses the `ESP8266WebServer` bindings built into the SDK.
* ESP32: uses [`ESPAsyncWebServer`](https://github.com/me-no-dev/ESPAsyncWebServer) bindings.