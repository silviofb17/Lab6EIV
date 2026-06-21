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