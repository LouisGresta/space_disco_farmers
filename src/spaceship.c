#include "spaceship.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_spaceship(uint8_t team_id, int8_t ship_id, uint16_t x, uint16_t y,
                      uint8_t broken, Spaceship *spaceships,
                      uint16_t *nb_spaceships) {
  Spaceship new_spaceship;
  new_spaceship.team_id = team_id;
  new_spaceship.ship_id = ship_id;
  new_spaceship.x = x;
  new_spaceship.y = y;
  new_spaceship.broken = broken;
  spaceships[*nb_spaceships] = new_spaceship;
  (*nb_spaceships)++;
}

Spaceship *get_spaceship(uint8_t team_id, int8_t ship_id, Spaceship *spaceships,
                         uint16_t nb_spaceships) {
  for (uint16_t i = 0; i < nb_spaceships; i++) {
    if (spaceships[i].team_id == team_id && spaceships[i].ship_id == ship_id) {
      return &spaceships[i];
    }
  }
  return NULL;
}

void set_spaceship(uint8_t team_id, int8_t ship_id, uint16_t x, uint16_t y,
                   uint8_t broken, Spaceship *spaceships,
                   uint16_t nb_spaceships) {
  Spaceship *spaceship =
      get_spaceship(team_id, ship_id, spaceships, nb_spaceships);
  if (spaceship == NULL)
    return;
  spaceship->x = x;
  spaceship->y = y;
  spaceship->broken = broken;
}

void delete_spaceship(uint8_t team_id, int8_t ship_id, Spaceship *spaceships,
                      uint16_t *nb_spaceships) {
  for (uint16_t i = 0; i < *nb_spaceships; i++) {
    if (spaceships[i].team_id == team_id && spaceships[i].ship_id == ship_id) {
      spaceships[i].team_id = 0;
      spaceships[i].ship_id = 0;
      spaceships[i].x = 0;
      spaceships[i].y = 0;
      spaceships[i].broken = 0;
      (*nb_spaceships)--;
      break;
    }
  }
}

void delete_all_spaceships(Spaceship *spaceships, uint16_t *nb_spaceships) {
  for (uint16_t i = 0; i < *nb_spaceships; i++) {
    spaceships[i].team_id = 0;
    spaceships[i].ship_id = 0;
    spaceships[i].x = 0;
    spaceships[i].y = 0;
    spaceships[i].broken = 0;
  }
  *nb_spaceships = 0;
}

void spaceship_to_string(char *str, Spaceship spaceship) {
  sprintf(str, "Spaceship %d: x=%d, y=%d, broken=%d", spaceship.ship_id,
          spaceship.x, spaceship.y, spaceship.broken);
}

void spaceships_to_string(char *str, Spaceship *spaceships,
                          uint16_t nb_spaceships) {
  for (uint16_t i = 0; i < nb_spaceships; i++) {
    char spaceship_str[100];
    spaceship_to_string(spaceship_str, spaceships[i]);
    strcat(str, spaceship_str);
    strcat(str, "\n");
  }
}
