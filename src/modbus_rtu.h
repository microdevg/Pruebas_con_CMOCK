#ifndef MODBUS_RTU_H
#define MODBUS_RTU_H

#include <stdint.h>
#include "esp_base.h"







typedef struct {
    int slave;
    int command;
    uint16_t reg_start;
    uint16_t reg_size;

} modbus_request_t;




esp_error_t  modbus_rtu_init(uart_num_t uart);


esp_error_t modbus_rtu_deinit(uart_num_t uart);


esp_error_t modbus_rtu_request(modbus_request_t* req, uint8_t* buffer,uint16_t len);









#endif