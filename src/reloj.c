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

#include "reloj.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */

/* === Private function declarations =========================================================== */

/* === Private variable definitions ============================================================ */

struct clock_s {
    clock_time_t current_time;
    bool valid_time;
    uint16_t ticks_per_second; 
    uint16_t tick_counter;
};

/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */

/* === Public function implementation ========================================================== */

clock_t RelojCreate(uint16_t ticks_por_segundo, void * callback){
    (void)callback; 

    clock_t self = malloc(sizeof(struct clock_s));
    if(self != NULL){
        self->valid_time = false;
        self->ticks_per_second = ticks_por_segundo; 
        self->tick_counter = 0;
        memset(&(self->current_time), 0, sizeof(clock_time_t)); //
    }
    return self; //
}

bool GetCurrentTime(clock_t reloj, hora_t hora_actual){
    if(reloj == NULL || hora_actual == NULL){
        return false;
    }

    if(!reloj->valid_time){
        memset(hora_actual, 0, sizeof(hora_t));
        return false;
    }

    memcpy(hora_actual, reloj->current_time.bcd, sizeof(hora_t));
    return true;
}

bool SetCurrentTime(clock_t reloj, hora_t nueva_hora) {
    if (reloj == NULL || nueva_hora == NULL) {
        return false;
    }
    
    memcpy(reloj->current_time.bcd, nueva_hora, sizeof(hora_t));
   
    reloj->valid_time = true;
    
    return true;
}

void ClockTick(clock_t reloj) {
    if (reloj == NULL || !reloj->valid_time) {
        return;
    }

    reloj->tick_counter++;

    if (reloj->tick_counter >= reloj->ticks_per_second) {
        reloj->tick_counter = 0;

        reloj->current_time.bcd[5]++;

        if (reloj->current_time.bcd[5] > 9) {
            reloj->current_time.bcd[5] = 0;
            reloj->current_time.bcd[4]++;

            if (reloj->current_time.bcd[4] > 5) {
                reloj->current_time.bcd[4] = 0;
                reloj->current_time.bcd[3]++;

                if (reloj->current_time.bcd[3] > 9) {
                    reloj->current_time.bcd[3] = 0;
                    reloj->current_time.bcd[2]++;

                    if (reloj->current_time.bcd[2] > 5) 
                        reloj->current_time.bcd[2] = 0;
                        reloj->current_time.bcd[1]++;

                        if (reloj->current_time.bcd[1] > 9) {
                            reloj->current_time.bcd[1] = 0;
                            reloj->current_time.bcd[0]++;
                        }
                    }
                }
            }
        }

        // Control de medianochee
        if (reloj->current_time.bcd[0] == 2 && reloj->current_time.bcd[1] == 4) {
            reloj->current_time.bcd[0] = 0; 
            reloj->current_time.bcd[1] = 0;
        }
    }

/* === End of documentation ==================================================================== */