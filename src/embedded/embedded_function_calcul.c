#include "embedded_function_calcul.h"
#include "functionCalculs.h"

void determine_target_planets_mutex(Spaceship collector1, Spaceship collector2,
                                    Planet *planets, uint8_t nb_planets,
                                    uint16_t results[2][2]) {
  get_mutex(planets_mutex_id);
  determine_target_planets(collector1, collector2, planets, nb_planets,
                           results);
  release_mutex(planets_mutex_id);
}

uint16_t determine_target_spaceship_angle_mutex(Spaceship attacker_ship,
                                                Spaceship *spaceships) {
  get_mutex(spaceships_mutex_id);
  uint16_t angle = determine_target_spaceship_angle(attacker_ship, spaceships);
  release_mutex(spaceships_mutex_id);
  return angle;
}
