#include <PathVariableHandler.h>

#ifdef ESP8266

PathVariableHandler::PathVariableHandler(
    const char* pattern,
    const HTTPMethod method,
    const PathVariableHandler::TPathVariableHandlerFn fn)
  : _pattern(new char[strlen(pattern) + 1]),
    patternTokens(NULL),
    method(method),
    fn(fn)
{
  strcpy(_pattern, pattern);
  this->patternTokens = new TokenIterator(_pattern, strlen(pattern), '/'));
}

PathVariableHandler::~PathVariableHandler() {
  delete _pattern;
  delete patternTokens;
}

bool PathVariableHandler::canHandle(HTTPMethod requestMethod, String requestUri) {
  if (this->method != HTTP_ANY && requestMethod != this->method) {
    return false;
  }

  bool canHandle = true;

  char requestUriCopy[requestUri.length() + 1];
  strcpy(requestUriCopy, requestUri.c_str());
  TokenIterator requestTokens(requestUriCopy, requestUri.length(), '/');

  patternTokens->reset();
  while (patternTokens->hasNext() && requestTokens.hasNext()) {
    const char* patternToken = patternTokens->nextToken();
    const char* requestToken = requestTokens.nextToken();

    if (patternToken[0] != ':' && strcmp(patternToken, requestToken) != 0) {
      canHandle = false;
      break;
    }

    if (patternTokens->hasNext() != requestTokens.hasNext()) {
      canHandle = false;
      break;
    }
  }

  return canHandle;
}

bool PathVariableHandler::handle(ESP8266WebServer& server, HTTPMethod requestMethod, String requestUri) {
  if (! canHandle(requestMethod, requestUri)) {
    return false;
  }

  char requestUriCopy[requestUri.length()];
  strcpy(requestUriCopy, requestUri.c_str());
  TokenIterator requestTokens(requestUriCopy, requestUri.length(), '/');

  UrlTokenBindings bindings(*patternTokens, requestTokens);
  fn(&bindings);

  return true;
}

#elif ESP32

PathVariableHandler::PathVariableHandler(
    const char* pattern,
    const WebRequestMethod method,
    PathVariableHandler::TPathVariableHandlerFn fn,
    PathVariableHandler::TPathVariableHandlerBodyFn bodyFn
) : method(method),
    _pattern(new char[strlen(pattern) + 1]),
    patternTokens(NULL)
{
  strcpy(_pattern, pattern);
  this->_fn = fn;
  this->_bodyFn = bodyFn;
  this->patternTokens = new TokenIterator(_pattern, strlen(pattern), '/'));
}

PathVariableHandler::~PathVariableHandler() {
  delete _pattern;
  delete patternTokens;
}

bool PathVariableHandler::canHandle(AsyncWebServerRequest* request) {
  if (this->method != HTTP_ANY && request->method() != this->method) {
    return false;
  }

  bool canHandle = true;
  const String& requestUri = request->url();
  char requestUriCopy[requestUri.length() + 1];
  strcpy(requestUriCopy, requestUri.c_str());
  TokenIterator requestTokens(requestUriCopy, requestUri.length(), '/');

  patternTokens->reset();
  while (patternTokens->hasNext() && requestTokens.hasNext()) {
    const char* patternToken = patternTokens->nextToken();
    const char* requestToken = requestTokens.nextToken();

    if (patternToken[0] != ':' && strcmp(patternToken, requestToken) != 0) {
      canHandle = false;
      break;
    }

    if (patternTokens->hasNext() != requestTokens.hasNext()) {
      canHandle = false;
      break;
    }
  }

  return canHandle;
}

void PathVariableHandler::handleRequest(AsyncWebServerRequest *request) {
  if (_fn) {
    const String& requestUri = request->url();
    char requestUriCopy[requestUri.length()];
    strcpy(requestUriCopy, requestUri.c_str());
    TokenIterator requestTokens(requestUriCopy, requestUri.length(), '/');
    UrlTokenBindings bindings(*patternTokens, requestTokens);
    _fn(&bindings, request);
  }
}

void PathVariableHandler::handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  if (_bodyFn) {
    const String& requestUri = request->url();
    char requestUriCopy[requestUri.length()];
    strcpy(requestUriCopy, requestUri.c_str());
    TokenIterator requestTokens(requestUriCopy, requestUri.length(), '/');
    UrlTokenBindings bindings(*patternTokens, requestTokens);
    _bodyFn(&bindings, request, data, len, index, total);
  }
}

#endif