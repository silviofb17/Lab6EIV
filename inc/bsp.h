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

#ifndef BSP_H_
#define BSP_H_

/** @file bsp.h
 ** @brief Abstraccion del hardware de la placa con poncho
 **
 ** Agrupa entradas, salidas digitales, pantalla multiplexada y temporizador SysTick.
 **/

/* === Headers files inclusions ==================================================================================== */

#include "digital.h"
#include "screen.h"
#include <stdint.h> 

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

/**
 * @brief Estructura para representar la placa con poncho
 * 
 */
typedef struct board_s
{
    digital_ouput_t buzzer;     /**< Salida del zumbador */
    digital_input_t f1;         /**< Tecla de funcion F1 */
    digital_input_t f2;         /**< Tecla de funcion F2 */
    digital_input_t f3;         /**< Tecla de funcion F3 */
    digital_input_t f4;         /**< Tecla de funcion F4 */
    digital_input_t accept;     /**< Tecla aceptar */
    digital_input_t cancel;     /**< Tecla cancelar */
    display_t display;          /**< Pantalla multiplexada de siete segmentos */
}const * board_t;


/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/**
 * @brief Crea un objeto para gestionar la placa con poncho
 * @return Puntero al objeto creado
 */
board_t BoardCreate();

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* BSP_H_ */