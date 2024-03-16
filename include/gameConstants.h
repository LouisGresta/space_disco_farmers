#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

#include <stdint.h>

#define MIN_PLANET_ID 0
#define MAX_PLANET_ID 65535

#define AREA_LENGTH 20000

#define ATTACKERS                                                              \
  { 1, 2, 3, 4, 5 }
#define EXPLORERS                                                              \
  { 6, 7 }
#define COLLECTORS                                                             \
  { 8, 9 }
// Move
#define COLLECTORS_MAX_SPEED 1000
#define EXPLORERS_MAX_SPEED 2000
#define ATTACKERS_MAX_SPEED 3000

#define MAX_ANGLE 359

// Fire
#define FIRE_RANGE 5000

// RADAR
#define RADAR_RANGE 5000
enum RadarType { PLANET = 'P', SPACESHIP = 'S', BASE = 'B' };

u_int16_t x_base = 0;
u_int16_t y_base = 0;

#endif // GAME_CONSTANTS_H
