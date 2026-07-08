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
#include "reloj.h"
#include <stdbool.h>
#include <string.h>

/* === Macros definitions ====================================================================== */

/* === Definición de Estados del Reloj === */
typedef enum {
    ESTADO_SIN_CONFIGURAR,
    ESTADO_NORMAL,
    ESTADO_AJUSTE_HORA_MIN,
    ESTADO_AJUSTE_HORA_HOR,
    ESTADO_AJUSTE_ALARMA_MIN,
    ESTADO_AJUSTE_ALARMA_HOR
} estado_reloj_t;

#define TICKS_POR_SEGUNDO   500 
#define TIEMPO_3_SEGUNDOS   (3 * TICKS_POR_SEGUNDO)
#define TIEMPO_30_SEGUNDOS  (30 * TICKS_POR_SEGUNDO)

/* === Public function implementation ========================================================== */

int main(void) {
    // Inicialización del hardware y reloj
    board_t board = BoardCreate();
    clock_t reloj = RelojCreate(TICKS_POR_SEGUNDO, NULL);

    // Variables de estado del sistema
    estado_reloj_t estado_actual = ESTADO_SIN_CONFIGURAR;
    bool alarma_sonando = false;
    static int ultimo_minuto = -1;
    static bool alarma_pospuesta = false;
    static bool punto_segundos_encendido = false;

    // Vectores temporales BCD para los ajustes locales y para el display
    uint8_t hora_ajuste[6] = {0, 0, 0, 0, 0, 0};
    uint8_t display_buffer[4] = {0, 0, 0, 0};

    // Contadores para control de tiempos
    uint32_t cont_f1 = 0;
    uint32_t cont_f2 = 0;
    uint32_t cont_inactividad = 0;
    uint32_t cont_ticks = 0;

    // Estado inicial: El reloj arranca titilando completo
    DisplayFlashDigits(board->display, 0, 3, TICKS_POR_SEGUNDO / 2);

    while (true) {
        // Base de tiempo para el reloj interno
        ClockTick(reloj);
        cont_ticks++;

        // Control del parpadeo del punto de segundos (1 vez por second)
        if (cont_ticks >= TICKS_POR_SEGUNDO) {
            cont_ticks = 0;
            if (estado_actual == ESTADO_NORMAL) {
                punto_segundos_encendido = !punto_segundos_encendido; 
            } else {
                punto_segundos_encendido = false;
            }
        }

        // Leer la hora actual para mostrar de forma normal
        hora_t hora_actual;
        GetCurrentTime(reloj, hora_actual);

        // Si cambia el minuto del reloj, liberamos la bandera de postergación
        if (ultimo_minuto != hora_actual[3]) {
            ultimo_minuto = hora_actual[3];
            alarma_pospuesta = false; 
        }

        bool tecla_aceptar_activa = DigitalInputHasActivate(board->accept);
        bool tecla_cancelar_activa = DigitalInputHasActivate(board->cancel);

        switch (estado_actual) {

            case ESTADO_SIN_CONFIGURAR:
                display_buffer[0] = 0; display_buffer[1] = 0;
                display_buffer[2] = 0; display_buffer[3] = 0;
                DisplayWriteBCD(board->display, display_buffer, 4);

                if (DigitalInputRead(board->f1)) {
                    cont_f1++;
                    if (cont_f1 >= TIEMPO_3_SEGUNDOS) {
                        cont_f1 = 0;
                        estado_actual = ESTADO_AJUSTE_HORA_MIN;
                        DisplayFlashDigits(board->display, 2, 3, TICKS_POR_SEGUNDO / 4);
                        cont_inactividad = 0;
                    }
                } else { cont_f1 = 0; }
                break;

            case ESTADO_NORMAL:
                display_buffer[0] = hora_actual[0]; display_buffer[1] = hora_actual[1];
                display_buffer[2] = hora_actual[2]; display_buffer[3] = hora_actual[3];
                DisplayWriteBCD(board->display, display_buffer, 4);    

                if (punto_segundos_encendido) {
                    DisplayToggleDots(board->display, 1, 1);
                }

                if (IsAlarmEnabled(reloj)) {
                    DisplayToggleDots(board->display, 0, 0); 
                }

                if (DigitalInputRead(board->f1)) {
                    if (++cont_f1 >= TIEMPO_3_SEGUNDOS) {
                        cont_f1 = 0;
                        GetCurrentTime(reloj, hora_ajuste);
                        estado_actual = ESTADO_AJUSTE_HORA_MIN;
                        DisplayFlashDigits(board->display, 2, 3, TICKS_POR_SEGUNDO / 4);
                        cont_inactividad = 0;
                    }
                } else { cont_f1 = 0; }

                if (DigitalInputRead(board->f2)) {
                    if (++cont_f2 >= TIEMPO_3_SEGUNDOS) {
                        cont_f2 = 0;
                        GetAlarmTime(reloj, hora_ajuste);
                        estado_actual = ESTADO_AJUSTE_ALARMA_MIN;
                        DisplayFlashDigits(board->display, 2, 3, TICKS_POR_SEGUNDO / 4);
                        cont_inactividad = 0;
                    }
                } else { cont_f2 = 0; }

                if (tecla_aceptar_activa && !alarma_sonando) {
                    SetAlarmEnabled(reloj, true);
                }
                if (tecla_cancelar_activa && !alarma_sonando) {
                    SetAlarmEnabled(reloj, false);
                }
                break;

            case ESTADO_AJUSTE_HORA_MIN:
                cont_inactividad++;
                display_buffer[0] = hora_ajuste[0]; display_buffer[1] = hora_ajuste[1];
                display_buffer[2] = hora_ajuste[2]; display_buffer[3] = hora_ajuste[3];
                DisplayWriteBCD(board->display, display_buffer, 4);

                if (DigitalInputHasActivate(board->f4)) {
                    cont_inactividad = 0;
                    hora_ajuste[3]++;
                    if (hora_ajuste[3] > 9) { hora_ajuste[3] = 0; hora_ajuste[2]++; }
                    if (hora_ajuste[2] > 5) { hora_ajuste[2] = 0; }
                }
                if (DigitalInputHasActivate(board->f3)) {
                    cont_inactividad = 0;
                    if (hora_ajuste[3] == 0) {
                        hora_ajuste[3] = 9;
                        if (hora_ajuste[2] == 0) { hora_ajuste[2] = 5; } else { hora_ajuste[2]--; }
                    } else { hora_ajuste[3]--; }
                }

                if (tecla_aceptar_activa) {
                    estado_actual = ESTADO_AJUSTE_HORA_HOR;
                    DisplayFlashDigits(board->display, 0, 1, TICKS_POR_SEGUNDO / 4); 
                    cont_inactividad = 0;
                }
                if (tecla_cancelar_activa || cont_inactividad >= TIEMPO_30_SEGUNDOS) {
                    DisplayFlashDigits(board->display, 0, 0, 0); 
                    estado_actual = (estado_actual == ESTADO_SIN_CONFIGURAR) ? ESTADO_SIN_CONFIGURAR : ESTADO_NORMAL;
                }
                break;

            case ESTADO_AJUSTE_HORA_HOR:
                cont_inactividad++;
                display_buffer[0] = hora_ajuste[0]; display_buffer[1] = hora_ajuste[1];
                display_buffer[2] = hora_ajuste[2]; display_buffer[3] = hora_ajuste[3];
                DisplayWriteBCD(board->display, display_buffer, 4);

                if (DigitalInputHasActivate(board->f4)) {   
                    cont_inactividad = 0;
                    hora_ajuste[1]++;
                    if (hora_ajuste[1] > 9) { hora_ajuste[1] = 0; hora_ajuste[0]++; }
                    if (hora_ajuste[0] == 2 && hora_ajuste[1] > 3) { hora_ajuste[0] = 0; hora_ajuste[1] = 0; }
                }
                if (DigitalInputHasActivate(board->f3)) {
                    cont_inactividad = 0;
                    if (hora_ajuste[1] == 0) {
                        if (hora_ajuste[0] == 0) { hora_ajuste[0] = 2; hora_ajuste[1] = 3; } 
                        else { hora_ajuste[0]--; hora_ajuste[1] = 9; }
                    } else { hora_ajuste[1]--; }
                }

                if (tecla_aceptar_activa) {   
                    SetCurrentTime(reloj, hora_ajuste); 
                    DisplayFlashDigits(board->display, 0, 0, 0);    
                    estado_actual = ESTADO_NORMAL;
                }
                if (tecla_cancelar_activa || cont_inactividad >= TIEMPO_30_SEGUNDOS) {
                    DisplayFlashDigits(board->display, 0, 0, 0);
                    estado_actual = ESTADO_NORMAL;
                }
                break;

            case ESTADO_AJUSTE_ALARMA_MIN:
                cont_inactividad++;
                display_buffer[0] = hora_ajuste[0]; display_buffer[1] = hora_ajuste[1];
                display_buffer[2] = hora_ajuste[2]; display_buffer[3] = hora_ajuste[3];
                DisplayWriteBCD(board->display, display_buffer, 4);
                
                DisplayToggleDots(board->display, 0, 3);

                if (DigitalInputHasActivate(board->f4)) {
                    cont_inactividad = 0;
                    hora_ajuste[3]++;
                    if (hora_ajuste[3] > 9) { hora_ajuste[3] = 0; hora_ajuste[2]++; }
                    if (hora_ajuste[2] > 5) { hora_ajuste[2] = 0; }
                }
                if (DigitalInputHasActivate(board->f3)) {
                    cont_inactividad = 0;
                    if (hora_ajuste[3] == 0) {
                        hora_ajuste[3] = 9;
                        if (hora_ajuste[2] == 0) { hora_ajuste[2] = 5; } else { hora_ajuste[2]--; }
                    } else { hora_ajuste[3]--; }
                }

                if (tecla_aceptar_activa) {
                    estado_actual = ESTADO_AJUSTE_ALARMA_HOR;
                    DisplayFlashDigits(board->display, 0, 1, TICKS_POR_SEGUNDO / 4);
                    cont_inactividad = 0;
                }
                if (tecla_cancelar_activa || cont_inactividad >= TIEMPO_30_SEGUNDOS) {
                    DisplayFlashDigits(board->display, 0, 0, 0);
                    estado_actual = ESTADO_NORMAL;
                }
                break;

            case ESTADO_AJUSTE_ALARMA_HOR:
                cont_inactividad++;
                display_buffer[0] = hora_ajuste[0]; display_buffer[1] = hora_ajuste[1];
                display_buffer[2] = hora_ajuste[2]; display_buffer[3] = hora_ajuste[3];
                DisplayWriteBCD(board->display, display_buffer, 4);

                DisplayToggleDots(board->display, 0, 3);

                if (DigitalInputHasActivate(board->f4)) {
                    cont_inactividad = 0;
                    hora_ajuste[1]++;
                    if (hora_ajuste[1] > 9) { hora_ajuste[1] = 0; hora_ajuste[0]++; }
                    if (hora_ajuste[0] == 2 && hora_ajuste[1] > 3) { hora_ajuste[0] = 0; hora_ajuste[1] = 0; }
                }
                if (DigitalInputHasActivate(board->f3)) {
                    cont_inactividad = 0;
                    if (hora_ajuste[1] == 0) {
                        if (hora_ajuste[0] == 0) {hora_ajuste[0] = 2; hora_ajuste[1] = 3; }
                        else { hora_ajuste[0]--; hora_ajuste[1] = 9; }
                    } else { hora_ajuste[1]--; }
                }

                if (tecla_aceptar_activa) {
                    SetAlarmTime(reloj, hora_ajuste);
                    DisplayFlashDigits(board->display, 0, 0, 0);
                    estado_actual = ESTADO_NORMAL;
                }
                if (tecla_cancelar_activa || cont_inactividad >= TIEMPO_30_SEGUNDOS) {
                    DisplayFlashDigits(board->display, 0, 0, 0);
                    estado_actual = ESTADO_NORMAL;
                }
                break;

            default:
                break;
        }

        hora_t alarma_real;
        GetAlarmTime(reloj, alarma_real);

        if (IsAlarmEnabled(reloj) && (memcmp(hora_actual, alarma_real, 6) == 0) && !alarma_pospuesta) { 
            if (!alarma_sonando) {
                alarma_sonando = true;  
            }
        }

        if (alarma_sonando) {   
            DigitalOutputActivate(board->buzzer);   
            
            if (tecla_aceptar_activa) { 
                alarma_pospuesta = true; 
                alarma_sonando = false; 
                DigitalOutputDeactivate(board->buzzer);

                GetCurrentTime(reloj, alarma_real);

                int horas_enteras = (alarma_real[0] * 10) + alarma_real[1];
                int minutos_enteros = (alarma_real[2] * 10) + alarma_real[3];
                
                minutos_enteros += 5;
                if (minutos_enteros >= 60) {
                    minutos_enteros -= 60;
                    horas_enteras += 1;
                    if (horas_enteras >= 24) {
                        horas_enteras = 0;
                    }
                }

                alarma_real[0] = horas_enteras / 10;
                alarma_real[1] = horas_enteras % 10;
                alarma_real[2] = minutos_enteros / 10;
                alarma_real[3] = minutos_enteros % 10;

                SetAlarmTime(reloj, alarma_real);
            }
            if (tecla_cancelar_activa) { 
                alarma_pospuesta = true;
                alarma_sonando = false;
                DigitalOutputDeactivate(board->buzzer);
            }
        } else {
            DigitalOutputDeactivate(board->buzzer); 
        }

        DisplayRefresh(board->display); 

        for (volatile int delay = 0; delay < 34000; delay++) {  
            __asm("NOP");  
        }
    }
}