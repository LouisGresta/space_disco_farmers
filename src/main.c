#include <math.h>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
#include "cmsis_os.h"
#include "main.h"

osThreadId_t defaultTaskHandle;
osThreadId_t Task1ID;

void StartDefaultTask(void *argument);

void StartDefaultTask(void *argument) {
  while (1) {
    puts("Hello world\n");
    osDelay(1000); // tache toute les secondes
  }
}

void Tache1(void *argument) {
  while (1) {
    puts("Tache 1 en cours\n");
    osDelay(1500); // tache toute les 1,5 secondes
  }
}

int main(void) {
  // les initialisations
  hardware_init();
  osKernelInitialize();

  // tache dafault
  const osThreadAttr_t defaultTask_attributes = {
      .name = "defaultTask",
      .priority = (osPriority_t)osPriorityNormal,
      .stack_size = 2048,
  };
  defaultTaskHandle =
      osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  if (defaultTaskHandle == NULL) {
    puts("Erreur lors de la création de la tache par défaut \n");
  }

  // tache 1
  const osThreadAttr_t Task1_attributes = {
      .name = "Task1",
      .priority = (osPriority_t)osPriorityNormal1,
      .stack_size = 2048,
  };
  Task1ID = osThreadNew(Tache1, NULL, &Task1_attributes);

  if (Task1ID == NULL) {
    puts("Erreur lors de la création de la tache 1 \n");
  }

  // démarrage du noyau
  osKernelStart();

  while (1) {
  }
}
