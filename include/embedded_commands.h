#ifndef EMBEDDED_COMMANDS_H
#define EMBEDDED_COMMANDS_H

#include "main.h"
#include "planet.h"
#include "spaceship.h"
#include <stdint.h>

extern uint16_t base_x, base_y;
uint8_t move(int8_t ship_id, uint16_t angle, uint16_t speed);
uint8_t move_v_max(int8_t ship_id, uint16_t angle);
uint8_t move_spaceship_to(Spaceship spaceship, uint16_t x, uint16_t y,
                          uint16_t speed);
uint8_t fire(int8_t ship_id, uint16_t angle);
void radar(char *response, int8_t ship_id);

extern osMutexId_t spaceships_mutex_id;
extern osMutexId_t planets_mutex_id;

// parse radar response using mutex
void parse_radar_response_mutex(const char *response, Planet *planets,
                                uint16_t *nb_planets, Spaceship *spaceships,
                                uint16_t *nb_spaceships, uint16_t *x_base,
                                uint16_t *y_base, uint8_t update_base);

void retour_base(Spaceship ship, uint16_t x_base, uint16_t y_base);

#endif // EMBEDDED_COMMANDS_H
