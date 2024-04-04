#include "planet.h"
#include "unity.h"
#include <stdio.h>

Planet planets[NB_MAX_PLANETS];
uint16_t nb_planets = 0;

void setUp(void) {
  // Arrange
  create_planet(352, 10000, 11200, 3, 0, planets, &nb_planets);
  create_planet(426, 9000, 11000, -1, 0, planets, &nb_planets);
  create_planet(500, 8000, 10000, 6, 1, planets, &nb_planets);
}

void tearDown(void) {
  // clean planets
  delete_all_planets(planets, &nb_planets);
}

void test_create_planet(void) {
  // Arrange
  uint16_t planet_id = 352;
  uint16_t x = 10000;
  uint16_t y = 11200;
  int8_t ship_id = 3;
  uint8_t saved = 1;
  // Act
  create_planet(planet_id, x, y, ship_id, saved, planets, &nb_planets);
  // Assert
  TEST_ASSERT_EQUAL(4, nb_planets);
  TEST_ASSERT_EQUAL(planet_id, planets[nb_planets - 1].planet_id);
  TEST_ASSERT_EQUAL(x, planets[nb_planets - 1].x);
  TEST_ASSERT_EQUAL(y, planets[nb_planets - 1].y);
  TEST_ASSERT_EQUAL(ship_id, planets[nb_planets - 1].ship_id);
  TEST_ASSERT_EQUAL(saved, planets[nb_planets - 1].saved);
}

void test_get_planet_null(void) {
  // Act
  Planet *planet = get_planet(638, planets, nb_planets);
  // Assert
  TEST_ASSERT_NULL(planet);
}

void test_get_planet(void) {
  // Act
  Planet *planet = get_planet(426, planets, nb_planets);
  // Assert
  TEST_ASSERT_EQUAL(3, nb_planets);
  TEST_ASSERT_NOT_NULL(planet);
  TEST_ASSERT_EQUAL(426, planet->planet_id);
  TEST_ASSERT_EQUAL(9000, planet->x);
  TEST_ASSERT_EQUAL(11000, planet->y);
  TEST_ASSERT_EQUAL(-1, planet->ship_id);
  TEST_ASSERT_EQUAL(0, planet->saved);
}

void test_set_bad_planet(void) {
  // Act
  set_planet(500, 8000, 10000, 6, 1, planets, nb_planets);
  // Assert
  TEST_ASSERT_EQUAL(3, nb_planets);
  TEST_ASSERT_EQUAL(352, planets[0].planet_id);
  TEST_ASSERT_EQUAL(10000, planets[0].x);
  TEST_ASSERT_EQUAL(11200, planets[0].y);
  TEST_ASSERT_EQUAL(3, planets[0].ship_id);
  TEST_ASSERT_EQUAL(0, planets[0].saved);
  TEST_ASSERT_EQUAL(426, planets[1].planet_id);
  TEST_ASSERT_EQUAL(9000, planets[1].x);
  TEST_ASSERT_EQUAL(11000, planets[1].y);
  TEST_ASSERT_EQUAL(-1, planets[1].ship_id);
  TEST_ASSERT_EQUAL(0, planets[1].saved);
}

void test_set_planet(void) {
  // Act
  set_planet(426, 9500, 12000, 2, 1, planets, nb_planets);
  // Assert
  TEST_ASSERT_EQUAL(3, nb_planets);
  TEST_ASSERT_EQUAL(426, planets[1].planet_id);
  TEST_ASSERT_EQUAL(9500, planets[1].x);
  TEST_ASSERT_EQUAL(12000, planets[1].y);
  TEST_ASSERT_EQUAL(2, planets[1].ship_id);
  TEST_ASSERT_EQUAL(1, planets[1].saved);
}

void test_delete_planet(void) {
  // Act
  delete_planet(352, planets, &nb_planets);
  // Assert
  TEST_ASSERT_EQUAL(2, nb_planets);
  TEST_ASSERT_EQUAL(426, planets[0].planet_id);
  TEST_ASSERT_EQUAL(9000, planets[0].x);
  TEST_ASSERT_EQUAL(11000, planets[0].y);
  TEST_ASSERT_EQUAL(-1, planets[0].ship_id);
  TEST_ASSERT_EQUAL(0, planets[0].saved);
}

void test_delete_all_planets(void) {
  // Act
  delete_all_planets(planets, &nb_planets);
  // Assert
  TEST_ASSERT_EQUAL(0, nb_planets);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_create_planet);
  RUN_TEST(test_get_planet_null);
  RUN_TEST(test_get_planet);
  RUN_TEST(test_set_bad_planet);
  RUN_TEST(test_set_planet);
  RUN_TEST(test_delete_planet);
  RUN_TEST(test_delete_all_planets);
  return UNITY_END();
}
