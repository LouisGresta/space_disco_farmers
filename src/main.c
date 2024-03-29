#include <math.h>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
#include "embedded_commands.h"
#include "functionCalculs.h"
#include "gameConstants.h"
#include "main.h"
#include "os_utils.h"

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
Planet *planets[NB_MAX_PLANETS];
uint16_t nb_planets = 0;
Spaceship *spaceships[NB_MAX_SPACESHIPS];
uint16_t nb_spaceships = 0;
uint16_t x_base = 0;
uint16_t y_base = 0;
// threads
struct collector_follow_args {
  int8_t collector_id;
  int8_t id;
} typedef collector_follow_args;

// explorers
// get as argument the collector id to follow and his id
void explorerTask(void *argument) {
  collector_follow_args *args = (collector_follow_args *)argument;
  int8_t collector_id = args->collector_id;
  int8_t explorer_id = args->id;
  while (1) {
    char *radar_response = radar(explorer_id);
    parse_radar_response_mutex(radar_response, planets, &nb_planets, spaceships,
                               &nb_spaceships, &x_base, &y_base);
    osDelay(2000);
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
// get as argument his id
void attackerTask(void *argument) {
  int8_t attacker_id = *(int8_t *)argument;
  while (1) {
    osDelay(1000);
  }
}

// get as argument the collector id to follow
void defenderTask(void *argument) {
  collector_follow_args *args = (collector_follow_args *)argument;
  int8_t collector_id = args->collector_id;
  int8_t defender_id = args->id;
  while (1) {
    osDelay(1000);
  }
}

// base defender
// get as argument his id
void baseDefenderTask(void *argument) {
  int8_t base_defender_id = *(int8_t *)argument;
  uint32_t startTime = osKernelGetTickCount(); // Temps de départ
  int direction = 1; // 1 pour avancer, -1 pour reculer
  int isVerticalMovementComplete =
      0; // 0 pour mouvement vertical en cours, 1 pour terminé

  while (1) {
    fire(base_defender_id, 90);

    uint32_t elapsedTime = osKernelGetTickCount() - startTime;

    if (!isVerticalMovementComplete) {
      if (elapsedTime < 2500) { // Durée du mouvement vertical en millisecondes
                                // (par exemple, 5000 pour 5 secondes)
        move(
            base_defender_id, 90,
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
          move(base_defender_id, 0, 2000); // Avancer
        } else {
          move(base_defender_id, 180, 2000); // Reculer
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

  while (!push_button_is_pressed()) {
    osDelay(2000);
  }
  osDelay(3000);

  // explorers threads
  const osThreadAttr_t explorersTask_attributes = {
      .name = "explorersTask",
      .priority = (osPriority_t)osPriorityHigh,
      .stack_size = 2048,
  };
  collector_follow_args explorer1_args = {8, 6};
  collector_follow_args explorer2_args = {9, 7};

  if ((explorer1TaskHandle = osThreadNew(explorerTask, &explorer1_args,
                                         &explorersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache du premier explorer\n");
  }
  if ((explorer2TaskHandle = osThreadNew(explorerTask, &explorer2_args,
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
  int8_t attacker1_id = 1;
  int8_t attacker2_id = 2;
  int8_t base_defender_id = 3;
  if ((attacker1TaskHandle = osThreadNew(attackerTask, &attacker1_id,
                                         &attackersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache des attaquants\n");
  }
  if ((attacker2TaskHandle = osThreadNew(attackerTask, &attacker2_id,
                                         &attackersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache des attaquants\n");
  }

  if ((baseDefenderTaskHandle = osThreadNew(baseDefenderTask, &base_defender_id,
                                            &attackersTask_attributes)) ==
      NULL) {
    puts("Erreur lors de la création de la tache du défenseur de base\n");
  }
  collector_follow_args defender1_args = {8, 4};
  collector_follow_args defender2_args = {9, 5};
  if ((defender1TaskHandle = osThreadNew(defenderTask, &defender1_args,
                                         &attackersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache du premier défenseur\n");
  }
  if ((defender2TaskHandle = osThreadNew(defenderTask, &defender2_args,
                                         &attackersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache du deuxième défenseur\n");
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
