#include "planet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_planet(uint16_t planet_id, uint16_t x, uint16_t y, int8_t ship_id,
                   uint8_t saved, Planet *planets, uint16_t *nb_planets) {
  Planet new_planet;
  new_planet.planet_id = planet_id;
  new_planet.x = x;
  new_planet.y = y;
  new_planet.ship_id = ship_id;
  new_planet.saved = saved;
  planets[*nb_planets] = new_planet;
  (*nb_planets)++;
}

Planet *get_planet(uint16_t planet_id, Planet *planets) {
  for (uint16_t i = 0; i < NB_MAX_PLANETS; i++) {
    if (planets[i].planet_id == planet_id) {
      return &planets[i];
    }
  }
  return NULL;
}

void set_planet(uint16_t planet_id, uint16_t x, uint16_t y, int8_t ship_id,
                uint8_t saved, Planet *planets) {
  Planet *planet = get_planet(planet_id, planets);
  if (planet == NULL)
    return;
  planet->x = x;
  planet->y = y;
  planet->ship_id = ship_id;
  planet->saved = saved;
}

void delete_planet(uint16_t planet_id, Planet *planets, uint16_t *nb_planets) {
  for (uint16_t i = 0; i < NB_MAX_PLANETS; i++) {
    if (planets[i].planet_id == planet_id) {
      planets[i].planet_id = 0;
      planets[i].x = 0;
      planets[i].y = 0;
      planets[i].ship_id = 0;
      planets[i].saved = 0;
      (*nb_planets)--;
      break;
    }
  }
}

void delete_all_planets(Planet *planets, uint16_t *nb_planets) {
  for (uint16_t i = 0; i < NB_MAX_PLANETS; i++) {
    planets[i].planet_id = 0;
    planets[i].x = 0;
    planets[i].y = 0;
    planets[i].ship_id = 0;
    planets[i].saved = 0;
  }
  *nb_planets = 0;
}

void planet_to_string(char *str, Planet planet) {
  sprintf(str, "Planet %d: x=%d, y=%d, ship_id=%d, saved=%d", planet.planet_id,
          planet.x, planet.y, planet.ship_id, planet.saved);
}
void planets_to_string(char *str, Planet *planets) {
  for (uint16_t i = 0; i < NB_MAX_PLANETS; i++) {
    char planet_str[100];
    planet_to_string(planet_str, planets[i]);
    strcat(str, planet_str);
    strcat(str, "\n");
  }
}
