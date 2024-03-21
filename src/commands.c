#include "gameConstants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "functionCalculs.h"

#define MAX_COMMAND_SIZE 100

static char buffer[MAX_COMMAND_SIZE];
/*
void move_v_max(Spaceship ship, uint16_t angle)
{
  uint16_t speed;
}*/

// print commands
const char *move_str(uint8_t ship_id, uint16_t angle, uint16_t speed) {
  sprintf(buffer, "MOVE %d %d %d\n", ship_id, angle, speed);
  return buffer;
}

const char *fire_str(uint8_t ship_id, uint16_t angle) {
  sprintf(buffer, "FIRE %d %d\n", ship_id, angle);
  return buffer;
}

const char *radar_str(uint8_t ship_id) {
  sprintf(buffer, "RADAR %d\n", ship_id);
  return buffer;
}

char **split(const char *str, const char delimiter, uint16_t *count) {
  char delimiter_str[2];
  delimiter_str[0] = delimiter;
  delimiter_str[1] = '\0';
  uint16_t i = 0;
  *count = 0;
  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] == delimiter) {
      (*count)++;
    }
  }
  (*count)++;

  char *token;
  char **tokens = malloc(sizeof(char *) * *count);
  if (!tokens)
    return NULL;
  i = 0;
  char *tmp = strdup(str);
  token = strtok(tmp, delimiter_str);
  while (token != NULL) {
    tokens[i] = token;
    token = strtok(NULL, delimiter_str);
    i++;
  }
  return tokens;
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
int parse_radar_response(const char *response, Planet *planets,
                         uint16_t *nb_planets, Spaceship *spaceships,
                         uint16_t *nb_spaceships, uint16_t *x_base,
                         uint16_t *y_base) {
  uint16_t count;
  char **str_scan = split(response, ',', &count);
  for (uint16_t i = 0; i < count; i++) {
    uint16_t nb_params;
    char **params = split(str_scan[i], ' ', &nb_params);
    if (params[0][0] == PLANET) {
      process_planet(params, planets, nb_planets);
    } else if (params[0][0] == SPACESHIP) {
      process_spaceship(params, spaceships, nb_spaceships);
    } else if (params[0][0] == BASE) {
      *x_base = atoi(params[1]);
      *y_base = atoi(params[2]);
    }
    free(params);
  }
  free(str_scan);
  return 0;
}
