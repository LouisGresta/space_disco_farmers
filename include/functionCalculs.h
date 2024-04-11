#ifndef FONCTION_CALCULS_H
#define FONCTION_CALCULS_H

#include "planet.h"
#include "spaceship.h"
#include <math.h>
#include <stdint.h>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define NOT_FOUND 404

void determine_target_planets(Spaceship collector1, Spaceship collector2,
                              Planet *planets, uint8_t nb_planets,
                              uint16_t results[2][2]);

uint16_t determine_target_planetV2(Spaceship collector, Planet *planets,
                                   uint8_t nb_planets);

uint16_t get_travel_angle(uint16_t start_x, uint16_t start_y, uint16_t end_x,
                          uint16_t end_y);

uint16_t get_angle_from_middle(uint16_t x_base, uint16_t y_base);

uint16_t determine_next_circle_point(uint16_t results[2], uint16_t angle_actuel,
                                     uint8_t direction);

uint16_t determine_target_spaceship_angle(Spaceship attacker_ship,
                                          Spaceship *spaceships);
#endif // FONCTION_CALCULS_H
