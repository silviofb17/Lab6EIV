/************************************************************************************************
Copyright (c) 2022-2023, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnología, Universidad Nacional de Tucumán
https://www.microprocesadores.unt.edu.ar/

Copyright (c) 2022-2023, Esteban Volentini <evolentini@herrera.unt.edu.ar>

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

/** \brief EDU-CIAA-NXP board sample application
 **
 ** \addtogroup samples Samples
 ** \brief Samples applications with MUJU Framwork
 ** @{ */

/* === Headers files inclusions =============================================================== */

#ifndef EDU_CIAA_NXP
#error "This program can only be compiled for the EDU-CIAA-NXP board"
#endif

#include "placa.h"


/* === Macros definitions ====================================================================== */

//aqui iban los #define

/* === Private data type declarations ========================================================== */

/**
 * @brief Enumeration with color sequence of RGB led
 */
typedef enum rgb_color_e {
    LED_RED_ON = 0,
    LED_RED_OFF,
    LED_GREEN_ON,
    LED_GREEN_OFF,
    LED_BLUE_ON,
    LED_BLUE_OFF,
} rgb_color_t;

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/**
 * @brief Function to flash RGB led in sequence
 */
static void FlashLed(board_t placa);

/**
 * @brief Function to switch on and off a led with two keys
 */
static void SwitchLed(board_t placa);

/**
 * @brief Function to switch on and off a led with a single key
 */
static void ToggleLed(board_t placa);

/**
 * @brief Function to turn on a led while a key is pressed
 */
static void TestLed(board_t placa);

/**
 * @brief Function to generate a delay of approximately 100 ms
 */
static void Delay(void);

/* === Public variable definitions ============================================================= */

digital_ouput_t led_verde; //creo puntero llamado led_verde
digital_ouput_t led_rojo; //creo puntero llamado led_rojo
digital_ouput_t led_amarillo; //creo puntero llamado led_amarillo

digital_ouput_t led_rgb_rojo;
digital_ouput_t led_rgb_verde; //punteros a los leds correspondientes
digital_ouput_t led_rgb_azul;

digital_input_t tecla_1;
digital_input_t tecla_2;  //puntero a cada tecla correspondiente
digital_input_t tecla_3;
digital_input_t tecla_4;

/* === Private variable definitions ============================================================ */

/* === Private function implementation ========================================================= */

static void FlashLed(board_t placa) {
    static int divisor = 0;
    static rgb_color_t state = LED_BLUE_OFF;

    divisor++;
    if (divisor == 5) {
        divisor = 0;
        state = (state + 1) % (LED_BLUE_OFF + 1);

        switch (state) {
        case LED_RED_ON:
            DigitalOutputActivate(placa->led_rgb_rojo);
            //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_R_GPIO, LED_R_BIT, true);
            break;
        case LED_GREEN_ON:
            DigitalOutputActivate(placa->led_rgb_verde);
            //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, true);
            break;
        case LED_BLUE_ON:
            DigitalOutputActivate(placa->led_rgb_azul);
            //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_B_GPIO, LED_B_BIT, true);
            break;
        default:
            DigitalOutputDeactivate(placa->led_rgb_rojo);
            DigitalOutputDeactivate(placa->led_rgb_verde);
            DigitalOutputDeactivate(placa->led_rgb_azul);
            //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_R_GPIO, LED_R_BIT, false);
            //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, false);
            //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_B_GPIO, LED_B_BIT, false);
            break;
        }
    }
}

static void SwitchLed(board_t placa) {
    if (DigitalInputRead(placa->tecla_prender)) { //Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TEC_1_GPIO, TEC_1_BIT) == 0
        DigitalOutputActivate(placa->led_rojo);
        //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_1_GPIO, LED_1_BIT, true);
    }
    if (DigitalInputRead(placa->tecla_apagar)) { //Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TEC_2_GPIO, TEC_2_BIT) == 0
        DigitalOutputDeactivate(placa->led_rojo);
        //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_1_GPIO, LED_1_BIT, false);
    }
}

static void ToggleLed(board_t placa) {
    //static bool last_state = false;
    //bool current_state;

    //current_state = (DigitalInputRead(tecla_3) == 0); //Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TEC_3_GPIO, TEC_3_BIT)
    if (DigitalInputHasActivate(placa->tecla_cambiar)) { //(current_state) && (!last_state)
        DigitalOutputToggle(placa->led_amarillo);
        //Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, LED_2_GPIO, LED_2_BIT);
    }
    //last_state = current_state;
}

static void TestLed(board_t placa) {
    if (DigitalInputRead(placa->tecla_probar)) { //Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, TEC_4_GPIO, TEC_4_BIT) == 0
        //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_3_GPIO, LED_3_BIT, true);
        DigitalOutputActivate(placa->led_verde);
    } else {
        //Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_3_GPIO, LED_3_BIT, false);
        DigitalOutputDeactivate(placa->led_verde);
    }
}

static void Delay(void) {
    for (int index = 0; index < 100; index++) {
        for (int delay = 0; delay < 25000; delay++) {
            __asm("NOP");
        }
    }
}

/* === Public function implementation ========================================================== */

int main(void) {

    //BoardSetup();
    //ConfigureLeds();
    //ConfigureKeys();
    board_t placa = BoardCreate();

    while (true) {
        FlashLed(placa);
        SwitchLed(placa);
        ToggleLed(placa);
        TestLed(placa);

        Delay();
    }

    return 0;
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
