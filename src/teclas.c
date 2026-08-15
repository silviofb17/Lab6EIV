#include "teclas.h"

#define TECLA_DELAY_MS 100

void KeyTask(void * pointer) {
    key_task_args_t args = pointer;

    while (true) {
        while (!DigitalInputRead(args->input)) {
            vTaskDelay(pdMS_TO_TICKS(TECLA_DELAY_MS));
        }
        vTaskDelay(pdMS_TO_TICKS(TECLA_DELAY_MS));

        xEventGroupSetBits(args->event_group, args->event_bit);

        while (DigitalInputRead(args->input)) {
            vTaskDelay(pdMS_TO_TICKS(TECLA_DELAY_MS));
        }
        vTaskDelay(pdMS_TO_TICKS(TECLA_DELAY_MS));
    }
}