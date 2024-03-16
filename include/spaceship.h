#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <stdint.h>

#define NB_MAX_SPACESHIPS 36

struct SpaceShip_t {
  uint8_t team_id;
  uint8_t ship_id;
  uint16_t x;
  uint16_t y;
  uint8_t broken;
} typedef Ship;

extern Ship spaceships[NB_MAX_SPACESHIPS];

void create_spaceship(uint8_t team_id, uint8_t ship_id, uint16_t x, uint16_t y,
                      uint8_t broken, Ship *spaceships,
                      uint16_t *nb_spaceships);
void delete_spaceship(uint8_t ship_id, Ship *spaceships,
                      uint16_t *nb_spaceships);
Ship *get_spaceship(uint8_t ship_id, Ship *spaceships, uint16_t nb_spaceships);
#endif // SPACESHIP_H
