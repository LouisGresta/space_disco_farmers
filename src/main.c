#include "main.h"
#include "embedded_commands.h"
#include "embedded_spaceship.h"
#include "functionCalculs.h"
#include "gameConstants.h"
#include "os_utils.h"
#include <stdlib.h>

// #define DEBUG_SERIAL 1

#define MAX_RESPONSE_SIZE 1024

osThreadId_t explorer1TaskHandle;
osThreadId_t explorer2TaskHandle;
osThreadId_t collectorsTaskHandle;
osThreadId_t attacker1TaskHandle;
osThreadId_t attacker2TaskHandle;
osThreadId_t baseDefenderTaskHandle;
osThreadId_t defender1TaskHandle;
osThreadId_t defender2TaskHandle;

const osMutexAttr_t serial_mutex_attr = {"serialMutex", osMutexPrioInherit,
                                         NULL, 0U};
const osMutexAttr_t planets_spaceships_mutex_attr = {
    "planets_spaceshipsMutex", osMutexPrioInherit, NULL, 0U};
const osMutexAttr_t planets_mutex_attr = {"planetsMutex", osMutexPrioInherit,
                                          NULL, 0U};
osMutexId_t serial_mutex_id;
osMutexId_t planets_spaceships_mutex_id;
osMutexId_t planets_mutex_id;

uint16_t collector_focus[2][2];
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
  if (embedded_ship->spaceship->ship_id == 6) {
    embedded_collector = get_embedded_spaceship(0, 8, embedded_spaceships);
    osDelay(500); // Pour désynchroniser le scan des radars
  } else {
    embedded_collector = get_embedded_spaceship(0, 9, embedded_spaceships);
  }
  collector =
      get_spaceship_mutex(embedded_spaceships, embedded_collector->index);
  while (1) {
    explorer = update_spaceship_mutex(explorer, embedded_spaceships,
                                      embedded_ship->index);
    collector = update_spaceship_mutex(collector, embedded_spaceships,
                                       embedded_collector->index);
    radar(radar_response, explorer.ship_id);
    parse_radar_response_mutex(radar_response, planets, &nb_planets, spaceships,
                               &nb_spaceships, &x_base, &y_base);
    move_spaceship_to(explorer, collector.x, collector.y, 1000);
    osDelay(5000);
  }
}

// collectors
void collectorsTask(void *argument) {
  while (1) {
    move(8, 90, 1000);
    osDelay(1000);
  }
}

// attackers
void attackerTask(void *argument) {
  Embedded_spaceship *embedded_ship = (Embedded_spaceship *)argument;
  Spaceship attacker =
      get_spaceship_mutex(embedded_spaceships, embedded_ship->index);

  while (1) {
    attacker = update_spaceship_mutex(attacker, embedded_spaceships,
                                      embedded_ship->index);
    osDelay(1000);
  }
}

// get as argument the collector id to follow
void defenderTask(void *argument) {
  Embedded_spaceship *embedded_ship = (Embedded_spaceship *)argument;
  Spaceship defender =
      get_spaceship_mutex(embedded_spaceships, embedded_ship->index);
  while (1) {
    defender = update_spaceship_mutex(defender, embedded_spaceships,
                                      embedded_ship->index);
    osDelay(1000);
  }
}

// base defender
void baseDefenderTask(void *argument) {
  Embedded_spaceship *embedded_ship = (Embedded_spaceship *)argument;
  Spaceship base_defender =
      get_spaceship_mutex(embedded_spaceships, embedded_ship->index);
  uint32_t startTime = osKernelGetTickCount(); // Temps de départ
  int direction = 1; // 1 pour avancer, -1 pour reculer
  int isVerticalMovementComplete =
      0; // 0 pour mouvement vertical en cours, 1 pour terminé

  while (1) {
    base_defender = update_spaceship_mutex(base_defender, embedded_spaceships,
                                           embedded_ship->index);
    fire(base_defender.ship_id, 90);

    uint32_t elapsedTime = osKernelGetTickCount() - startTime;

    if (!isVerticalMovementComplete) {
      if (elapsedTime < 2500) { // Durée du mouvement vertical en millisecondes
                                // (par exemple, 5000 pour 5 secondes)
        move(
            base_defender.ship_id, 90,
            1000); // Déplacer verticalement (exemple : angle 90, durée 1000 ms)
      } else {
        isVerticalMovementComplete = 1; // Le mouvement vertical est terminé
        startTime = osKernelGetTickCount(); // Réinitialiser le temps de départ
      }
    } else {
      if (elapsedTime <
          10000) { // Temps total du parcours horizontal en millisecondes (par
                   // exemple, 10000 pour 10 secondes)
        if (direction == 1) {
          move(base_defender.ship_id, 0, 2000); // Avancer
        } else {
          move(base_defender.ship_id, 180, 2000); // Reculer
        }
      } else {
        startTime = osKernelGetTickCount(); // Réinitialiser le temps de départ
        direction = -direction;             // Inverser la direction
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
  planets_spaceships_mutex_id = create_mutex(&planets_spaceships_mutex_attr);
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
                             &nb_spaceships, &x_base, &y_base);
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
  if ((collectorsTaskHandle = osThreadNew(
           collectorsTask, NULL, &collectorsTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache des collecteurs\n");
  }
  // attackers threads
  const osThreadAttr_t attackersTask_attributes = {
      .name = "attackersTask",
      .priority = (osPriority_t)osPriorityAboveNormal,
      .stack_size = 1024,
  };
  if ((attacker1TaskHandle = osThreadNew(
           attackerTask, get_embedded_spaceship(0, 1, embedded_spaceships),
           &attackersTask_attributes)) == NULL) {
    // Erreur lors de la création de la tache des attaquants\n");
  }
  if ((attacker2TaskHandle = osThreadNew(
           attackerTask, get_embedded_spaceship(0, 2, embedded_spaceships),
           &attackersTask_attributes)) == NULL) {
    // Erreur lors de la création de la tache des attaquants
  }

  if ((baseDefenderTaskHandle = osThreadNew(
           baseDefenderTask, get_embedded_spaceship(0, 3, embedded_spaceships),
           &attackersTask_attributes)) == NULL) {
    // Erreur lors de la création de la tache du défenseur de base
  }
  if ((defender1TaskHandle = osThreadNew(
           defenderTask, get_embedded_spaceship(0, 4, embedded_spaceships),
           &attackersTask_attributes)) == NULL) {
    // Erreur lors de la création de la tache du premier défenseur
  }
  if ((defender2TaskHandle = osThreadNew(
           defenderTask, get_embedded_spaceship(0, 5, embedded_spaceships),
           &attackersTask_attributes)) == NULL) {
    // Erreur lors de la création de la tache du deuxième défenseur
  }

  // démarrage du noyau
  osKernelStart();

  while (!push_button_is_pressed()) {
    // Wait for the user to press the button to stop the program
    osDelay(1000);
  }
  // Stop all threads
  osThreadTerminate(explorer1TaskHandle);
  osThreadTerminate(explorer2TaskHandle);
  osThreadTerminate(collectorsTaskHandle);
  osThreadTerminate(attacker1TaskHandle);
  osThreadTerminate(attacker2TaskHandle);
  osThreadTerminate(baseDefenderTaskHandle);
  osThreadTerminate(defender1TaskHandle);
  osThreadTerminate(defender2TaskHandle);
  return 0;
}
