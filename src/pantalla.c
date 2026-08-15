#include "pantalla.h"

void RefreshDisplayTask(void * pointer) {
    display_task_args_t args = pointer;

    while (true) {
        /* Toma el Mutex para asegurar barrido atómico de dígitos */
        if (xSemaphoreTake(args->mutex, portMAX_DELAY) == pdTRUE) {
            DisplayRefresh(args->display);
            xSemaphoreGive(args->mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void RefreshElapsed(void * pointer) {
    /* Tarea reservada para actualización por colas en caso de desacoplar AppTask */
    vTaskDelete(NULL);
}