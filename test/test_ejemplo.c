// test_modbus_rtu.c
#include "unity.h"


#include "esp_base.h"
#include "modbus_rtu.h"
#include "mock_esp_uart.h"


modbus_request_t req = {0};


void setUp(void) {
    // Código que se ejecuta antes de cada test (si es necesario)
}

void tearDown(void) {
    // Código que se ejecuta después de cada test (si es necesario)
}

void test_modbus_init_error(void){
 esp_error_t err = ESP_OK;
    uart_init_ExpectAndReturn(UART_NUM_0, ESP_ERROR);  // Esperar que uart_init devuelva un error.
    err = modbus_rtu_init(UART_NUM_0);  // Llama a modbus_rtu_init con UART_NUM_0.
    TEST_ASSERT_EQUAL(ESP_ERROR, err);   // Verifica que el error sea manejado correctamente.

}


void test_modbus_init_ok(void){
    esp_error_t err = ESP_ERROR;
    uart_init_ExpectAndReturn(UART_NUM_0, ESP_OK);  // Esperar que uart_init devuelva un error.
    err = modbus_rtu_init(UART_NUM_0);  // Llama a modbus_rtu_init con UART_NUM_0.
    TEST_ASSERT_EQUAL(ESP_OK, err);   // Verifica que el error sea manejado correctamente.
}


void test_modbus_send_req_successed(void){

    #define REG_SIZE    10
    #define BUFFER_LEN  (REG_SIZE )    // Los registros son de 2bytes cada uno
    uint8_t buffer[BUFFER_LEN];

    modbus_request_t req = {
        .command = 0x03,
        .slave = 0x01,
        .reg_start = 0,
        .reg_size = REG_SIZE
    };
    esp_error_t err = ESP_ERROR;
    #define NUM_BYTES_REQUEST   8

    // ExpectAnyArgsAndReturn  significa que no me interesa los parametros que reciba 
    // la funcion mock, quiero controlar que es lo que retorna
    // Las peticiones siempre tienen 8 bytes
    uart_write_ExpectAnyArgsAndReturn(NUM_BYTES_REQUEST);
    // Las respuestas dependen de cuantos registros tengas
    uart_read_ExpectAnyArgsAndReturn( 5 + (REG_SIZE* 2));

    err = modbus_rtu_request(&req,buffer,BUFFER_LEN);

    TEST_ASSERT_EQUAL(ESP_OK,err); 
}

