#include "commands.h"
#include "planet.h"
#include "spaceship.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unity.h>

void build_planet_response(char *result, uint16_t planet_id, uint16_t x,
                           uint16_t y, uint16_t ship_id, uint8_t saved);
void build_spaceship_response(char *result, uint8_t team_id, uint8_t ship_id,
                              uint16_t x, uint16_t y, uint8_t broken);
void build_base_response(char *result, uint16_t x, uint16_t y);
void build_radar_response(char *result);

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_move_command(void) {
  TEST_ASSERT_EQUAL_STRING("MOVE 1 90 1000\n", move_str(1, 90, 1000));
  TEST_ASSERT_EQUAL_STRING("MOVE 6 180 2000\n", move_str(6, 180, 2000));
  TEST_ASSERT_EQUAL_STRING("MOVE 8 270 3000\n", move_str(8, 270, 3000));
}

void test_fire_command(void) {
  TEST_ASSERT_EQUAL_STRING("FIRE 1 90\n", fire_str(1, 90));
  TEST_ASSERT_EQUAL_STRING("FIRE 3 180\n", fire_str(3, 180));
  TEST_ASSERT_EQUAL_STRING("FIRE 5 270\n", fire_str(5, 270));
}

void test_radar_command(void) {
  TEST_ASSERT_EQUAL_STRING("RADAR 6\n", radar_str(6));
  TEST_ASSERT_EQUAL_STRING("RADAR 7\n", radar_str(7));
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

void test_parse_radar_response(void) {
  // Arrange
  Planet planets[NB_MAX_PLANETS];
  uint16_t nb_planets = 0;
  Spaceship spaceships[NB_MAX_SPACESHIPS];
  uint16_t nb_spaceships = 0;
  uint16_t x_base = 0;
  uint16_t y_base = 0;
  char response[200];
  build_radar_response(response);
  // Act
  parse_radar_response(response, planets, &nb_planets, spaceships,
                       &nb_spaceships, &x_base, &y_base);
  // Assert
  TEST_ASSERT_EQUAL(5, nb_planets);
  TEST_ASSERT_EQUAL(5, nb_spaceships);
  TEST_ASSERT_EQUAL(10000, x_base);
  TEST_ASSERT_EQUAL(2000, y_base);
  for (uint16_t i = 0; i < 5; i++) {
    TEST_ASSERT_EQUAL(i, planets[i].planet_id);
    TEST_ASSERT_EQUAL(i + 1, planets[i].x);
    TEST_ASSERT_EQUAL(i + 2, planets[i].y);
    TEST_ASSERT_EQUAL(i + 3, planets[i].ship_id);
    TEST_ASSERT_EQUAL(i % 2, planets[i].saved);

    TEST_ASSERT_EQUAL(i, spaceships[i].team_id);
    TEST_ASSERT_EQUAL(i + 1, spaceships[i].ship_id);
    TEST_ASSERT_EQUAL(i + 2, spaceships[i].x);
    TEST_ASSERT_EQUAL(i % 2, spaceships[i].broken);
  }
}

void build_radar_response(char *result) {
  char planet[17];
  char ship[17];
  char base[8];
  result[0] = '\0';
  for (uint16_t i = 0; i < 5; i++) {
    build_planet_response(planet, i, i + 1, i + 2, i + 3, i % 2);
    strcat(result, planet);
    strcat(result, ",");
    build_spaceship_response(ship, i, i + 1, i + 2, i + 3, i % 2);
    strcat(result, ship);
    strcat(result, ",");
  }
  build_base_response(base, 10000, 2000);
  strcat(result, base);
  strcat(result, "\n");
}

void build_planet_response(char *result, uint16_t planet_id, uint16_t x,
                           uint16_t y, uint16_t ship_id, uint8_t saved) {
  sprintf(result, "P %d %d %d %d %d", planet_id, x, y, ship_id, saved);
}

void build_spaceship_response(char *result, uint8_t team_id, uint8_t ship_id,
                              uint16_t x, uint16_t y, uint8_t broken) {
  sprintf(result, "S %d %d %d %d %d", team_id, ship_id, x, y, broken);
}

void build_base_response(char *result, uint16_t x, uint16_t y) {
  sprintf(result, "B %d %d", x, y);
}

int main(void) {
  UNITY_BEGIN();
  RUN_TEST(test_move_command);
  RUN_TEST(test_fire_command);
  RUN_TEST(test_radar_command);
  RUN_TEST(test_split);
  RUN_TEST(test_parse_radar_response);
  return UNITY_END();
}
