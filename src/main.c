#include "main.h"
#include "embedded_commands.h"
#include "embedded_function_calcul.h"
#include "embedded_spaceship.h"
#include "functionCalculs.h"
#include "gameConstants.h"
#include "os_utils.h"
#include <stdlib.h>
#include <string.h>

// #define DEBUG_SERIAL 1

#define MAX_RESPONSE_SIZE 1024

osThreadId_t explorer1TaskHandle;
osThreadId_t explorer2TaskHandle;
osThreadId_t collector1TaskHandle;
osThreadId_t collector2TaskHandle;
osThreadId_t attacker1TaskHandle;
osThreadId_t attacker2TaskHandle;
osThreadId_t baseDefenderTaskHandle;
osThreadId_t defender1TaskHandle;
osThreadId_t defender2TaskHandle;

const osMutexAttr_t serial_mutex_attr = {"serialMutex", osMutexPrioInherit,
                                         NULL, 0U};
const osMutexAttr_t spaceships_mutex_attr = {"spaceshipsMutex",
                                             osMutexPrioInherit, NULL, 0U};
const osMutexAttr_t planets_mutex_attr = {"planetsMutex", osMutexPrioInherit,
                                          NULL, 0U};
osMutexId_t serial_mutex_id;
osMutexId_t spaceships_mutex_id;
osMutexId_t planets_mutex_id;

Planet planets[NB_MAX_PLANETS] = {0};
uint16_t nb_planets = 0;
Spaceship spaceships[NB_MAX_SPACESHIPS] = {0};
Embedded_spaceship embedded_spaceships[NB_MAX_SPACESHIPS] = {0};
uint16_t nb_spaceships = 0;
uint16_t x_base = 0;
uint16_t y_base = 0;
// threads
struct collector_follow_args {
  int8_t collector_id;
  int8_t id;
} typedef collector_follow_args;

// explorers
void explorerTask(void *argument) {
  Embedded_spaceship *embedded_ship = (Embedded_spaceship *)argument;
  Spaceship explorer =
      get_spaceship_mutex(embedded_spaceships, embedded_ship->index);
  Embedded_spaceship *embedded_collector;
  Spaceship collector;
  char radar_response[MAX_RESPONSE_SIZE];
  uint8_t is_returning = 0;
  if (embedded_ship->spaceship->ship_id == 6) {
    embedded_collector = get_embedded_spaceship(0, 8, embedded_spaceships);
    osDelay(500); // Pour désynchroniser le scan des radars
  } else {
    embedded_collector = get_embedded_spaceship(0, 9, embedded_spaceships);
  }
  collector =
      get_spaceship_mutex(embedded_spaceships, embedded_collector->index);
  while (1) {
    // update spaceships
    explorer = update_spaceship_mutex(explorer, embedded_spaceships,
                                      embedded_ship->index);
    collector = update_spaceship_mutex(collector, embedded_spaceships,
                                       embedded_collector->index);
    // if broken go back to base
    if (explorer.broken && !is_returning) {
      move_spaceship_to(explorer, x_base, y_base, EXPLORERS_MAX_SPEED);
      is_returning = 1;
    } else if (!explorer.broken && is_returning) {
      is_returning = 0;
    }
    // if not broken do the logic
    if (!explorer.broken) {
      radar(radar_response, explorer.ship_id);
      parse_radar_response_mutex(radar_response, planets, &nb_planets,
                                 spaceships, &nb_spaceships, &x_base, &y_base,
                                 0);
      move_spaceship_to(explorer, collector.x, collector.y,
                        EXPLORERS_MAX_SPEED);
    }
    osDelay(1000);
  }
}

// collectors
void collectorsTask(void *argument) {
  Embedded_spaceship *embedded_ship = (Embedded_spaceship *)argument;

  Spaceship collector =
      get_spaceship_mutex(embedded_spaceships, embedded_ship->index);

  Planet target_planet;

  while (1) {
    collector = update_spaceship_mutex(collector, embedded_spaceships,
                                       embedded_ship->index);

    get_mutex(planets_mutex_id);
    target_planet = determine_target_planetV2(collector, planets, nb_planets);
    release_mutex(planets_mutex_id);

    if (!collector.broken && target_planet.ship_id == -1) {

      // move vers la planète cible
      move_spaceship_to(collector, target_planet.x, target_planet.y,
                        COLLECTORS_MAX_SPEED);
    } else {
      retour_base(collector, x_base, y_base);
    }

    osDelay(1500);
  }
}

// attackers
void attackerTask(void *argument) {
  Embedded_spaceship *embedded_ship = (Embedded_spaceship *)argument;
  Spaceship attacker =
      get_spaceship_mutex(embedded_spaceships, embedded_ship->index);
  uint8_t is_returning = 0;
  while (1) {
    // update spaceship
    attacker = update_spaceship_mutex(attacker, embedded_spaceships,
                                      embedded_ship->index);
    // if broken go back to base
    if (attacker.broken && !is_returning) {
      move_spaceship_to(attacker, x_base, y_base, COLLECTORS_MAX_SPEED);
      is_returning = 1;
    } else if (!attacker.broken && is_returning) {
      is_returning = 0;
    }
    // if not broken do the logic
    if (!attacker.broken) {
      // TODO : implement the attacker logic
    }
    osDelay(1000);
  }
}

// collector defenders
void defenderTask(void *argument) {
  Embedded_spaceship *embedded_ship = (Embedded_spaceship *)argument;
  Spaceship defender =
      get_spaceship_mutex(embedded_spaceships, embedded_ship->index);
  Embedded_spaceship *embedded_collector;
  Spaceship collector;
  if (embedded_ship->spaceship->ship_id == 4) {
    embedded_collector = get_embedded_spaceship(0, 8, embedded_spaceships);
  } else {
    embedded_collector = get_embedded_spaceship(0, 9, embedded_spaceships);
  }
  collector =
      get_spaceship_mutex(embedded_spaceships, embedded_collector->index);
  uint8_t is_returning = 0;
  uint16_t fire_angle = NOT_FOUND;
  while (1) {
    // update spaceships
    defender = update_spaceship_mutex(defender, embedded_spaceships,
                                      embedded_ship->index);
    collector = update_spaceship_mutex(collector, embedded_spaceships,
                                       embedded_collector->index);
    // if broken go back to base
    if (defender.broken && !is_returning) {
      move_spaceship_to(defender, x_base, y_base, COLLECTORS_MAX_SPEED);
      is_returning = 1;
    } else if (!defender.broken && is_returning) {
      is_returning = 0;
    }
    // if not broken do the logic
    if (!defender.broken) {
      // follow the collector
      move_spaceship_to(defender, collector.x, collector.y,
                        ATTACKERS_MAX_SPEED);
      fire_angle = determine_target_spaceship_angle_mutex(defender, spaceships);
      if (fire_angle != NOT_FOUND) {
        fire(defender.ship_id, fire_angle);
      } else {
        fire(defender.ship_id,
             (get_angle_from_middle(x_base, y_base) + 180) % 360);
      }
    }
    osDelay(1000);
  }
}

// base defender
void baseDefenderTask(void *argument) {
  Embedded_spaceship *embedded_ship = (Embedded_spaceship *)argument;
  Spaceship base_defender =
      get_spaceship_mutex(embedded_spaceships, embedded_ship->index);
  uint32_t startTime = osKernelGetTickCount(); // Temps de départ
  uint8_t state = 0; // État du mouvement trapézoïdal
  uint16_t fire_angle = NOT_FOUND;
  while (1) {
    base_defender = update_spaceship_mutex(base_defender, embedded_spaceships,
                                           embedded_ship->index);
    fire_angle =
        determine_target_spaceship_angle_mutex(base_defender, spaceships);
    if (fire_angle != NOT_FOUND) {
      fire(base_defender.ship_id, fire_angle);
    } else {
      fire(base_defender.ship_id,
           (get_angle_from_middle(x_base, y_base) + 180) % 360);
    }

    uint32_t elapsedTime = osKernelGetTickCount() - startTime;

    if (state == 0) {
      // Mouvement vers l'avant
      if (elapsedTime < 4000) {
        move(base_defender.ship_id, 0,
             ATTACKERS_MAX_SPEED); // Avancer à la vitesse maximale
      } else {
        startTime = osKernelGetTickCount(); // Réinitialiser le temps de départ
        state = 1;                          // Changer d'état
      }
    } else if (state == 1) {
      // Mouvement circulaire
      if (elapsedTime < 2000) {
        move(base_defender.ship_id, 60,
             1500); // Déplacer en arc de cercle avec un angle de 45 degrés et
                    // une vitesse réduite
      } else {
        startTime = osKernelGetTickCount(); // Réinitialiser le temps de départ
        state = 2;                          // Changer d'état
      }
    } else if (state == 2) {
      // Mouvement vers l'arrière
      if (elapsedTime < 6000) {
        move(base_defender.ship_id, 180,
             ATTACKERS_MAX_SPEED); // Reculer à la vitesse maximale
      } else {
        startTime = osKernelGetTickCount(); // Réinitialiser le temps de départ
        state = 3;                          // Changer d'état
      }
    } else if (state == 3) {
      // Mouvement circulaire
      if (elapsedTime < 3000) {
        move(base_defender.ship_id, 315,
             1500); // Déplacer en arc de cercle avec un angle de 225 degrés et
                    // une vitesse réduite
      } else {
        startTime = osKernelGetTickCount(); // Réinitialiser le temps de départ
        state = 0;                          // Revenir à l'état initial
      }
    }
    osDelay(1000);
  }
}

int main(void) {
  hardware_init();
  push_button_init();
  osKernelInitialize();
  init_embedded_spaceships(embedded_spaceships, spaceships);

  serial_mutex_id = create_mutex(&serial_mutex_attr);
  spaceships_mutex_id = create_mutex(&spaceships_mutex_attr);
  planets_mutex_id = create_mutex(&planets_mutex_attr);
#ifdef DEBUG_SERIAL
  while (!push_button_is_pressed()) {
    // Wait for the user to press the button to start the program
    osDelay(1000);
  }
#else
  gets(NULL); // wait the START message
#endif
  // first scan
  char radar_response[MAX_RESPONSE_SIZE];
  radar(radar_response, 6);
  parse_radar_response_mutex(radar_response, planets, &nb_planets, spaceships,
                             &nb_spaceships, &x_base, &y_base, 1);
  // explorers threads
  const osThreadAttr_t explorersTask_attributes = {
      .name = "explorersTask",
      .priority = (osPriority_t)osPriorityHigh,
      .stack_size = 2048,
  };

  if ((explorer1TaskHandle = osThreadNew(
           explorerTask, get_embedded_spaceship(0, 6, embedded_spaceships),
           &explorersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache du premier explorer\n");
  }
  if ((explorer2TaskHandle = osThreadNew(
           explorerTask, get_embedded_spaceship(0, 7, embedded_spaceships),
           &explorersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache du deuxième explorer\n");
  }
  // collectors threads
  const osThreadAttr_t collectorsTask_attributes = {
      .name = "collectorsTask",
      .priority = (osPriority_t)osPriorityAboveNormal,
      .stack_size = 1024,
  };
  if ((collector1TaskHandle = osThreadNew(
           collectorsTask, get_embedded_spaceship(0, 8, embedded_spaceships),
           &collectorsTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache des collecteurs\n");
  }
  if ((collector2TaskHandle = osThreadNew(
           collectorsTask, get_embedded_spaceship(0, 9, embedded_spaceships),
           &collectorsTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache des collecteurs\n");
  }
  // attackers threads
  // const osThreadAttr_t attackersTask_attributes = {
  //     .name = "attackersTask",
  //     .priority = (osPriority_t)osPriorityAboveNormal,
  //     .stack_size = 1024,
  // };
  // if ((attacker1TaskHandle = osThreadNew(
  //          attackerTask, get_embedded_spaceship(0, 1, embedded_spaceships),
  //          &attackersTask_attributes)) == NULL) {
  //   // Erreur lors de la création de la tache des attaquants\n");
  // }
  // if ((attacker2TaskHandle = osThreadNew(
  //          attackerTask, get_embedded_spaceship(0, 2, embedded_spaceships),
  //          &attackersTask_attributes)) == NULL) {
  //   // Erreur lors de la création de la tache des attaquants
  // }

  // if ((baseDefenderTaskHandle = osThreadNew(
  //          baseDefenderTask, get_embedded_spaceship(0, 3,
  //          embedded_spaceships), &attackersTask_attributes)) == NULL) {
  //   // Erreur lors de la création de la tache du défenseur de base
  // }
  // if ((defender1TaskHandle = osThreadNew(
  //          defenderTask, get_embedded_spaceship(0, 4, embedded_spaceships),
  //          &attackersTask_attributes)) == NULL) {
  //   // Erreur lors de la création de la tache du premier défenseur
  // }
  // if ((defender2TaskHandle = osThreadNew(
  //          defenderTask, get_embedded_spaceship(0, 5, embedded_spaceships),
  //          &attackersTask_attributes)) == NULL) {
  //   // Erreur lors de la création de la tache du deuxième défenseur
  // }

  // démarrage du noyau
  osKernelStart();

  while (!push_button_is_pressed()) {
    // Wait for the user to press the button to stop the program
    osDelay(1000);
  }
  // Stop all threads
  osThreadTerminate(explorer1TaskHandle);
  osThreadTerminate(explorer2TaskHandle);
  osThreadTerminate(collector1TaskHandle);
  osThreadTerminate(collector2TaskHandle);
  osThreadTerminate(attacker1TaskHandle);
  osThreadTerminate(attacker2TaskHandle);
  osThreadTerminate(baseDefenderTaskHandle);
  osThreadTerminate(defender1TaskHandle);
  osThreadTerminate(defender2TaskHandle);

  while (1)
    ;
  return 0;
}
