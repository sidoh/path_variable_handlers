#ifndef _PATH_VARIABLE_HANDLER_H
#define _PATH_VARIABLE_HANDLER_H

#include <functional>
#include <TokenIterator.h>
#include <UrlTokenBindings.h>
#include <string.h>

#if defined(PVH_USE_ASYNC_WEBSERVER)
#define PVH_ASYNC_WEBSERVER
#elif defined(ESP32)
#define PVH_ESP32
#define PVH_BUILTIN_WEBSERVER
#elif defined(ESP8266)
#define PVH_ESP8266
#define PVH_BUILTIN_WEBSERVER
#endif

#ifdef PVH_ASYNC_WEBSERVER
#include <ESPAsyncWebServer.h>
typedef AsyncWebServer TServerType;
#elif defined(PVH_ESP8266)
#include <ESP8266WebServer.h>
typedef ESP8266WebServer TServerType;
#elif defined(PVH_ESP32)
#include <WebServer.h>
typedef WebServer TServerType;
#endif

#if defined(PVH_BUILTIN_WEBSERVER)
class PathVariableHandler : public RequestHandler {
public:
  typedef std::function<void(UrlTokenBindings*)> TPathVariableHandlerFn;

  PathVariableHandler(
    const char* pattern,
    const HTTPMethod method,
    const TPathVariableHandlerFn fn);

  bool canHandle(HTTPMethod requestMethod, String requestUri) override;
  bool handle(TServerType& server, HTTPMethod requesetMethod, String requestUri) override;
#elif defined(PVH_ASYNC_WEBSERVER)
class PathVariableHandler : public AsyncWebHandler {
public:

  typedef std::function<void(AsyncWebServerRequest* request, const UrlTokenBindings*)> TPathVariableHandlerFn;
  typedef std::function<void(
    AsyncWebServerRequest*,
    uint8_t* data,
    size_t len,
    size_t index,
    size_t total,
    const UrlTokenBindings*
  )> TPathVariableHandlerBodyFn;
  typedef std::function<void(
    AsyncWebServerRequest *request,
    const String& filename,
    size_t index,
    uint8_t *data,
    size_t len,
    bool final,
    const UrlTokenBindings*
  )> TPathVariableHandlerUploadFn;


  PathVariableHandler(const char* pattern,
    const WebRequestMethod method,
    TPathVariableHandlerFn fn = NULL,
    TPathVariableHandlerBodyFn bodyFn = NULL,
    TPathVariableHandlerUploadFn uploadFn = NULL);
  PathVariableHandler(const char* pattern,
    const WebRequestMethod method,
    TPathVariableHandlerUploadFn uploadFn);
  PathVariableHandler(const char* pattern,
    const WebRequestMethod method,
    TPathVariableHandlerFn fn,
    TPathVariableHandlerUploadFn uploadFn);

  virtual bool isRequestHandlerTrivial() { return false; }

  virtual bool canHandle(AsyncWebServerRequest* request);
  virtual void handleRequest(AsyncWebServerRequest *request);
  virtual void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
  virtual void handleUpload(
    AsyncWebServerRequest *request,
    const String& filename,
    size_t index,
    uint8_t *data,
    size_t len,
    bool isFinal
  );
#else
class PathVariableHandler {
public:
  PathVariableHandler(const char* pattern);
#endif

  ~PathVariableHandler();

  bool canHandlePath(const char* requestPath, const size_t length);
  inline bool canHandlePath(const char* requestPath) {
    return canHandlePath(requestPath, strlen(requestPath));
  }

private:
  char* _pattern;
  TokenIterator* patternTokens;
#if defined(PVH_BUILTIN_WEBSERVER)
private:
  const HTTPMethod method;
  const PathVariableHandler::TPathVariableHandlerFn fn;
#elif defined(PVH_ASYNC_WEBSERVER)
  const WebRequestMethod method;
  PathVariableHandler::TPathVariableHandlerFn _fn;
  PathVariableHandler::TPathVariableHandlerBodyFn _bodyFn;
  PathVariableHandler::TPathVariableHandlerUploadFn _uploadFn;
#endif
};

#endif