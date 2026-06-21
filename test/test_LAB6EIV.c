#include "unity.h"
#include "reloj.h"
#include <stdbool.h>  
#include <stdint.h>

static const hora_t INITIAL_TIME = {0, 0, 0, 0, 0, 0};
//Al iniciar el reloj esta en 00:00 y con hora invsalida
void test_reloj_inicial_invalido(void){
    clock_t reloj;  
    hora_t hora_actual = {1, 2, 3, 4, 5, 6};
    bool es_valida;

    reloj = RelojCreate(1 ,NULL);
    es_valida = RelojGetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_FALSE(es_valida);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(INITIAL_TIME, hora_actual, 6);

}