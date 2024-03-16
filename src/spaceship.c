#include "spaceship.h"
#include <stdlib.h>

void create_spaceship(uint8_t team_id, uint8_t ship_id, uint16_t x, uint16_t y,
                      uint8_t broken, Ship *spaceships,
                      uint16_t *nb_spaceships) {
  Ship *new_spaceship = malloc(sizeof(Ship));
  new_spaceship->team_id = team_id;
  new_spaceship->ship_id = ship_id;
  new_spaceship->x = x;
  new_spaceship->y = y;
  new_spaceship->broken = broken;
  spaceships[*nb_spaceships] = *new_spaceship;
  (*nb_spaceships)++;
}

void delete_spaceship(uint8_t ship_id, Ship *spaceships,
                      uint16_t *nb_spaceships) {
  for (uint16_t i = 0; i < *nb_spaceships; i++) {
    if (spaceships[i].ship_id == ship_id) {
      free(&spaceships[i]);
      for (uint16_t j = i; j < *nb_spaceships - 1; j++) {
        spaceships[j] = spaceships[j + 1];
      }
      (*nb_spaceships)--;
      break;
    }
  }
}

Ship *get_spaceship(uint8_t ship_id, Ship *spaceships, uint16_t nb_spaceships) {
  for (uint16_t i = 0; i < nb_spaceships; i++) {
    if (spaceships[i].ship_id == ship_id) {
      return &spaceships[i];
    }
  }
  return NULL;
}
