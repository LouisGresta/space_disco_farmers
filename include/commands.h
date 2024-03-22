#ifndef COMMANDS_H
#define COMMANDS_H

#include "planet.h"
#include "spaceship.h"
#include <stdint.h>

#define MAX_RESPONSE_SIZE 1024

char *move_str(int8_t ship_id, uint16_t angle, uint16_t speed);
char *fire_str(int8_t ship_id, uint16_t angle);
char *radar_str(int8_t ship_id);

char **split(const char *str, const char delimiter, uint16_t *count);
int parse_radar_response(const char *response, Planet **planets,
                         uint16_t *nb_planets, Spaceship **spaceships,
                         uint16_t *nb_spaceships, uint16_t *x_base,
                         uint16_t *y_base);
#endif // COMMANDS_H
