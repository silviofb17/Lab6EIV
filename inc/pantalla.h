#ifndef PANTALLA_H_
#define PANTALLA_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include <board.h>
#include "screen.h"

#define REFRESH_TASK_STACK_SIZE 256

typedef struct refresh_task_args_s {
    QueueHandle_t data;      /* Cola con los 4 dígitos BCD a mostrar */
    SemaphoreHandle_t mutex; /* Mutex de acceso exclusivo a la pantalla */
    display_t display;       /* Descriptor de la pantalla multiplexada */
} * refresh_task_args_t;

typedef struct display_task_args_s {
    SemaphoreHandle_t mutex;
    display_t display;
} * display_task_args_t;

void RefreshDisplayTask(void * args);
void RefreshElapsed(void * args);

#endif /* PANTALLA_H_ */