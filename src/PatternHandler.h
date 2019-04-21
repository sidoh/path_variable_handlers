#ifndef _PATTERNHANDLER_H
#define _PATTERNHANDLER_H

#include <functional>
#include <TokenIterator.h>
#include <UrlTokenBindings.h>

#ifdef ESP8266

#include <ESP8266WebServer.h>

class PatternHandler : public RequestHandler {
public:
  typedef std::function<void(UrlTokenBindings*)> TPatternHandlerFn;

  PatternHandler(const String& pattern,
    const HTTPMethod method,
    const TPatternHandlerFn fn);

  ~PatternHandler();

  bool canHandle(HTTPMethod requestMethod, String requestUri) override;
  bool handle(ESP8266WebServer& server, HTTPMethod requesetMethod, String requestUri) override;

private:
  char* _pattern;
  TokenIterator* patternTokens;
  const HTTPMethod method;
  const PatternHandler::TPatternHandlerFn fn;
};

#elif ESP32

class PatternHandler : public AsyncWebHandler {
public:
  typedef std::function<void(const UrlTokenBindings*, AsyncWebServerRequest* request)> TPatternHandlerFn;
  typedef std::function<void(
    const UrlTokenBindings*,
    AsyncWebServerRequest*,
    uint8_t* data,
    size_t len,
    size_t index,
    size_t total
  )> TPatternHandlerBodyFn;

  PatternHandler(const char* pattern,
    const WebRequestMethod method,
    TPatternHandlerFn fn = NULL,
    TPatternHandlerBodyFn bodyFn = NULL);

  ~PatternHandler();

  virtual bool isRequestHandlerTrivial() { return false; }

  virtual bool canHandle(AsyncWebServerRequest* request);
  virtual void handleRequest(AsyncWebServerRequest *request);
  virtual void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

private:
  char* _pattern;
  TokenIterator* patternTokens;
  const WebRequestMethod method;
  PatternHandler::TPatternHandlerFn _fn;
  PatternHandler::TPatternHandlerBodyFn _bodyFn;
};

#endif // Platform
#endif // Header
