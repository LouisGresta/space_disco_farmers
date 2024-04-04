#ifndef COMMANDS_H
#define COMMANDS_H

#include "planet.h"
#include "spaceship.h"
#include <stdint.h>

#define MAX_COMMAND_SIZE 100
#define MAX_SPLIT_COUNT 200

char *move_str(int8_t ship_id, uint16_t angle, uint16_t speed);
char *fire_str(int8_t ship_id, uint16_t angle);
char *radar_str(int8_t ship_id);

void split(char **tokens, const char *str, const char delimiter,
           uint16_t *count);
void parse_radar_response(const char *response, Planet *planets,
                          uint16_t *nb_planets, Spaceship *spaceships,
                          uint16_t *nb_spaceships, uint16_t *x_base,
                          uint16_t *y_base);
#endif // COMMANDS_H
