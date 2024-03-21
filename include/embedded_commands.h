#ifndef EMBEDDED_COMMANDS_H
#define EMBEDDED_COMMANDS_H

#include "main.h"
#include <stdint.h>

uint8_t move(int8_t ship_id, uint16_t angle, uint16_t speed);
uint8_t move_v_max(int8_t ship_id, uint16_t angle);
uint8_t fire(int8_t ship_id, uint16_t angle);
char *radar(int8_t ship_id);

#endif // EMBEDDED_COMMANDS_H
