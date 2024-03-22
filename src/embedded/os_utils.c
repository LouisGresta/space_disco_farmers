#include "os_utils.h"

// Mutexes

osMutexId_t create_mutex(const osMutexAttr_t *mutex_attr) {
  osMutexId_t mutex_id = osMutexNew(mutex_attr);
  if (mutex_id != NULL) {
    // Mutex object created
    return mutex_id;
  }
  return create_mutex(mutex_attr);
}

void get_mutex(osMutexId_t mutex_id) {
  if (osMutexAcquire(mutex_id, osWaitForever) != osOK) {
    get_mutex(mutex_id);
  }
}

void release_mutex(osMutexId_t mutex_id) {
  if (osMutexRelease(mutex_id) != osOK) {
    release_mutex(mutex_id);
  }
}
