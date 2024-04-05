#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "gameConstants.h"

static char buffer[MAX_COMMAND_SIZE];

char *move_str(int8_t ship_id, uint16_t angle, uint16_t speed) {
  sprintf(buffer, "MOVE %d %d %d\n", ship_id, angle, speed);
  return buffer;
}

char *fire_str(int8_t ship_id, uint16_t angle) {
  sprintf(buffer, "FIRE %d %d\n", ship_id, angle);
  return buffer;
}

char *radar_str(int8_t ship_id) {
  sprintf(buffer, "RADAR %d\n", ship_id);
  return buffer;
}

void split(char **tokens, const char *str, const char delimiter,
           uint16_t *count) {
  uint16_t i = 0;
  *count = 0;
  uint16_t pos = 0;
  char token[MAX_COMMAND_SIZE];
  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] == delimiter) {
      token[pos] = '\0';
      tokens[*count] = strdup(token);
      (*count)++;
      pos = 0;
      token[0] = '\0';
    } else {
      token[pos] = str[i];
      pos++;
    }
  }
  token[pos] = '\0';
  tokens[*count] = strdup(token);
  (*count)++;
}

void process_planet(char **params, Planet *planets, uint16_t *nb_planets) {
  uint16_t planet_id = atoi(params[1]);
  uint16_t x = atoi(params[2]);
  uint16_t y = atoi(params[3]);
  int8_t ship_id = atoi(params[4]);
  uint8_t saved = atoi(params[5]);
  Planet *planet = get_planet(planet_id, planets, *nb_planets);
  if (planet == NULL) {
    // If the planet does not exist, create it
    create_planet(planet_id, x, y, ship_id, saved, planets, nb_planets);
    return;
  }
  planet->x = x;
  planet->y = y;
  planet->ship_id = ship_id;
  planet->saved = saved;
}

void process_spaceship(char **params, Spaceship *spaceships,
                       uint16_t *nb_spaceships) {
  uint8_t team_id = atoi(params[1]);
  int8_t ship_id = atoi(params[2]);
  uint16_t x = atoi(params[3]);
  uint16_t y = atoi(params[4]);
  uint8_t broken = atoi(params[5]);
  Spaceship *spaceship =
      get_spaceship(team_id, ship_id, spaceships, *nb_spaceships);
  if (spaceship == NULL) {
    // If the spaceship does not exist, create it
    create_spaceship(team_id, ship_id, x, y, broken, spaceships, nb_spaceships);
    return;
  }
  spaceship->x = x;
  spaceship->y = y;
  spaceship->broken = broken;
}

// Parse responses from the server
void parse_radar_response(const char *response, Planet *planets,
                          uint16_t *nb_planets, Spaceship *spaceships,
                          uint16_t *nb_spaceships, uint16_t *x_base,
                          uint16_t *y_base) {
  uint16_t count;
  char *str_scan[MAX_SPLIT_COUNT];
  split(str_scan, response, ',', &count);
  for (uint16_t i = 0; i < count; i++) {
    uint16_t nb_params;
    char *params[MAX_SPLIT_COUNT];
    split(params, str_scan[i], ' ', &nb_params);
    if (params[0][0] == PLANET) {
      process_planet(params, planets, nb_planets);
    } else if (params[0][0] == SPACESHIP) {
      process_spaceship(params, spaceships, nb_spaceships);
    } else if (params[0][0] == BASE) {
      *x_base = atoi(params[1]);
      *y_base = atoi(params[2]);
    }
  }
}
