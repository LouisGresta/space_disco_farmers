#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <stdint.h>

#define NB_MAX_SPACESHIPS 36

struct SpaceShip_t {
  uint8_t team_id;
  int8_t ship_id;
  uint16_t x;
  uint16_t y;
  uint8_t broken; // 0 ou 1
} typedef Spaceship;

void create_spaceship(uint8_t team_id, int8_t ship_id, uint16_t x, uint16_t y,
                      uint8_t broken, Spaceship *spaceships,
                      uint16_t *nb_spaceships);
Spaceship *get_spaceship(uint8_t team_id, int8_t ship_id, Spaceship *spaceships,
                         uint16_t nb_spaceships);
void set_spaceship(uint8_t team_id, int8_t ship_id, uint16_t x, uint16_t y,
                   uint8_t broken, Spaceship *spaceships,
                   uint16_t nb_spaceships);

void delete_spaceship(uint8_t team_id, int8_t ship_id, Spaceship *spaceships,
                      uint16_t *nb_spaceships);
void delete_all_spaceships(Spaceship *spaceships, uint16_t *nb_spaceships);

void spaceship_to_string(char *str, Spaceship spaceship);
void spaceships_to_string(char *str, Spaceship *spaceships,
                          uint16_t nb_spaceships);
#endif // SPACESHIP_H
