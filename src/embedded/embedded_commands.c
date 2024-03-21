#include "embedded_commands.h"
#include "commands.h"
#include "gameConstants.h"
#include <string.h>
#include <stdlib.h>

int8_t ATTACKERS[ATTACKERS_SIZE] = { 1, 2, 3, 4, 5 };
int8_t EXPLORERS[EXPLORERS_SIZE] = { 6, 7 };
int8_t COLLECTORS[COLLECTORS_SIZE] = { 8, 9 };

uint8_t move(int8_t ship_id, uint16_t angle, uint16_t speed) {
  char response[5];
  puts(move_str(ship_id, angle, speed));
  gets(response);
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
  puts(fire_str(ship_id, angle));
  gets(response);
  if (strcmp(response, "OK") == 0ULL) {
    return 1;
  }
  return 0;
}

char *radar(int8_t ship_id) {
  puts(radar_str(ship_id));
  char *response = malloc(sizeof(char) * MAX_RESPONSE_SIZE);
  gets(response);
  return response;
}
