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

/** @file digital.c
 ** @brief Implementacion de biblioteca para gestion de entradas y salidas digitales
 **/

/* === Headers files inclusions ================================================================ */

#include "digital.h"
#include <stdlib.h>
#include "chip.h"

/* === Macros definitions ====================================================================== */

/* === Private data type declarations ========================================================== */
struct digital_ouput_s{
    uint32_t puerto;
    uint8_t terminal;
};

struct digital_input_s{
    uint32_t puerto;
    uint8_t terminal;
    bool inverted;
    bool lastState;
};
/* === Private function declarations =========================================================== */

/* === Private variable definitions ============================================================ */

/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */

/* === Public function implementation ========================================================== */

digital_ouput_t DigitalOutputCreate(uint32_t puerto, uint8_t terminal){
digital_ouput_t self; //puntero a estructura
self = malloc(sizeof(struct digital_ouput_s));
    if (self) {
    self->puerto=puerto;        //reserva memoria, guarda datos y devuelve puntero
    self->terminal=terminal;    //si tiene memoria me devuelve un puntero
    DigitalOutputDeactivate(self);
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self->puerto, self->terminal, true);
    }
    return self;
}

void DigitalOutputActivate(digital_ouput_t self){
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->puerto, self->terminal, true);
}

void DigitalOutputDeactivate(digital_ouput_t self){
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->puerto, self->terminal, false);
}

void DigitalOutputToggle(digital_ouput_t self){
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, self->puerto, self->terminal);
}

digital_input_t DigitalInputCreate(uint32_t puerto, uint8_t terminal, bool inverted){
digital_input_t self; //puntero a estructura
self = malloc(sizeof(struct digital_input_s));
    if (self) {
    self->puerto = puerto;        //reserva memoria, guarda datos y devuelve puntero
    self->terminal = terminal;    //si tiene memoria me devuelve un puntero
    Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self->puerto, self->terminal, false);
    self->inverted = inverted;
    self->lastState = DigitalInputRead(self);
    }
    return self;
}

bool DigitalInputRead(digital_input_t entrada){
    return (Chip_GPIO_GetPinState(LPC_GPIO_PORT, entrada->puerto, entrada->terminal) ==0) == entrada->inverted; //La tecla se enciende en alto
}

int DigitalInputHasChange(digital_input_t self){
    int resultado=0;
    bool actual = DigitalInputRead(self);
    if (actual && !self->lastState)
    {
        resultado = ACTIVATE_EVENT; 
    } else if (!actual && self->lastState)
    {
        resultado = DEACTIVATE_EVENT;
    }
    self->lastState = actual;
    return resultado;
}

bool DigitalInputHasActivate(digital_input_t self){
    return DigitalInputHasChange(self) == ACTIVATE_EVENT;
}

bool DigitalInputHasDeactivate(digital_input_t self){
    return DigitalInputHasChange(self) == DEACTIVATE_EVENT;
}

/* === End of documentation ==================================================================== */