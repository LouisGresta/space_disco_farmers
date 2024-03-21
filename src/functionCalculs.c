#include "functionCalculs.h"
#include "gameConstants.h"
#include <stdlib.h>

// fonction qui calcule l'angle à prendre pour aller d'un point A(X,Y) à un
// point B(x,y)

uint16_t get_travel_angle(uint16_t start_x, uint16_t start_y, uint16_t end_x,
                          uint16_t end_y) {

  float dx = end_x - start_x;
  float dy = end_y - start_y;

  float angle_rad = atan2(dy, dx);

  // Conversion de radians en degrés
  float angle_deg = angle_rad * (180.0 / M_PI);

  // Si l'angle est en négatif, on le règle pour qu'il soit entre 0 et 359
  if (angle_deg < 0)
    angle_deg += 360.0;

  else if (angle_deg >= 360)
    angle_deg -= 360;

  // Conversion de l'angle en entier non signé de 16 bits
  uint16_t angle_deg_uint16 = (uint16_t)angle_deg;

  return angle_deg_uint16;
}

// fonction qui calcule quel collecteur doit viser quelle planète
// Format variable de retour => [collector1 ID, planet1 ID, collector2 ID,
// planet2 ID] version actuelle => verif planete la plus proche du collecteur 1
// et apres collecteur 2 donc c'est opti avec le cul A UPDATE !

void determine_target_planets(Spaceship collector1, Spaceship collector2,
                              Planet *planets, uint8_t nb_planets,
                              uint16_t results[2][2]) {
  results[0][0] = collector1.ship_id;
  results[1][0] = collector2.ship_id;

  // Variables pour stocker les distances minimales et les indices des planètes
  uint16_t min_distance1 = AREA_LENGTH;
  uint16_t min_distance2 = AREA_LENGTH;
  uint8_t index_planet1;
  uint8_t index_planet2;

  for (uint8_t i = 0; i < nb_planets; i++) {

    if (!planets[i].saved) {
      // distance avec collecteur 1
      uint16_t dist1 = round(sqrt(pow(abs(collector1.x - planets[i].x), 2) +
                                  pow(abs(collector1.y - planets[i].y), 2)));
      // distance avec collecteur 2
      uint16_t dist2 = round(sqrt(pow(abs(collector2.x - planets[i].x), 2) +
                                  pow(abs(collector2.y - planets[i].y), 2)));

      // vérifier si la planète est la + proche de collector1
      if (dist1 < min_distance1) {
        min_distance1 = dist1;
        index_planet1 = i;
      }

      // vérifier si la planète est la + proche de collector2 sans avoir same
      // focus que collector1
      if (dist2 < min_distance2 && i != index_planet1) {
        min_distance2 = dist2;
        index_planet2 = i;
      }
    }
  }

  // Affectation des planètes cibles aux vaisseaux
  results[0][1] = planets[index_planet1].planet_id;
  results[1][1] = planets[index_planet2].planet_id;
}

/*

uint8_t is_in_set(const int *set, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (set[i] == value) {
            return 1;
        }
    }
    return 0;
}

uint8_t is_attackers(int value) {
    static const int attackers[] = ATTACKERS;
    static const int size = sizeof(ATTACKERS) / sizeof(ATTACKERS[0]);
    return is_in_set(attackers, size, value);
}

uint8_t is_explorers(int value) {
    static const int explorers[] = EXPLORERS;
    static const int size = sizeof(EXPLORERS) / sizeof(EXPLORERS[0]);
    return is_in_set(explorers, size, value);
}

uint8_t is_collectors(int value) {
    static const int collectors[] = COLLECTORS;
    static const int size = sizeof(COLLECTORS) / sizeof(COLLECTORS[0]);
    return is_in_set(collectors, size, value);
}


*/
