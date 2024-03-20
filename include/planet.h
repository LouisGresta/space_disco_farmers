#ifndef PLANET_H
#define PLANET_H

#include <stdint.h>

#define NB_MAX_PLANETS 8
struct Planet_t {
  uint16_t planet_id;
  int8_t ship_id;
  uint16_t x;
  uint16_t y;
  uint8_t saved;
} typedef Planet;

void create_planet(uint16_t planet_id, uint16_t x, uint16_t y, uint16_t ship_id,
                   uint8_t saved, Planet *planets, uint16_t *nb_planets);
void delete_planet(uint16_t planet_id, Planet *planets, uint16_t *nb_planets);
Planet *get_planet(uint16_t planet_id, Planet *planets, uint16_t nb_planets);

#endif // PLANET_H
