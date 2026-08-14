#ifndef APP_H_
#define APP_H_

#include "bsp.h"
#include "reloj.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Inicializa la aplicación del reloj
 * @param board Puntero a la abstracción de la placa
 */
void AppInit(board_t board);

/**
 * @brief Procesa un tick de tiempo (debe ser llamado periódicamente desde SysTick o main)
 */
void AppTick(void);

/**
 * @brief Ejecuta la tarea principal de la máquina de estados
 */
void AppTask(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_H_ */