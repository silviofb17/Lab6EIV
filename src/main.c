#include "FreeRTOS.h"
#include "task.h"
#include "teclas.h"
#include "pantalla.h"
#include "app.h"
#include "bsp.h"

int main(void) {
    board_t board;
    static struct key_task_args_s key_aceptar;
    static struct key_task_args_s key_cancelar;
    static struct key_task_args_s key_mas;
    static struct key_task_args_s key_menos;
    static struct clock_task_args_s clock_args;
    static struct display_task_args_s display_args;

    EventGroupHandle_t keys_events;
    SemaphoreHandle_t screen_mutex;

    /* Inicialización de hardware a través del BSP */
    board = BoardCreate();

    keys_events = xEventGroupCreate();
    screen_mutex = xSemaphoreCreateMutex();

    if ((keys_events == NULL) || (screen_mutex == NULL)) {
        DigitalOutputActivate(board->buzzer);
        while (true);
    }

    /* Asignación de teclas asignadas según bsp.h (accept, cancel, f3, f4) */
    key_aceptar.event_group = keys_events;
    key_aceptar.event_bit = TECLA_ACEPTAR;
    key_aceptar.input = board->accept;
    xTaskCreate(KeyTask, "KeyAccept", KEY_TASK_STACK_SIZE, &key_aceptar, tskIDLE_PRIORITY + 1, NULL);

    key_cancelar.event_group = keys_events;
    key_cancelar.event_bit = TECLA_CANCELAR;
    key_cancelar.input = board->cancel;
    xTaskCreate(KeyTask, "KeyCancel", KEY_TASK_STACK_SIZE, &key_cancelar, tskIDLE_PRIORITY + 1, NULL);

    key_mas.event_group = keys_events;
    key_mas.event_bit = TECLA_MAS;
    key_mas.input = board->f4;
    xTaskCreate(KeyTask, "KeyPlus", KEY_TASK_STACK_SIZE, &key_mas, tskIDLE_PRIORITY + 1, NULL);

    key_menos.event_group = keys_events;
    key_menos.event_bit = TECLA_MENOS;
    key_menos.input = board->f3;
    xTaskCreate(KeyTask, "KeyMinus", KEY_TASK_STACK_SIZE, &key_menos, tskIDLE_PRIORITY + 1, NULL);

    /* Tarea Principal de la Aplicación */
    clock_args.events = keys_events;
    clock_args.board = board;
    xTaskCreate(AppTask, "AppTask", CLOCK_TASK_STACK_SIZE, &clock_args, tskIDLE_PRIORITY + 3, NULL);

    /* Tarea de Barrido de Pantalla */
    display_args.mutex = screen_mutex;
    display_args.display = board->display;
    xTaskCreate(RefreshDisplayTask, "Display", REFRESH_TASK_STACK_SIZE, &display_args, tskIDLE_PRIORITY + 5, NULL);

    /* Inicia el planificador de FreeRTOS */
    vTaskStartScheduler();

    DigitalOutputActivate(board->buzzer);
    while (true);

    return 0;
}