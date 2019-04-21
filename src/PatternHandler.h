#ifndef _PATTERNHANDLER_H
#define _PATTERNHANDLER_H

#include <functional>

#ifdef ESP8266

#include <ESP8266WebServer.h>
#include <TokenIterator.h>
#include <UrlTokenBindings.h>

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

#endif // Platform
#endif // Header
