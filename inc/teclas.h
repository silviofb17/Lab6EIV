#ifndef TECLAS_H_
#define TECLAS_H_

#include "FreeRTOS.h"
#include "event_groups.h"
#include "bsp.h"

/* Asignación de bits para las 4 teclas */
#define TECLA_ACEPTAR     ((EventBits_t)(1 << 0))
#define TECLA_CANCELAR    ((EventBits_t)(1 << 1))
#define TECLA_MAS         ((EventBits_t)(1 << 2))
#define TECLA_MENOS       ((EventBits_t)(1 << 3))

#define KEY_TASK_STACK_SIZE 256

typedef struct key_task_args_s {
    EventGroupHandle_t event_group;
    EventBits_t event_bit;
    digital_input_t input;
} * key_task_args_t;

void KeyTask(void * args);

#endif /* TECLAS_H_ */