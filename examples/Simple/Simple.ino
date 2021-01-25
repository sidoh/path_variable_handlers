#include <Arduino.h>

#include <TokenIterator.h>
#include <UrlTokenBindings.h>

void setup() {
  Serial.begin(115200);

  char templatePath[] = "example/path/:var1/:var2/:var3";
  char samplePath[] = "example/path/a/b/c";

  TokenIterator templateItr(templatePath, strlen(templatePath), '/');
  TokenIterator sampleItr(samplePath, strlen(samplePath), '/');
  UrlTokenBindings bindings(templateItr, sampleItr);

  if (bindings.hasBinding("var1")) {      // has this one
    Serial.println(bindings.get("var1")); // will print "a"
  }

  if (bindings.hasBinding("var4")) {
    // does not have this binding
  }
}

void loop() { }
