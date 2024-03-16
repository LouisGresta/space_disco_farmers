#include "planet.h"
#include <stdlib.h>

void create_planet(uint16_t planet_id, uint16_t x, uint16_t y, uint16_t ship_id,
                   uint8_t saved, Planet *planets, uint16_t *nb_planets) {
  Planet *new_planet = malloc(sizeof(Planet));
  new_planet->planet_id = planet_id;
  new_planet->x = x;
  new_planet->y = y;
  new_planet->ship_id = ship_id;
  new_planet->saved = saved;
  planets[*nb_planets] = *new_planet;
  (*nb_planets)++;
}

void delete_planet(uint16_t planet_id, Planet *planets, uint16_t *nb_planets) {
  for (uint16_t i = 0; i < *nb_planets; i++) {
    if (planets[i].planet_id == planet_id) {
      free(&planets[i]);
      for (uint16_t j = i; j < *nb_planets - 1; j++) {
        planets[j] = planets[j + 1];
      }
      (*nb_planets)--;
      break;
    }
  }
}

Planet *get_planet(uint16_t planet_id, Planet *planets, uint16_t nb_planets) {
  for (uint16_t i = 0; i < nb_planets; i++) {
    if (planets[i].planet_id == planet_id) {
      return &planets[i];
    }
  }
  return NULL;
}
