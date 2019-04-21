# Path Variable Handlers [![Build Status](https://travis-ci.org/sidoh/path_variable_handlers.svg?branch=master)](https://travis-ci.org/sidoh/path_variable_handlers)

This is a library for handling HTTP and REST paths containing variables. Many RESTful APIs contain resources that have variables in their paths (e.g., `/things/:thing_id`).  This library exposes a way to easily process such resource paths in a low-effort way.

Compatible with `ESP8266WebServer` for ESP8266 and ESPAsyncWebServer for `ESP32`.

### Examples

Examples are found in the `./examples` directory.  This is the easiest way to get started.

## Compatible Hardware

This library is built on top of handler bindings tied to the espressif platform:

* ESP32: uses [`ESPAsyncWebServer`](https://github.com/me-no-dev/ESPAsyncWebServer) bindings.
* ESP266: uses the `ESP8266WebServer` bindings built into the SDK by default.  Use `ESPAsyncWebServer` by defining `PVH_USE_ASYNC_WEBSERVER`.

## Development

Run tests with:

```
platformio test
```

Build examples with, for instance:

```
platformio ci --board=d1_mini --lib=. examples/esp8266_simple
```