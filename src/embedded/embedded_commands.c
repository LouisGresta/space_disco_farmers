#include "embedded_commands.h"
#include "commands.h"
#include "functionCalculs.h"
#include "gameConstants.h"
#include "os_utils.h"
#include <stdlib.h>
#include <string.h>

int8_t ATTACKERS[ATTACKERS_SIZE] = {1, 2, 3, 4, 5};
int8_t EXPLORERS[EXPLORERS_SIZE] = {6, 7};
int8_t COLLECTORS[COLLECTORS_SIZE] = {8, 9};

uint8_t move(int8_t ship_id, uint16_t angle, uint16_t speed) {
  char response[5];
  get_mutex(serial_mutex_id);
  puts(move_str(ship_id, angle, speed));
  gets(response);
  release_mutex(serial_mutex_id);
  if (strcmp(response, "OK") == 0) {
    return 1;
  }
  return 0;
}

uint8_t is_in_array(int8_t ship_id, int8_t *array, uint16_t array_size) {
  for (uint16_t i = 0; i < array_size; i++) {
    if (ship_id == array[i]) {
      return 1;
    }
  }
  return 0;
}

uint8_t move_v_max(int8_t ship_id, uint16_t angle) {
  uint16_t speed;
  if (is_in_array(ship_id, ATTACKERS, ATTACKERS_SIZE)) {
    speed = ATTACKERS_MAX_SPEED;
  } else if (is_in_array(ship_id, COLLECTORS, COLLECTORS_SIZE)) {
    speed = COLLECTORS_MAX_SPEED;
  } else if (is_in_array(ship_id, EXPLORERS, EXPLORERS_SIZE)) {
    speed = EXPLORERS_MAX_SPEED;
  } else {
    // Invalid ship_id
    return 0;
  }
  return move(ship_id, angle, speed);
}

uint8_t fire(int8_t ship_id, uint16_t angle) {
  char response[5];
  get_mutex(serial_mutex_id);
  puts(fire_str(ship_id, angle));
  gets(response);
  release_mutex(serial_mutex_id);
  if (strcmp(response, "OK") == 0) {
    return 1;
  }
  return 0;
}

void radar(char *response, int8_t ship_id) {
  get_mutex(serial_mutex_id);
  puts(radar_str(ship_id));
  gets(response);
  release_mutex(serial_mutex_id);
}

void parse_radar_response_mutex(const char *response, Planet *planets,
                                uint16_t *nb_planets, Spaceship *spaceships,
                                uint16_t *nb_spaceships, uint16_t *x_base,
                                uint16_t *y_base) {
  get_mutex(planets_spaceships_mutex_id);
  parse_radar_response(response, planets, nb_planets, spaceships, nb_spaceships,
                       x_base, y_base);
  release_mutex(planets_spaceships_mutex_id);
}

// fonction à call quand vaisseau détruit ou possède une planète
void retour_base(Spaceship ship) {

  uint16_t angle = get_travel_angle(ship.x, ship.y, base_x, base_y);
  move_v_max(ship.ship_id, angle);
}
