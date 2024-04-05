#include "embedded_spaceship.h"
#include "os_utils.h"

void init_embedded_spaceship(Embedded_spaceship *embedded_spaceship,
                             Spaceship *spaceship, uint8_t index) {
  embedded_spaceship->spaceship = spaceship;
  embedded_spaceship->index = index;
  embedded_spaceship->mutex_id = create_mutex(&spaceships_mutex_attr);
}

void init_embedded_spaceships(Embedded_spaceship *embedded_spaceships,
                              Spaceship *spaceships, uint16_t nb_spaceships) {
  for (uint16_t i = 0; i < nb_spaceships; i++) {
    init_embedded_spaceship(&embedded_spaceships[i], &spaceships[i], i);
  }
}

Spaceship get_spaceship_mutex(Embedded_spaceship *embedded_spaceships,
                              uint8_t index) {
  get_mutex(embedded_spaceships[index].mutex_id);
  Spaceship *spaceship = embedded_spaceships[index].spaceship;
  release_mutex(embedded_spaceships[index].mutex_id);
  return *spaceship;
}

Spaceship update_spaceship_mutex(Spaceship spaceship,
                                 Embedded_spaceship *embedded_spaceships,
                                 uint8_t index) {
  if (osMutexGetOwner(planets_spaceships_mutex_id) != NULL) {
    return spaceship;
  }
  return get_spaceship_mutex(embedded_spaceships, index);
}