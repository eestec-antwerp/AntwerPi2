
/*
 * A brave attempt at implementing the hwuart.h interface for the Raspberry Pi 3B, with a Navio2 on top.
 * 
 * Author: Evert Heylen
 * 
 */

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

#include "hwuart.h"

struct uart_handle {
  // define stuff you need here
};


uart_handle_t* uart_init(uint8_t channel, uint32_t baudrate, uint8_t pins) {
	
}

bool uart_disable(uart_handle_t* uart) {
	return true;
}

bool uart_enable(uart_handle_t* uart) {
	return false;
}

void uart_send_byte(uart_handle_t* uart, uint8_t data) {
	
}

void uart_send_bytes(uart_handle_t* uart, void const *data, size_t length) {
	
}

void uart_send_string(uart_handle_t* uart, const char *string) {
	
}

error_t uart_rx_interrupt_enable(uart_handle_t* uart) {
	
}

void uart_rx_interrupt_disable(uart_handle_t* uart) {
	
}

void uart_set_rx_interrupt_callback(uart_handle_t* uart, uart_rx_inthandler_t rx_handler) {
	
}
