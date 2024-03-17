#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

const char *move_command(uint8_t ship_id, uint16_t angle, uint16_t speed);
const char *fire_command(uint8_t ship_id, uint16_t angle);
const char *radar_command(uint8_t ship_id);

char **split(const char *str, const char delimiter, uint16_t *count);

#endif // COMMANDS_H