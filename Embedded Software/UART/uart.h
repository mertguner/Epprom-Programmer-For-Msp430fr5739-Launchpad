/*
 * uart.h
 *
 *  Created on: 13 Mar 2013
 *      Author: Smit
 */

#ifndef UART_H_
#define UART_H_
#define UartRxBufferLeght 100
#define UartTxBufferLeght 100

#define UART_Rx_proses BIT0
#define UART_Tx_Done BIT1
#define UART_Rx_Done BIT2
#define UART_GPS_init BIT3
#define UART_Busy BIT4

void Uart_Init();
void uart_Write_With_CRC();
void uart_Write_Gps_init();


#endif /* UART_H_ */
