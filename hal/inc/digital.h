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

#ifndef DIGITAL_H_
#define DIGITAL_H_  

/** @file digital.h
 ** @brief Declaracion de biblioteca para gestion de entradas y salidas digitales
 **/

/* === Headers files inclusions ==================================================================================== */

#include <stdint.h>
#include <stdbool.h>

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

#define ACTIVATE_EVENT 1
#define DEACTIVATE_EVENT 2

/* === Public data type declarations =============================================================================== */

/**
 * @brief Estructura para representar una salida digital
 * 
 */
typedef struct digital_ouput_s * digital_ouput_t; //"digital_output_t es un puntero a struct digital_output_s"

/**
 * @brief Estructura para representar una entrada digital
 * 
 */
typedef struct digital_input_s * digital_input_t; //"digital_input_t es un puntero a struct digital_input_s"

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/**
 * @brief Crea un objeto para gestionar una salida digital
 * @param puerto Puerto al que pertenece la salida
 * @param terminal Terminal al que pertenece la salida
 * @return Puntero al objeto creado
 */
digital_ouput_t DigitalOutputCreate(uint32_t puerto, uint8_t terminal); //crea un objeto asociado al puerto ... y salida ...

/**
 * @brief Activa una salida digital
 * @param salida Puntero a la salida digital
 */
void DigitalOutputActivate(digital_ouput_t salida); //funcion que recibe salida digital y no devuelve nada.

/**
 * @brief Desactiva una salida digital
 * @param salida Puntero a la salida digital
 */
void DigitalOutputDeactivate(digital_ouput_t salida);

/**
 * @brief Alterna el estado de una salida digital
 * @param salida Puntero a la salida digital
 */
void DigitalOutputToggle(digital_ouput_t salida);

/**
 * @brief Crea un objeto para gestionar una entrada digital
 * @param puerto Puerto al que pertenece la entrada
 * @param terminal Terminal al que pertenece la entrada
 * @param inverted Indica si la entrada está invertida
 * @return Puntero al objeto creado
 */
digital_input_t DigitalInputCreate(uint32_t puerto, uint8_t terminal, bool inverted);

/**
 * @brief Lee el estado de una entrada digital
 * @param entrada Puntero a la entrada digital
 * @return Estado de la entrada digital
 */
bool DigitalInputRead(digital_input_t entrada);

/**
 * @brief Verifica si una entrada digital ha cambiado
 * @param self Puntero a la entrada digital
 * @return 1 si ha cambiado, 0 si no ha cambiado
 */
int DigitalInputHasChange(digital_input_t self);

/**
 * @brief Verifica si una entrada digital ha sido activada
 * @param self Puntero a la entrada digital
 * @return 1 si ha sido activada, 0 si no
 */
bool DigitalInputHasActivate(digital_input_t self);

/**
 * @brief Verifica si una entrada digital ha sido desactivada
 * @param self Puntero a la entrada digital
 * @return 1 si ha sido desactivada, 0 si no
 */
bool DigitalInputHasDeactivate(digital_input_t self);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* DIGITAL_H_ */