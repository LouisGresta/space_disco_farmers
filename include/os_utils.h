#ifndef OS_UTILS_H_
#define OS_UTILS_H_

#include "cmsis_os.h"

osMutexId_t create_mutex(const osMutexAttr_t *mutex_attr);
void get_mutex(osMutexId_t mutex_id);
void release_mutex(osMutexId_t mutex_id);

#endif // OS_UTILS_H_
