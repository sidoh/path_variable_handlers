#ifndef _PATH_VARIABLE_HANDLER_H
#define _PATH_VARIABLE_HANDLER_H

#include <functional>
#include <TokenIterator.h>
#include <UrlTokenBindings.h>

#ifdef ESP8266

#include <ESP8266WebServer.h>

class PathVariableHandler : public RequestHandler {
public:
  typedef std::function<void(UrlTokenBindings*)> TPathVariableHandlerFn;

  PathVariableHandler(
    const char* pattern,
    const HTTPMethod method,
    const TPathVariableHandlerFn fn);

  ~PathVariableHandler();

  bool canHandle(HTTPMethod requestMethod, String requestUri) override;
  bool handle(ESP8266WebServer& server, HTTPMethod requesetMethod, String requestUri) override;

private:
  char* _pattern;
  TokenIterator patternTokens;
  const HTTPMethod method;
  const PathVariableHandler::TPathVariableHandlerFn fn;
};

#elif ESP32

class PathVariableHandler : public AsyncWebHandler {
public:
  typedef std::function<void(const UrlTokenBindings*, AsyncWebServerRequest* request)> TPathVariableHandlerFn;
  typedef std::function<void(
    const UrlTokenBindings*,
    AsyncWebServerRequest*,
    uint8_t* data,
    size_t len,
    size_t index,
    size_t total
  )> TPathVariableHandlerBodyFn;

  PathVariableHandler(const char* pattern,
    const WebRequestMethod method,
    TPathVariableHandlerFn fn = NULL,
    TPathVariableHandlerBodyFn bodyFn = NULL);

  ~PathVariableHandler();

  virtual bool isRequestHandlerTrivial() { return false; }

  virtual bool canHandle(AsyncWebServerRequest* request);
  virtual void handleRequest(AsyncWebServerRequest *request);
  virtual void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

private:
  char* _pattern;
  TokenIterator patternTokens;
  const WebRequestMethod method;
  PathVariableHandler::TPathVariableHandlerFn _fn;
  PathVariableHandler::TPathVariableHandlerBodyFn _bodyFn;
};

#endif // Platform
#endif // Header
