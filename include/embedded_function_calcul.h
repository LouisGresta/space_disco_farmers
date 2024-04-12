#ifndef EMBEDDED_FUNCTION_CALCUL_H
#define EMBEDDED_FUNCTION_CALCUL_H

#include "os_utils.h"
#include "planet.h"
#include "spaceship.h"
#include <stdint.h>

extern osMutexId_t planets_mutex_id;
extern osMutexId_t spaceships_mutex_id;

void determine_target_planets_mutex(Spaceship collector1, Spaceship collector2,
                                    Planet *planets, uint8_t nb_planets,
                                    uint16_t results[2][2]);

uint16_t determine_target_spaceship_angle_mutex(Spaceship attacker_ship,
                                                Spaceship *spaceships);
#endif // EMBEDDED_FUNCTION_CALCUL_H
