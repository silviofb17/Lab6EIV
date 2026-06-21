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

#ifndef RELOJ_H
#define RELOJ_H

/** @file reloj.h
 ** @brief Módulo para la gestión del reloj en el sistema
 **/

/* === Headers files inclusions ==================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

typedef union {
    struct {
        uint8_t hours[2];
        uint8_t minutes[2];
        uint8_t seconds[2];
    } time;
    uint8_t bcd[6];
} clock_time_t;

/* === Public data type declarations =============================================================================== */

// Declaración de la estructura del reloj (definida en reloj.c)
typedef struct clock_s * clock_t;

// Tipo para representar la hora en formato BCD (6 dígitos: HHMMSS)
typedef uint8_t hora_t[6];

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

// Función para crear un nuevo reloj
clock_t RelojCreate(uint16_t ticks_por_segundo, void * alarm_handler);

// Función para obtener la hora actual del reloj
bool GetCurrentTime(clock_t reloj, hora_t hora_actual);

// Función para ajustar la hora del reloj
bool SetCurrentTime(clock_t reloj, hora_t nueva_hora);

// Función para procesar cada tick del sistema
void ClockTick(clock_t reloj);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* PLANTILLA_H_ */