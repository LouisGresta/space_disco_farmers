#ifndef FONCTION_CALCULS_H
#define FONCTION_CALCULS_H

#include "planet.h"
#include "spaceship.h"
#include <math.h>
#include <stdint.h>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

void determine_target_planets(Ship collector1, Ship collector2, Planet *planets,
                              uint8_t nb_planets, uint16_t results[2][2]);

float get_travel_angle(uint16_t start_x, uint16_t start_y, uint16_t end_x,
                       uint16_t end_y);

#endif // FONCTION_CALCULS_H
