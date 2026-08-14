#ifndef EDU_CIAA_NXP
#error "This program can only be compiled for the EDU-CIAA-NXP board"
#endif

#include "bsp.h"
#include "app.h"
#include "chip.h"

void SysTick_Handler(void) {
    AppTick();
}

int main(void) {
    // Inicialización del Hardware
    board_t board = BoardCreate();
    
    // Inicialización de la Aplicación
    AppInit(board);

    // Configuración del SysTick a 1 ms
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);

    while (true) {
        AppTask();
        __WFI();
    }
}