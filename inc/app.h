#ifndef APP_H_
#define APP_H_

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "queue.h"
#include "reloj.h"
#include "bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CLOCK_TASK_STACK_SIZE 512

/**
 * @brief Argumentos para la tarea de la aplicación del reloj
 */
typedef struct clock_task_args_s {
    EventGroupHandle_t events;    /* Grupo de eventos de las teclas */
    board_t board;                /* Descriptor de la placa EDU-CIAA */
} * clock_task_args_t;

/**
 * @brief Inicializa el módulo de la aplicación
 */
void AppInit(board_t board);

/**
 * @brief Tarea principal de FreeRTOS que ejecuta la máquina de estados del reloj
 */
void AppTask(void * args);

#ifdef __cplusplus
}
#endif

#endif /* APP_H_ */