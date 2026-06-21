/*********************************************************************************************************************
Copyright 2016-2026, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnología
Universidad Nacional de Tucuman
http://www.microprocesadores.unt.edu.ar/

Copyright 2016-2026, Barraza Silvio Fernando <silviobarraza13@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/** @file plantilla.c
 ** @brief Plantilla para la creación de archivos de código fuente en lenguaje C
 **/

/* === Headers files inclusions ================================================================ */

#include "screen.h"
#include "bsp.h"
#include <stdlib.h>
#include <string.h>

/* === Macros definitions ====================================================================== */

#ifndef DISPLAY_MAX_DIGITS
#define DISPLAY_MAX_DIGITS 8
#endif

/* === Private data type declarations ========================================================== */

struct display_s {
    uint8_t digits;                                 /**< Cantidad de digitos en la pantalla */
    uint8_t active_digit;                           /**< Indice del digito activo en el barrido */
    uint8_t flashing_from;                          /**< Primer digito con parpadeo habilitado */
    uint8_t flashing_to;                            /**< Ultimo digito con parpadeo habilitado */
    uint16_t flashing_frecuency;                    /**< Divisdor de frecuencia para el parpadeo */
    uint16_t flashing_count;                        /**< Contador de ciclos de refresco para parpadeo */
    uint8_t display_memory [DISPLAY_MAX_DIGITS];    /**< Patron de segmentos por digito */
    struct display_driver_s driver;                 /**< Driver de bajo nivel para el control de la pantalla */
};

/* === Private function declarations =========================================================== */

static display_t DisplayAllocate(void);

/* === Private variable definitions ============================================================ */

static const uint8_t IMAGES[] = {
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F,             //!< Representa los segmentos del número "0"
    SEGMENT_B | SEGMENT_C,                                                             //!< Representa los segmentos del número "1"
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_G,                         //!< Representa los segmentos del número "2"
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_G,                         //!< Representa los segmentos del número "3"
    SEGMENT_B | SEGMENT_C | SEGMENT_F | SEGMENT_G,                                     //!< Representa los segmentos del número "4"
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,                         //!< Representa los segmentos del número "5"
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,             //!< Representa los segmentos del número "6"
    SEGMENT_A | SEGMENT_B | SEGMENT_C,                                                 //!< Representa los segmentos del número "7"
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G, //!< Representa los segmentos del número "8"
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,             //!< Representa los segmentos del número "9"
};

/* === Public variable definition  ============================================================= */

/* ===  Private function implementation ========================================================== */

static display_t DisplayAllocate(void){
    static struct display_s instances[1] = {0};

    return &instances[0];
}

/* === Private function definitions ============================================================ */

/* === Public function implementation ========================================================== */

display_t DisplayCreate(uint8_t digits, display_driver_t driver) {
    display_t display = DisplayAllocate();

    if (display) {
        display->digits = digits;
        display->active_digit = digits - 1;
        display->flashing_count = 0;
        display->flashing_from = 0;
        display->flashing_to = 0;
        display->flashing_frecuency = 0;
        
        memcpy(&(display->driver), driver, sizeof(struct display_driver_s)); 
        memset(display->display_memory, 0, sizeof(display->display_memory));
        
        display->driver.UpdateSegments(0x00); 
    }

    return display;
}

void DisplayWriteBCD(display_t display, uint8_t * number, uint8_t size) {
    memset(display->display_memory, 0, sizeof(display->display_memory));
    for (int index = 0; index < size; index++) {
        if (index >= display->digits) {
            break;
        }
        display->display_memory[index] = IMAGES[number[index]];
    }
}

void DisplayRefresh(display_t display) {
    uint8_t segments;

    display->driver.UpdateSegments(0x00);
    display->active_digit = (display->active_digit + 1) % display->digits;

    if (display->active_digit == 0) {
        display->flashing_count++;
        if (display->flashing_count >= display->flashing_frecuency) {
            display->flashing_count = 0;
        }
    }

    segments = display->display_memory[display->active_digit];
    if (display->flashing_frecuency > 0) {
        if (display->flashing_count >= display->flashing_frecuency / 2) {
            if ((display->active_digit >= display->flashing_from) && (display->active_digit <= display->flashing_to)) {
                segments = 0;
            }
        }
    }

    display->driver.UpdateSegments(segments);
    display->driver.UpdateDigits(display->active_digit);
}

void DisplayFlashDigits(display_t display, uint8_t from, uint8_t to, uint16_t frecuency) {
    display->flashing_count = 0;
    display->flashing_from = from;
    display->flashing_to = to;
    display->flashing_frecuency = frecuency;
}

void DisplayToggleDots(display_t display, uint8_t from, uint8_t to) {
    for (int index = from; index <= to; index++) {
        display->display_memory[index] ^= SEGMENT_P;
    }
}

/* === End of documentation ==================================================================== */