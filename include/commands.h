#ifndef COMMANDS_H
#define COMMANDS_H

#include "planet.h"
#include "spaceship.h"
#include <stdint.h>

void move_v_max(Spaceship ship, uint16_t angle);

const char *move_command(uint8_t ship_id, uint16_t angle, uint16_t speed);
const char *fire_command(uint8_t ship_id, uint16_t angle);
const char *radar_command(uint8_t ship_id);

char **split(const char *str, const char delimiter, uint16_t *count);
int parse_radar_response(const char *response, Planet *planets,
                         uint16_t *nb_planets, Spaceship *spaceships,
                         uint16_t *nb_spaceships, uint16_t *x_base,
                         uint16_t *y_base);
#endif // COMMANDS_H
