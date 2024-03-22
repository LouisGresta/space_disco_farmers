#include <math.h>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
#include "commands.h"
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
osMutexId_t serial_mutex_id;

uint16_t collector_focus[2][2];

// threads

// explorers
// get as argument the collector id to follow
void explorerTask(void *argument) {
  while (1) {
    radar(6);
    osDelay(1000);
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
  while (1) {
    osDelay(1000);
  }
}

// get as argument the collector id to follow
void defenderTask(void *argument) {
  while (1) {
    osDelay(1000);
  }
}

// base defender
void baseDefenderTask(void *argument) {
  uint32_t startTime = osKernelGetTickCount(); // Temps de départ
  int direction = 1; // 1 pour avancer, -1 pour reculer
  int isVerticalMovementComplete =
      0; // 0 pour mouvement vertical en cours, 1 pour terminé

  while (1) {
    fire(1, 90);

    uint32_t elapsedTime = osKernelGetTickCount() - startTime;

    if (!isVerticalMovementComplete) {
      if (elapsedTime < 2500) { // Durée du mouvement vertical en millisecondes
                                // (par exemple, 5000 pour 5 secondes)
        move(
            1, 90,
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
          move(1, 0, 2000); // Avancer
        } else {
          move(1, 180, 2000); // Reculer
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
  if ((explorer1TaskHandle = osThreadNew(explorerTask, NULL,
                                         &explorersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache du premier explorer\n");
  }
  if ((explorer2TaskHandle = osThreadNew(explorerTask, NULL,
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
  if ((attacker1TaskHandle = osThreadNew(attackerTask, NULL,
                                         &attackersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache des attaquants\n");
  }
  if ((attacker2TaskHandle = osThreadNew(attackerTask, NULL,
                                         &attackersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache des attaquants\n");
  }
  if ((baseDefenderTaskHandle = osThreadNew(
           baseDefenderTask, NULL, &attackersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache du défenseur de base\n");
  }
  if ((defender1TaskHandle = osThreadNew(defenderTask, NULL,
                                         &attackersTask_attributes)) == NULL) {
    puts("Erreur lors de la création de la tache du premier défenseur\n");
  }
  if ((defender2TaskHandle = osThreadNew(defenderTask, NULL,
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
