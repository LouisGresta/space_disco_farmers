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
void test_determine_target_spaceship(void) {
  Spaceship base_defender = {.ship_id = 8, .x = 10, .y = 10};
  Spaceship ennemies[] = {{.ship_id = 9, .x = 20, .y = 20},
                          {.ship_id = 2, .x = 50, .y = 100}};

  uint8_t nb_spaceships = 2;

  Spaceship *ennemy_ship =
      determine_target_spaceship(base_defender, ennemies, nb_spaceships);

  TEST_ASSERT_EQUAL(&ennemies[0], ennemy_ship);
}

void test_get_angle_from_middle(void) {
  TEST_ASSERT_EQUAL(270, get_angle_from_middle(10000, 0));
  TEST_ASSERT_EQUAL(180, get_angle_from_middle(0, 10000));
  TEST_ASSERT_EQUAL(90, get_angle_from_middle(10000, 20000));
  TEST_ASSERT_EQUAL(0, get_angle_from_middle(20000, 10000));
}

void test_determine_next_circle_point(void) {
  uint16_t results[2];
  uint16_t angle_result;
  angle_result = determine_next_circle_point(results, 135, 0);

  TEST_ASSERT_EQUAL(90, angle_result);
  TEST_ASSERT_EQUAL(20000, results[1]);
  TEST_ASSERT_EQUAL(10000, results[0]);

  angle_result = determine_next_circle_point(results, 135, 1);

  TEST_ASSERT_EQUAL(180, angle_result);
  TEST_ASSERT_EQUAL(10000, results[1]);
  TEST_ASSERT_EQUAL(0, results[0]);

  angle_result = determine_next_circle_point(results, 315, 1);

  TEST_ASSERT_EQUAL(0, angle_result);
  TEST_ASSERT_EQUAL(10000, results[1]);
  TEST_ASSERT_EQUAL(20000, results[0]);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_get_travel_angle);
  RUN_TEST(test_determine_target_planets);
  RUN_TEST(test_get_angle_from_middle);
  RUN_TEST(test_determine_next_circle_point);
  RUN_TEST(test_determine_target_spaceship);
  return UNITY_END();
}
