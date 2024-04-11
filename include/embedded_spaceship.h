#ifndef EMBEDDED_SPACESHIP_H
#define EMBEDDED_SPACESHIP_H

#include "os_utils.h"
#include "spaceship.h"
#include <stdint.h>

extern osMutexId_t spaceships_mutex_id;

struct Embedded_spaceship_t {
  Spaceship *spaceship;
  uint8_t index;
  osMutexId_t mutex_id;
} typedef Embedded_spaceship;

void init_embedded_spaceship(Embedded_spaceship *embedded_spaceship,
                             Spaceship *spaceship, uint8_t index);
void init_embedded_spaceships(Embedded_spaceship *embedded_spaceships,
                              Spaceship *spaceships);

Embedded_spaceship *
get_embedded_spaceship(uint8_t team_id, int8_t ship_id,
                       Embedded_spaceship *embedded_spaceships);

Spaceship get_spaceship_mutex(Embedded_spaceship *embedded_spaceships,
                              uint8_t index);

Spaceship update_spaceship_mutex(Spaceship spaceship,
                                 Embedded_spaceship *embedded_spaceships,
                                 uint8_t index);
#endif // EMBEDDED_SPACESHIP_H
