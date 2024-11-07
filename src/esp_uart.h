#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include "esp_base.h"





esp_error_t  uart_init(uart_num_t uart);


esp_error_t uart_deinit( uart_num_t uart);


size_t uart_write(uart_num_t uart, uint8_t *buf, size_t nbyte);


size_t uart_read(uart_num_t uart, uint8_t *buf, size_t nbyte);










#endif