#include "main.h"
#include "embedded_commands.h"
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
const osMutexAttr_t planets_spceships_mutex_attr = {
    "planets_spceshipsMutex", osMutexPrioInherit, NULL, 0U};
osMutexId_t serial_mutex_id;
osMutexId_t planets_spceships_mutex_id;

uint16_t collector_focus[2][2];
Planet planets[NB_MAX_PLANETS];
uint16_t nb_planets = 0;
Spaceship spaceships[NB_MAX_SPACESHIPS];
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
  Spaceship *explorer = (Spaceship *)argument;
  char radar_response[MAX_RESPONSE_SIZE];
  while (1) {
    radar(radar_response, explorer->ship_id);
    parse_radar_response_mutex(radar_response, planets, &nb_planets, spaceships,
                               &nb_spaceships, &x_base, &y_base);
    // TODO : emit refresh signal for threads
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
  Spaceship *attacker = (Spaceship *)argument;
  uint8_t direction = 0;
  if (attacker->ship_id != 2) // le 2 va dans le sens horaire et le 4 en sens inverse
    direction = 1;

  uint16_t angle_actuel = 270;
  uint16_t angle_cible = 0;
  uint16_t results[2];

  while (1) {

    // verif vaisseau cassé ou pas
    // si cassé:
    angle_actuel = 270;
    // retour base

    // si pas cassé :

    angle_actuel =
        determine_next_circle_point(results, angle_actuel, direction);
    // get angle selon notre position initiale
    // moov avec l'angle que l'on récupère au dessus
    fire(attacker_id, angle_cible);
    osDelay(1000);
    fire(attacker_id, angle_cible);
    osDelay(1000);
  }
}

// get as argument the collector id to follow
void defenderTask(void *argument) {
  Spaceship *defender = (Spaceship *)argument;
  while (1) {
    osDelay(1000);
  }
}

// base defender
void baseDefenderTask(void *argument) {
  Spaceship *base_defender = (Spaceship *)argument;
  uint32_t startTime = osKernelGetTickCount(); // Temps de départ
  int direction = 1; // 1 pour avancer, -1 pour reculer
  int isVerticalMovementComplete =
      0; // 0 pour mouvement vertical en cours, 1 pour terminé

  while (1) {
    fire(base_defender->ship_id, 90);

    uint32_t elapsedTime = osKernelGetTickCount() - startTime;

    if (!isVerticalMovementComplete) {
      if (elapsedTime < 2500) { // Durée du mouvement vertical en millisecondes
                                // (par exemple, 5000 pour 5 secondes)
        move(
            base_defender->ship_id, 90,
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
          move(base_defender->ship_id, 0, 2000); // Avancer
        } else {
          move(base_defender->ship_id, 180, 2000); // Reculer
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
  // les initialisations
  hardware_init();
  push_button_init();
  osKernelInitialize();

  serial_mutex_id = create_mutex(&serial_mutex_attr);
  planets_spceships_mutex_id = create_mutex(&planets_spceships_mutex_attr);
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
           explorerTask, get_spaceship(0, 6, spaceships, nb_spaceships),
           &explorersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache du premier explorer\n");
  }
  if ((explorer2TaskHandle = osThreadNew(
           explorerTask, get_spaceship(0, 7, spaceships, nb_spaceships),
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
           attackerTask, get_spaceship(0, 2, spaceships, nb_spaceships),
           &attackersTask_attributes)) == NULL) {
    // Erreur lors de la création de la tache des attaquants\n");
  }
  if ((attacker2TaskHandle = osThreadNew(
           attackerTask, get_spaceship(0, 4, spaceships, nb_spaceships),
           &attackersTask_attributes)) == NULL) {
    // Erreur lors de la création de la tache des attaquants
  }

  if ((baseDefenderTaskHandle = osThreadNew(
           baseDefenderTask, get_spaceship(0, 3, spaceships, nb_spaceships),
           &attackersTask_attributes)) == NULL) {
    // Erreur lors de la création de la tache du défenseur de base
  }
  if ((defender1TaskHandle = osThreadNew(
           defenderTask, get_spaceship(0, 1, spaceships, nb_spaceships),
           &attackersTask_attributes)) == NULL) {
    // Erreur lors de la création de la tache du premier défenseur
  }
  if ((defender2TaskHandle = osThreadNew(
           defenderTask, get_spaceship(0, 5, spaceships, nb_spaceships),
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
