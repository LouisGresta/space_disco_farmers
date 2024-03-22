#include "functionCalculs.h"
#include <unity.h>

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_get_travel_angle(void) {
  TEST_ASSERT_EQUAL(45, get_travel_angle(10, 10, 20, 20));
  TEST_ASSERT_EQUAL(225, get_travel_angle(50, 50, 40, 40));
  TEST_ASSERT_EQUAL(90, get_travel_angle(100, 100, 100, 160));
}

void test_determine_target_planets(void) {
  // cas basique
  Spaceship collector1 = {.ship_id = 8, .x = 10, .y = 10};
  Spaceship collector2 = {.ship_id = 9, .x = 20, .y = 20};

  Planet planets[3] = {{.planet_id = 1, .x = 5, .y = 5, .saved = 0},
                       {.planet_id = 2, .x = 16, .y = 16, .saved = 0},
                       {.planet_id = 3, .x = 25, .y = 25, .saved = 0}};
  uint8_t nb_planets = 3;

  uint16_t results[2][2];
  determine_target_planets(collector1, collector2, planets, nb_planets,
                           results);

  TEST_ASSERT_EQUAL_INT(
      8, results[0][0]); // Vérifie que le collecteur 1 a le bon ID
  TEST_ASSERT_EQUAL_INT(
      9, results[1][0]); // Vérifie que le collecteur 2 a le bon ID
  TEST_ASSERT_EQUAL_INT(
      1, results[0][1]); // Vérifie que le collecteur 1 cible la bonne planète
  TEST_ASSERT_EQUAL_INT(
      2, results[1][1]); // Vérifie que le collecteur 2 cible la bonne planète

  // cas avec 1 planète saved
  planets[0].saved = 1;

  determine_target_planets(collector1, collector2, planets, nb_planets,
                           results);
  TEST_ASSERT_EQUAL_INT(
      8, results[0][0]); // Vérifie que le collecteur 1 a le bon ID
  TEST_ASSERT_EQUAL_INT(
      9, results[1][0]); // Vérifie que le collecteur 2 a le bon ID
  TEST_ASSERT_EQUAL_INT(
      2, results[0][1]); // Vérifie que le collecteur 1 cible la bonne planète
  TEST_ASSERT_EQUAL_INT(
      3, results[1][1]); // Vérifie que le collecteur 2 cible la bonne planète
}

void test_get_base_angle(void) {
  TEST_ASSERT_EQUAL(270, get_base_angle(10000, 0));
  TEST_ASSERT_EQUAL(180, get_base_angle(0, 10000));
  TEST_ASSERT_EQUAL(90, get_base_angle(10000, 20000));
  TEST_ASSERT_EQUAL(0, get_base_angle(20000, 10000));
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_get_travel_angle);
  RUN_TEST(test_determine_target_planets);
  RUN_TEST(test_get_base_angle);
  return UNITY_END();
}
