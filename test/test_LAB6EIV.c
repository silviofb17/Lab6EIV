#include "unity.h"
#include "reloj.h"
#include <stdbool.h>  
#include <stdint.h>

static const hora_t INITIAL_TIME = {0, 0, 0, 0, 0, 0};
//TEST 1- Al iniciar el reloj esta en 00:00 y con hora invsalida
void test_reloj_inicial_invalido(void){
    clock_t reloj;  
    hora_t hora_actual = {1, 2, 3, 4, 5, 6};
    bool es_valida;

    reloj = RelojCreate(1 ,NULL);
    es_valida = GetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_FALSE(es_valida);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(INITIAL_TIME, hora_actual, 6);
}

// TEST 2- Al ajustar la hora, el reloj se configura correctamente y pasa a ser válida.
void test_ajustar_hora_valida(void) {
    clock_t reloj;      //
    hora_t hora_actual; //
    bool es_valida;

    // 12:34:56 en formato BCD sin compactar
    uint8_t hora_a_ajustar[6] = {1, 2, 3, 4, 5, 6}; 
    uint8_t hora_esperada[6]  = {1, 2, 3, 4, 5, 6};

    reloj = RelojCreate(1, NULL); //

    // Llamamos a la función de ajuste (va a tirar error porque todavía no existe en reloj.h)
    bool ajuste_exitoso = SetCurrentTime(reloj, hora_a_ajustar);
    TEST_ASSERT_TRUE(ajuste_exitoso);

    // Consultamos si la hora actual cambió y si ahora es válida
    es_valida = GetCurrentTime(reloj, hora_actual); //
    
    TEST_ASSERT_TRUE(es_valida);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(hora_esperada, hora_actual, 6);
}

// TEST 3- El reloj avanza un segundo al recibir la cantidad configurada de ticks.
void test_reloj_avanza_un_segundo(void) {
    clock_t reloj;
    hora_t hora_actual;

    uint8_t hora_inicial[6]  = {1, 2, 0, 0, 0, 0}; // 12:00:00
    uint8_t hora_esperada[6] = {1, 2, 0, 0, 0, 1}; // 12:00:01

    // Creamos el reloj configurado para avanzar 1 segundo cada 5 ticks
    reloj = RelojCreate(5, NULL);
    SetCurrentTime(reloj, hora_inicial);

    // Simulamos 4 ticks: la hora NO debería cambiar todavía
    for (int i = 0; i < 4; i++) {
        ClockTick(reloj);
    }
    GetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(hora_inicial, hora_actual, 6);

    ClockTick(reloj);
    GetCurrentTime(reloj, hora_actual);
    
    TEST_ASSERT_EQUAL_UINT8_ARRAY(hora_esperada, hora_actual, 6);
}

// TEST 4- El reloj avanza un minuto completo de manera correcta (00:00:59 -> 00:01:00)
void test_reloj_avanza_un_minuto(void) {
    clock_t reloj;
    hora_t hora_actual;

    uint8_t hora_inicial[6]  = {0, 0, 0, 0, 5, 9}; // 00:00:59
    uint8_t hora_esperada[6] = {0, 0, 0, 1, 0, 0}; // 00:01:00

    // Configurado a 1 tick por segundo para hacerlo simple
    reloj = RelojCreate(1, NULL);
    SetCurrentTime(reloj, hora_inicial);

    // Un solo tick debería desbordan los segundos e incrementar el minuto
    ClockTick(reloj);
    GetCurrentTime(reloj, hora_actual);
    
    TEST_ASSERT_EQUAL_UINT8_ARRAY(hora_esperada, hora_actual, 6);
}

// TEST 5- El reloj se resetea correctamente a la medianoche (23:59:59 -> 00:00:00)
void test_reloj_resetea_a_medianoche(void) {
    clock_t reloj;
    hora_t hora_actual;

    uint8_t hora_inicial[6]  = {2, 3, 5, 9, 5, 9}; // 23:59:59
    uint8_t hora_esperada[6] = {0, 0, 0, 0, 0, 0}; // 00:00:00

    reloj = RelojCreate(1, NULL);
    SetCurrentTime(reloj, hora_inicial);

    // Un tick extra debería disparar el desborde completo hasta las 00:00:00
    ClockTick(reloj);
    GetCurrentTime(reloj, hora_actual);
    
    TEST_ASSERT_EQUAL_UINT8_ARRAY(hora_esperada, hora_actual, 6);
}

// TEST 6- Se puede fijar la hora de la alarma y consultarla correctamente.
void test_fijar_y_consultar_alarma(void) {
    clock_t reloj;
    hora_t alarma_consultada;

    uint8_t alarma_a_fijar[6]  = {0, 7, 3, 0, 0, 0}; // 07:30:00
    uint8_t alarma_esperada[6] = {0, 7, 3, 0, 0, 0};

    reloj = RelojCreate(1, NULL);

    // Fijamos la alarma (va a fallar la compilación porque no existe la función)
    bool set_ok = SetAlarmTime(reloj, alarma_a_fijar);
    TEST_ASSERT_TRUE(set_ok);

    // Consultamos la alarma para verificar que coincida
    bool get_ok = GetAlarmTime(reloj, alarma_consultada);
    TEST_ASSERT_TRUE(get_ok);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(alarma_esperada, alarma_consultada, 6);
}

// TEST 7- Se puede habilitar y deshabilitar la alarma, y consultar su estado correctamente.
void test_habilitar_y_deshabilitar_alarma(void) {
    clock_t reloj;

    reloj = RelojCreate(1, NULL);

    // Por defecto, la alarma debería iniciar deshabilitada
    TEST_ASSERT_FALSE(IsAlarmEnabled(reloj));

    // La habilitamos y comprobamos
    SetAlarmEnabled(reloj, true);
    TEST_ASSERT_TRUE(IsAlarmEnabled(reloj));

    // La deshabilitamos y comprobamos
    SetAlarmEnabled(reloj, false);
    TEST_ASSERT_FALSE(IsAlarmEnabled(reloj));
}