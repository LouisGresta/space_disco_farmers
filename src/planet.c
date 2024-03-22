#include "planet.h"
#include <stdio.h>
#include <stdlib.h>

void create_planet(uint16_t planet_id, uint16_t x, uint16_t y, int8_t ship_id,
                   uint8_t saved, Planet **planets, uint16_t *nb_planets) {
  Planet *new_planet = malloc(sizeof(Planet));
  if (new_planet == NULL)
    return;
  new_planet->planet_id = planet_id;
  new_planet->x = x;
  new_planet->y = y;
  new_planet->ship_id = ship_id;
  new_planet->saved = saved;
  planets[*nb_planets] = new_planet;
  (*nb_planets)++;
}

Planet *get_planet(uint16_t planet_id, Planet **planets, uint16_t nb_planets) {
  for (uint16_t i = 0; i < nb_planets; i++) {
    if (planets[i]->planet_id == planet_id) {
      return planets[i];
    }
  }
  return NULL;
}

void set_planet(uint16_t planet_id, uint16_t x, uint16_t y, int8_t ship_id,
                uint8_t saved, Planet **planets, uint16_t nb_planets) {
  Planet *planet = get_planet(planet_id, planets, nb_planets);
  if (planet == NULL)
    return;
  planet->x = x;
  planet->y = y;
  planet->ship_id = ship_id;
  planet->saved = saved;
}

void delete_planet(uint16_t planet_id, Planet **planets, uint16_t *nb_planets) {
  for (uint16_t i = 0; i < *nb_planets; i++) {
    if (planets[i]->planet_id == planet_id) {
      free(planets[i]);
      for (uint16_t j = i; j < *nb_planets - 1; j++) {
        planets[j] = planets[j + 1];
      }
      (*nb_planets)--;
      break;
    }
  }
}

void delete_all_planets(Planet **planets, uint16_t *nb_planets) {
  for (uint16_t i = 0; i < *nb_planets; i++) {
    free(planets[i]);
  }
  *nb_planets = 0;
}
