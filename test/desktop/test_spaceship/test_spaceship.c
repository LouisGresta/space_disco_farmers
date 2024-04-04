#include "spaceship.h"
#include "unity.h"
#include <stdlib.h>

Spaceship spaceships[NB_MAX_SPACESHIPS];
uint16_t nb_spaceships = 0;

void setUp(void) {
  // Arrange
  create_spaceship(1, 3, 10000, 11200, 1, spaceships, &nb_spaceships);
  create_spaceship(2, 4, 9000, 11000, 0, spaceships, &nb_spaceships);
  create_spaceship(3, 5, 8000, 10000, 1, spaceships, &nb_spaceships);
}

void tearDown(void) {
  // clean spaceships allocation
  delete_all_spaceships(spaceships, &nb_spaceships);
}

void test_create_spaceship(void) {
  // Arrange
  uint8_t team_id = 1;
  int8_t ship_id = 3;
  uint16_t x = 10000;
  uint16_t y = 11200;
  uint8_t broken = 1;
  // Act
  create_spaceship(team_id, ship_id, x, y, broken, spaceships, &nb_spaceships);
  // Assert
  TEST_ASSERT_EQUAL(4, nb_spaceships);
  TEST_ASSERT_EQUAL(team_id, spaceships[nb_spaceships - 1].team_id);
  TEST_ASSERT_EQUAL(ship_id, spaceships[nb_spaceships - 1].ship_id);
  TEST_ASSERT_EQUAL(x, spaceships[nb_spaceships - 1].x);
  TEST_ASSERT_EQUAL(y, spaceships[nb_spaceships - 1].y);
  TEST_ASSERT_EQUAL(broken, spaceships[nb_spaceships - 1].broken);
}

void test_get_spaceship_null(void) {
  // Act
  Spaceship *spaceship = get_spaceship(5, 1, spaceships, nb_spaceships);
  // Assert
  TEST_ASSERT_NULL(spaceship);
}

void test_get_spaceship(void) {
  // Act
  Spaceship *spaceship = get_spaceship(2, 4, spaceships, nb_spaceships);
  // Assert
  TEST_ASSERT_EQUAL(3, nb_spaceships);
  TEST_ASSERT_NOT_NULL(spaceship);
  TEST_ASSERT_EQUAL(2, spaceship->team_id);
  TEST_ASSERT_EQUAL(4, spaceship->ship_id);
  TEST_ASSERT_EQUAL(9000, spaceship->x);
  TEST_ASSERT_EQUAL(11000, spaceship->y);
  TEST_ASSERT_EQUAL(0, spaceship->broken);
}

void test_set_bad_spaceship(void) {
  // Act
  set_spaceship(2, 5, 8000, 10000, 1, spaceships, nb_spaceships);
  // Assert
  Spaceship *spaceship = get_spaceship(2, 4, spaceships, nb_spaceships);
  TEST_ASSERT_NOT_NULL(spaceship);
  TEST_ASSERT_EQUAL(9000, spaceship->x);
  TEST_ASSERT_EQUAL(11000, spaceship->y);
  TEST_ASSERT_EQUAL(0, spaceship->broken);
}

void test_set_spaceship(void) {
  // Act
  set_spaceship(2, 4, 8000, 10000, 1, spaceships, nb_spaceships);
  // Assert
  Spaceship *spaceship = get_spaceship(2, 4, spaceships, nb_spaceships);
  TEST_ASSERT_NOT_NULL(spaceship);
  TEST_ASSERT_EQUAL(8000, spaceship->x);
  TEST_ASSERT_EQUAL(10000, spaceship->y);
  TEST_ASSERT_EQUAL(1, spaceship->broken);
}

void test_delete_spaceship(void) {
  // Act
  delete_spaceship(2, 4, spaceships, &nb_spaceships);
  // Assert
  TEST_ASSERT_EQUAL(2, nb_spaceships);
  TEST_ASSERT_EQUAL(1, spaceships[0].team_id);
  TEST_ASSERT_EQUAL(3, spaceships[0].ship_id);
  TEST_ASSERT_EQUAL(10000, spaceships[0].x);
  TEST_ASSERT_EQUAL(11200, spaceships[0].y);
  TEST_ASSERT_EQUAL(1, spaceships[0].broken);
  TEST_ASSERT_EQUAL(3, spaceships[1].team_id);
  TEST_ASSERT_EQUAL(5, spaceships[1].ship_id);
  TEST_ASSERT_EQUAL(8000, spaceships[1].x);
  TEST_ASSERT_EQUAL(10000, spaceships[1].y);
  TEST_ASSERT_EQUAL(1, spaceships[1].broken);
}

void test_delete_all_spaceships(void) {
  // Act
  delete_all_spaceships(spaceships, &nb_spaceships);
  // Assert
  TEST_ASSERT_EQUAL(0, nb_spaceships);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_create_spaceship);
  RUN_TEST(test_get_spaceship_null);
  RUN_TEST(test_get_spaceship);
  RUN_TEST(test_set_bad_spaceship);
  RUN_TEST(test_set_spaceship);
  RUN_TEST(test_delete_spaceship);
  RUN_TEST(test_delete_all_spaceships);
  return UNITY_END();
}
