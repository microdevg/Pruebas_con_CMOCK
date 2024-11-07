#include "modbus_rtu.h"
#include "esp_uart.h"
#include "esp_base.h"

#define BUFFER_LEN_INTERN    200
static uint8_t _buffer[BUFFER_LEN_INTERN];



static uart_num_t _modbus_uart = UART_NUM_0;

// Funcion interna
uint16_t modbus_calculate_crc(uint8_t* buffer, uint16_t length) {
    uint16_t crc = 0xFFFF;  // Valor inicial del CRC

    for (uint16_t i = 0; i < length; i++) {
        crc ^= buffer[i];  // XOR entre el byte actual y el CRC

        for (uint8_t j = 0; j < 8; j++) {  // Procesa cada bit del byte actual
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;  // Aplica el polinomio si el bit menos significativo es 1
            } else {
                crc >>= 1;  // Solo realiza un desplazamiento si no hay que aplicar el polinomio
            }
        }
    }

    return crc;  // Retorna el CRC calculado
}




esp_error_t  modbus_rtu_init(uart_num_t num){

    esp_error_t err = uart_init(num);
    _modbus_uart = num;
    return err;
}


esp_error_t modbus_rtu_deinit(uart_num_t num){
    esp_error_t err = uart_deinit(num);

    return ESP_OK;
}




esp_error_t modbus_rtu_request(modbus_request_t* req, uint8_t* buffer, uint16_t len) {
    esp_error_t err = ESP_ERROR;

    // Verificación del tamaño mínimo necesario en el buffer de salida
    if (len < 8) {  // 8 bytes necesarios para dirección, función, start, size y CRC
        return err;
    }

    // Construcción del mensaje Modbus RTU
    buffer[0] = req->slave;                       // Dirección del esclavo
    buffer[1] = req->command;                     // Código de función
    buffer[2] = (req->reg_start >> 8) & 0xFF;     // Dirección de inicio (parte alta)
    buffer[3] = req->reg_start & 0xFF;            // Dirección de inicio (parte baja)
    buffer[4] = (req->reg_size >> 8) & 0xFF;      // Cantidad de registros (parte alta)
    buffer[5] = req->reg_size & 0xFF;             // Cantidad de registros (parte baja)

    // Calcula el CRC y lo coloca en el buffer
    uint16_t crc = modbus_calculate_crc(buffer, 6);  // Calcula CRC sobre los primeros 6 bytes
    buffer[6] = crc & 0xFF;                        // CRC (parte baja)
    buffer[7] = (crc >> 8) & 0xFF;                 // CRC (parte alta)

    // Envía el mensaje a través de UART
    size_t bytes_sent = uart_write(_modbus_uart,buffer, 8);

    // Verifica que todos los bytes se hayan enviado
    if (bytes_sent != 8) {
        return err;
    }

    // Calcular el tamaño esperado de la respuesta:
    // Respuesta: || ADDRESS | FUNCTION | BYTE COUNT | DATA ... | CRC (2 bytes) ||
    uint16_t expected_response_len = 5 + (req->reg_size * 2);  // 5 bytes de encabezado y CRC + datos
    if (expected_response_len > BUFFER_LEN_INTERN) {
        // Error si el tamaño esperado excede el tamaño del buffer interno
        return err;
    }

    // Leer la respuesta de UART
    signed bytes_received = uart_read(_modbus_uart,_buffer, expected_response_len);

    // Verificar si se recibieron los bytes esperados
    if (bytes_received != expected_response_len) {
        return err;
    }

    err = ESP_OK;
    return err;
}
