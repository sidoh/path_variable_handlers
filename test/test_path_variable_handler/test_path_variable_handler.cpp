#include <PathVariableHandler.h>
#include <unity.h>
#include <string.h>

char buffer1[100];

void test_simple(void) {
  strcpy(buffer1, "a/:b/:c");

  PathVariableHandler handler(buffer1);

  TEST_ASSERT_EQUAL(true, handler.canHandlePath("a/123/456"));
  TEST_ASSERT_EQUAL(false, handler.canHandlePath("a/123"));
  TEST_ASSERT_EQUAL(false, handler.canHandlePath("a/123/456/789"));
  TEST_ASSERT_EQUAL(true, handler.canHandlePath("a//b"));
}

void test_mixed(void) {
  strcpy(buffer1, "a/:b/c/:d");

  PathVariableHandler handler(buffer1);

  TEST_ASSERT_EQUAL(false, handler.canHandlePath("a/123/456/d"));
  TEST_ASSERT_EQUAL(true, handler.canHandlePath("a/123/c/456"));
  TEST_ASSERT_EQUAL(false, handler.canHandlePath("a/123/d/c"));
}

void process() {
    UNITY_BEGIN();
    RUN_TEST(test_simple);
    RUN_TEST(test_mixed);
    UNITY_END();
}

int main(int argc, char **argv) {
    process();
    return 0;
}