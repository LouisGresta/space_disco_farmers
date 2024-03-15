#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

const char* move_command(uint8_t ship_id, uint16_t angle, uint16_t speed);
const char* fire_command(uint8_t ship_id, uint16_t angle);
const char* radar_command(uint8_t ship_id);
#endif // COMMANDS_H