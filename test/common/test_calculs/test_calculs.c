#include "functionCalculs.h"
#include <unity.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_get_travel_angle(void) {
  TEST_ASSERT_EQUAL(90, get_travel_angle(15, 10, 15, 5));
  TEST_ASSERT_EQUAL(315, get_travel_angle(25, 13, 27, 15));
  TEST_ASSERT_EQUAL(135, get_travel_angle(100, 80, 80, 60));
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_get_travel_angle);
  return UNITY_END();
}
