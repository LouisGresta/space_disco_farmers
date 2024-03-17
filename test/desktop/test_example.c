#include <unity.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_example(void) { TEST_ASSERT_EQUAL(4, 2 + 2); }

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_example);
  return UNITY_END();
}
