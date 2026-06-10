/************************************************************************************************
Copyright (c) 2016-2026, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnología, Universidad Nacional de Tucumán
https://www.microprocesadores.unt.edu.ar/

Copyright (c) 2016-2026, Barraza Silvio Fernando <silviobarraza13@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/** \brief Programa principal adaptado a la abstracción de hardware del Poncho UNT
 **
 ** \addtogroup aplicación Aplicación Principal
 ** @{ */

/* === Headers files inclusions =============================================================== */

#ifndef EDU_CIAA_NXP
#error "This program can only be compiled for the EDU-CIAA-NXP board"
#endif

#include "bsp.h"
#include "digital.h"
#include "screen.h"
#include <stdbool.h>

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

/* === Public function implementation ========================================================== */

int main(void) {

    int divisor = 0;
    int contador_puntos = 0;
    uint8_t entrada[4] = {1, 2, 3, 4}; // Valores iniciales en la pantalla

    // Inicialización de la placa del poncho
    board_t board = BoardCreate();
    
    // Escribe el valor inicial en el display
    DisplayWriteBCD(board->display, entrada, sizeof(entrada));

    while (true) {

        // Control manual del buzzer (en este caso sera un led azul)
        if (DigitalInputRead(board->accept)) {
            DigitalOutputActivate(board->buzzer);
        }
        if (DigitalInputRead(board->cancel)) {
            DigitalOutputDeactivate(board->buzzer);
        }

        // Con tecla f1 modifico el numero del display 4
        if (DigitalInputHasActivate(board->f1)) {
            entrada[3] = (entrada[3] + 1) % 10;
            DisplayWriteBCD(board->display, entrada, sizeof(entrada));
        }

        // Con tecla f2 modifico el numero del display 3
        if (DigitalInputHasActivate(board->f2)) {
            entrada[2] = (entrada[2] + 1) % 10;
            DisplayWriteBCD(board->display, entrada, sizeof(entrada));
        }

        // Con tecla f3 modifico el numero del display 2
        if (DigitalInputHasActivate(board->f3)) {
            entrada[1] = (entrada[1] + 1) % 10;
            DisplayWriteBCD(board->display, entrada, sizeof(entrada));
        }

        // Con tecla f4 modifico el numero del display 1
        if (DigitalInputHasActivate(board->f4)) {
            entrada[0] = (entrada[0] + 1) % 10;
            DisplayWriteBCD(board->display, entrada, sizeof(entrada));
        }

        // Divisor de ciclos y parpadeo de puntos decimales
        divisor++;
        if (divisor == 5) {
            divisor = 0;
            
            // Cada 50 refrescos de pantalla conmutamos el estado de los puntos
            contador_puntos++;
            if (contador_puntos >= 50) {
                contador_puntos = 0;
                DisplayToggleDots(board->display, 0, 3); // Prende y apaga los puntos 0, 1, 2 y 3
            }
        }

        // Barrido continuo del display multiplexado
        DisplayRefresh(board->display);

        // Retardo por software para controlar la velocidad del lazo
        for (int delay = 0; delay < 25000; delay++) {
            __asm("NOP");
        }
    }

    return 0;
}

/* === End of documentation ==================================================================== */
/** @} End of module definition for doxygen */