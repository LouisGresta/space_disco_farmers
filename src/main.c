#include <math.h>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
#include "cmsis_os.h"
#include "embedded_commands.h"
#include "functionCalculs.h"
#include "gameConstants.h"
#include "main.h"
#include "planet.h"
#include "spaceship.h"

osThreadId_t explorer1TaskHandle;
osThreadId_t explorer2TaskHandle;
osThreadId_t collectorsTaskHandle;
osThreadId_t attacker1TaskHandle;
osThreadId_t attacker2TaskHandle;
osThreadId_t attacker3TaskHandle;
osThreadId_t defender1TaskHandle;
osThreadId_t defender2TaskHandle;

uint16_t base_x, base_y;
uint16_t collector_focus[2][2];

// threads

// explorers
// get as argument the collector id to follow
void explorerTask(void *argument) {
  while (1) {

    osDelay(1000);
  }
}

// collectors
void collectorsTask(void *argument) {
  while (1) {
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

int main(void) {
  // les initialisations
  hardware_init();
  push_button_init();
  osKernelInitialize();

  while (!push_button_is_pressed()) {
    puts("Appuyez sur le bouton pour démarrer\n");
    osDelay(1000);
  }
  puts("Starting...\n");
  osDelay(3000);

  // explorers threads
  const osThreadAttr_t explorersTask_attributes = {
      .name = "explorersTask",
      .priority = (osPriority_t)osPriorityHigh,
      .stack_size = 2048,
  };
  if (explorer1TaskHandle =
          osThreadNew(explorerTask, NULL, &explorersTask_attributes) == NULL) {
    puts("Erreur lors de la création de la tache du premier explorer\n");
  }
  if (explorer2TaskHandle =
          osThreadNew(explorerTask, NULL, &explorersTask_attributes) == NULL) {
    puts("Erreur lors de la création de la tache du deuxième explorer\n");
  }
  // collectors threads
  const osThreadAttr_t collectorsTask_attributes = {
      .name = "collectorsTask",
      .priority = (osPriority_t)osPriorityAboveNormal,
      .stack_size = 2048,
  };
  if (collectorsTaskHandle = osThreadNew(collectorsTask, NULL,
                                         &collectorsTask_attributes) == NULL) {
    puts("Erreur lors de la création de la tache des collecteurs\n");
  }
  // attackers threads
  const osThreadAttr_t attackersTask_attributes = {
      .name = "attackersTask",
      .priority = (osPriority_t)osPriorityAboveNormal,
      .stack_size = 2048,
  };
  if (attacker1TaskHandle =
          osThreadNew(attackerTask, NULL, &attackersTask_attributes) == NULL) {
    puts("Erreur lors de la création de la tache des attaquants\n");
  }
  if (attacker2TaskHandle =
          osThreadNew(attackerTask, NULL, &attackersTask_attributes) == NULL) {
    puts("Erreur lors de la création de la tache des attaquants\n");
  }
  if (attacker3TaskHandle =
          osThreadNew(attackerTask, NULL, &attackersTask_attributes) == NULL) {
    puts("Erreur lors de la création de la tache des attaquants\n");
  }
  if (defender1TaskHandle =
          osThreadNew(defenderTask, NULL, &attackersTask_attributes) == NULL) {
    puts("Erreur lors de la création de la tache du premier défenseur\n");
  }
  if (defender2TaskHandle =
          osThreadNew(defenderTask, NULL, &attackersTask_attributes) == NULL) {
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
  osThreadTerminate(defender1TaskHandle);
  osThreadTerminate(defender2TaskHandle);
  osThreadTerminate(attacker3TaskHandle);

  // Stop the kernel
  osKernelTerminate();
  return 0;
}
