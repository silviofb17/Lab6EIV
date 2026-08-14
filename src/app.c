#include "app.h"
#include "screen.h"
#include "reloj.h"
#include <stdbool.h>
#include <string.h>

#define TICKS_POR_SEGUNDO   1000 
#define TIEMPO_3_SEGUNDOS   (3 * TICKS_POR_SEGUNDO)
#define TIEMPO_30_SEGUNDOS  (30 * TICKS_POR_SEGUNDO)

typedef enum {
    ESTADO_SIN_CONFIGURAR,
    ESTADO_NORMAL,
    ESTADO_AJUSTE_HORA_MIN,
    ESTADO_AJUSTE_HORA_HOR,
    ESTADO_AJUSTE_ALARMA_MIN,
    ESTADO_AJUSTE_ALARMA_HOR
} estado_reloj_t;

static board_t board_app;
static clock_t reloj_app;
static estado_reloj_t estado_actual = ESTADO_SIN_CONFIGURAR;
static bool alarma_sonando = false;
static bool punto_segundos_encendido = false;

static uint8_t hora_ajuste[6] = {0, 0, 0, 0, 0, 0};
static uint8_t display_buffer[4] = {0, 0, 0, 0};

static uint32_t cont_f1 = 0;
static uint32_t cont_f2 = 0;
static uint32_t cont_inactividad = 0;
static uint32_t cont_ticks = 0;

// Callback disparado automáticamente por el reloj al coincidir la hora
static void OnAlarmTrigger(clock_t reloj) {
    alarma_sonando = true;
}

void AppInit(board_t board) {
    board_app = board;
    reloj_app = RelojCreate(TICKS_POR_SEGUNDO, OnAlarmTrigger);

    // Estado inicial: Titilan todos los dígitos
    DisplayFlashDigits(board_app->display, 0, 3, TICKS_POR_SEGUNDO / 2);
}

void AppTick(void) {
    ClockTick(reloj_app);
    DisplayRefresh(board_app->display);

    cont_ticks++;
    if (cont_ticks >= TICKS_POR_SEGUNDO) {
        cont_ticks = 0;
        if (estado_actual == ESTADO_NORMAL) {
            punto_segundos_encendido = !punto_segundos_encendido;
        } else {
            punto_segundos_encendido = false;
        }
    }
}

void AppTask(void) {
    hora_t hora_actual;
    GetCurrentTime(reloj_app, hora_actual);

    bool tecla_aceptar = DigitalInputHasActivate(board_app->accept);
    bool tecla_cancelar = DigitalInputHasActivate(board_app->cancel);

    switch (estado_actual) {

        case ESTADO_SIN_CONFIGURAR:
            memset(display_buffer, 0, sizeof(display_buffer));
            DisplayWriteBCD(board_app->display, display_buffer, 4);

            if (DigitalInputRead(board_app->f1)) {
                if (++cont_f1 >= TIEMPO_3_SEGUNDOS) {
                    cont_f1 = 0;
                    estado_actual = ESTADO_AJUSTE_HORA_MIN;
                    DisplayFlashDigits(board_app->display, 2, 3, TICKS_POR_SEGUNDO / 4);
                    cont_inactividad = 0;
                }
            } else { cont_f1 = 0; }
            break;

        case ESTADO_NORMAL:
            display_buffer[0] = hora_actual[0]; display_buffer[1] = hora_actual[1];
            display_buffer[2] = hora_actual[2]; display_buffer[3] = hora_actual[3];
            DisplayWriteBCD(board_app->display, display_buffer, 4);

            if (punto_segundos_encendido) {
                DisplayToggleDots(board_app->display, 1, 1);
            }

            if (IsAlarmEnabled(reloj_app)) {
                DisplayToggleDots(board_app->display, 0, 0);
            }

            if (DigitalInputRead(board_app->f1)) {
                if (++cont_f1 >= TIEMPO_3_SEGUNDOS) {
                    cont_f1 = 0;
                    GetCurrentTime(reloj_app, hora_ajuste);
                    estado_actual = ESTADO_AJUSTE_HORA_MIN;
                    DisplayFlashDigits(board_app->display, 2, 3, TICKS_POR_SEGUNDO / 4);
                    cont_inactividad = 0;
                }
            } else { cont_f1 = 0; }

            if (DigitalInputRead(board_app->f2)) {
                if (++cont_f2 >= TIEMPO_3_SEGUNDOS) {
                    cont_f2 = 0;
                    GetAlarmTime(reloj_app, hora_ajuste);
                    estado_actual = ESTADO_AJUSTE_ALARMA_MIN;
                    DisplayFlashDigits(board_app->display, 2, 3, TICKS_POR_SEGUNDO / 4);
                    cont_inactividad = 0;
                }
            } else { cont_f2 = 0; }

            if (tecla_aceptar && !alarma_sonando) {
                SetAlarmEnabled(reloj_app, true);
            }
            if (tecla_cancelar && !alarma_sonando) {
                SetAlarmEnabled(reloj_app, false);
            }
            break;

        case ESTADO_AJUSTE_HORA_MIN:
            cont_inactividad++;
            display_buffer[0] = hora_ajuste[0]; display_buffer[1] = hora_ajuste[1];
            display_buffer[2] = hora_ajuste[2]; display_buffer[3] = hora_ajuste[3];
            DisplayWriteBCD(board_app->display, display_buffer, 4);

            if (DigitalInputHasActivate(board_app->f4)) {
                cont_inactividad = 0;
                hora_ajuste[3]++;
                if (hora_ajuste[3] > 9) { hora_ajuste[3] = 0; hora_ajuste[2]++; }
                if (hora_ajuste[2] > 5) { hora_ajuste[2] = 0; }
            }
            if (DigitalInputHasActivate(board_app->f3)) {
                cont_inactividad = 0;
                if (hora_ajuste[3] == 0) {
                    hora_ajuste[3] = 9;
                    if (hora_ajuste[2] == 0) { hora_ajuste[2] = 5; } else { hora_ajuste[2]--; }
                } else { hora_ajuste[3]--; }
            }

            if (tecla_aceptar) {
                estado_actual = ESTADO_AJUSTE_HORA_HOR;
                DisplayFlashDigits(board_app->display, 0, 1, TICKS_POR_SEGUNDO / 4);
                cont_inactividad = 0;
            }
            if (tecla_cancelar || cont_inactividad >= TIEMPO_30_SEGUNDOS) {
                DisplayFlashDigits(board_app->display, 0, 0, 0);
                estado_actual = ESTADO_NORMAL;
            }
            break;

        case ESTADO_AJUSTE_HORA_HOR:
            cont_inactividad++;
            display_buffer[0] = hora_ajuste[0]; display_buffer[1] = hora_ajuste[1];
            display_buffer[2] = hora_ajuste[2]; display_buffer[3] = hora_ajuste[3];
            DisplayWriteBCD(board_app->display, display_buffer, 4);

            if (DigitalInputHasActivate(board_app->f4)) {
                cont_inactividad = 0;
                hora_ajuste[1]++;
                if (hora_ajuste[1] > 9) { hora_ajuste[1] = 0; hora_ajuste[0]++; }
                if (hora_ajuste[0] == 2 && hora_ajuste[1] > 3) { hora_ajuste[0] = 0; hora_ajuste[1] = 0; }
            }
            if (DigitalInputHasActivate(board_app->f3)) {
                cont_inactividad = 0;
                if (hora_ajuste[1] == 0) {
                    if (hora_ajuste[0] == 0) { hora_ajuste[0] = 2; hora_ajuste[1] = 3; } 
                    else { hora_ajuste[0]--; hora_ajuste[1] = 9; }
                } else { hora_ajuste[1]--; }
            }

            if (tecla_aceptar) {
                SetCurrentTime(reloj_app, hora_ajuste);
                DisplayFlashDigits(board_app->display, 0, 0, 0);
                estado_actual = ESTADO_NORMAL;
            }
            if (tecla_cancelar || cont_inactividad >= TIEMPO_30_SEGUNDOS) {
                DisplayFlashDigits(board_app->display, 0, 0, 0);
                estado_actual = ESTADO_NORMAL;
            }
            break;

        case ESTADO_AJUSTE_ALARMA_MIN:
            cont_inactividad++;
            display_buffer[0] = hora_ajuste[0]; display_buffer[1] = hora_ajuste[1];
            display_buffer[2] = hora_ajuste[2]; display_buffer[3] = hora_ajuste[3];
            DisplayWriteBCD(board_app->display, display_buffer, 4);
            DisplayToggleDots(board_app->display, 0, 3);

            if (DigitalInputHasActivate(board_app->f4)) {
                cont_inactividad = 0;
                hora_ajuste[3]++;
                if (hora_ajuste[3] > 9) { hora_ajuste[3] = 0; hora_ajuste[2]++; }
                if (hora_ajuste[2] > 5) { hora_ajuste[2] = 0; }
            }
            if (DigitalInputHasActivate(board_app->f3)) {
                cont_inactividad = 0;
                if (hora_ajuste[3] == 0) {
                    hora_ajuste[3] = 9;
                    if (hora_ajuste[2] == 0) { hora_ajuste[2] = 5; } else { hora_ajuste[2]--; }
                } else { hora_ajuste[3]--; }
            }

            if (tecla_aceptar) {
                estado_actual = ESTADO_AJUSTE_ALARMA_HOR;
                DisplayFlashDigits(board_app->display, 0, 1, TICKS_POR_SEGUNDO / 4);
                cont_inactividad = 0;
            }
            if (tecla_cancelar || cont_inactividad >= TIEMPO_30_SEGUNDOS) {
                DisplayFlashDigits(board_app->display, 0, 0, 0);
                estado_actual = ESTADO_NORMAL;
            }
            break;

        case ESTADO_AJUSTE_ALARMA_HOR:
            cont_inactividad++;
            display_buffer[0] = hora_ajuste[0]; display_buffer[1] = hora_ajuste[1];
            display_buffer[2] = hora_ajuste[2]; display_buffer[3] = hora_ajuste[3];
            DisplayWriteBCD(board_app->display, display_buffer, 4);
            DisplayToggleDots(board_app->display, 0, 3);

            if (DigitalInputHasActivate(board_app->f4)) {
                cont_inactividad = 0;
                hora_ajuste[1]++;
                if (hora_ajuste[1] > 9) { hora_ajuste[1] = 0; hora_ajuste[0]++; }
                if (hora_ajuste[0] == 2 && hora_ajuste[1] > 3) { hora_ajuste[0] = 0; hora_ajuste[1] = 0; }
            }
            if (DigitalInputHasActivate(board_app->f3)) {
                cont_inactividad = 0;
                if (hora_ajuste[1] == 0) {
                    if (hora_ajuste[0] == 0) { hora_ajuste[0] = 2; hora_ajuste[1] = 3; }
                    else { hora_ajuste[0]--; hora_ajuste[1] = 9; }
                } else { hora_ajuste[1]--; }
            }

            if (tecla_aceptar) {
                SetAlarmTime(reloj_app, hora_ajuste);
                DisplayFlashDigits(board_app->display, 0, 0, 0);
                estado_actual = ESTADO_NORMAL;
            }
            if (tecla_cancelar || cont_inactividad >= TIEMPO_30_SEGUNDOS) {
                DisplayFlashDigits(board_app->display, 0, 0, 0);
                estado_actual = ESTADO_NORMAL;
            }
            break;

        default:
            break;
    }

    // Control del Buzzer y Posponer Alarma
    if (alarma_sonando) {
        DigitalOutputActivate(board_app->buzzer);

        if (tecla_aceptar) {
            PostponeAlarm(reloj_app, 5);
            alarma_sonando = false;
            DigitalOutputDeactivate(board_app->buzzer);
        }

        if (tecla_cancelar) {
            alarma_sonando = false;
            DigitalOutputDeactivate(board_app->buzzer);
        }
    } else {
        DigitalOutputDeactivate(board_app->buzzer);
    }
}