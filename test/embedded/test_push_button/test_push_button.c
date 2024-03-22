#include "main.h"
#include "unity.h"

void setUp(void) { push_button_init(); }

void tearDown(void) {
  // clean stuff up here
}

void test_push_button_not_pressed(void) {
  TEST_ASSERT_EQUAL(0, push_button_is_pressed());
}

void test_push_button_pressed(void) {
  // simulate button press
  // set the button pin to 1
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
  TEST_ASSERT_EQUAL(1, push_button_is_pressed());
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_push_button_not_pressed);
  RUN_TEST(test_push_button_pressed);
  return UNITY_END();
}
