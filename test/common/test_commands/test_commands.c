#include "commands.h"
#include <stdlib.h>
#include <unity.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_move_command(void) {
  TEST_ASSERT_EQUAL_STRING("MOVE 1 90 1000\n", move_command(1, 90, 1000));
  TEST_ASSERT_EQUAL_STRING("MOVE 6 180 2000\n", move_command(6, 180, 2000));
  TEST_ASSERT_EQUAL_STRING("MOVE 8 270 3000\n", move_command(8, 270, 3000));
}

void test_fire_command(void) {
  TEST_ASSERT_EQUAL_STRING("FIRE 1 90\n", fire_command(1, 90));
  TEST_ASSERT_EQUAL_STRING("FIRE 3 180\n", fire_command(3, 180));
  TEST_ASSERT_EQUAL_STRING("FIRE 5 270\n", fire_command(5, 270));
}

void test_radar_command(void) {
  TEST_ASSERT_EQUAL_STRING("RADAR 6\n", radar_command(6));
  TEST_ASSERT_EQUAL_STRING("RADAR 7\n", radar_command(7));
}

void test_split(void) {
  uint16_t count;
  char **tokens = split("Theo,Louis,Pachelle Carelle", ',', &count);
  TEST_ASSERT_EQUAL(3, count);
  TEST_ASSERT_EQUAL_STRING("Theo", tokens[0]);
  TEST_ASSERT_EQUAL_STRING("Louis", tokens[1]);
  TEST_ASSERT_EQUAL_STRING("Pachelle Carelle", tokens[2]);
  free(tokens);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_move_command);
  RUN_TEST(test_fire_command);
  RUN_TEST(test_radar_command);
  RUN_TEST(test_split);
  return UNITY_END();
}
