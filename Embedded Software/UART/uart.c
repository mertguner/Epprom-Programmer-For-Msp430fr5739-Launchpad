/*
 * uart.c
 *
 *  Created on: 13 Mar 2013
 *      Author: Smit
 */
#include <msp430.h>
#include "uart.h"
#include "../headers/GenelTanimlar.h"




//--------------externler--------------
unsigned char UartTXData[UartTxBufferLeght], UartRXData[UartRxBufferLeght];
unsigned int UART_status = 0;
//-------------------------------------
unsigned char CRCGeldi;
unsigned int SerialTxCount=0, SerialRxCount=0, Buffer, Hesaplanan_CRC, Gelen_CRC;


void Uart_Init()
{
	P2SEL1 |= BIT0 + BIT1;
	P2SEL0 &= ~(BIT0 + BIT1);

	UCA0CTLW0 = UCSWRST;                    // Modülü Kapat

	UCA0CTLW0 |= UCSSEL__ACLK;				// UCSSEL = 01 olduðunda UCBRCLK = ACLK

	UCA0BR0 = 26;							// 9600 için
	UCA0BR1 = 0;							// 9600 için  >  Yukarýdaki Tablodan Bakýldý
	UCA0IFG = 0;
	UCA0MCTLW = 0xB600 /*| UCBRF_8*/ | UCOS16;	// 38400 için
	UCA0CTL1 &= ~UCSWRST;    				// Modülü Aç
	UCA0IE = UCRXIE + UCTXCPTIE + UCTXIE;	// RX,TX Buffer Dolu,TX interrupt
	UCA0IFG = 0;
}


void uart_Write_With_CRC()
{
	Hesaplanan_CRC = 0x00;
	SerialTxCount=1;
	do{
		Hesaplanan_CRC ^= UartTXData[SerialTxCount++];
	}while(UartTXData[SerialTxCount] != 0);

	UartTXData[SerialTxCount] = '*';
	UartTXData[SerialTxCount+1] = IntToHex[Hesaplanan_CRC >> 4];
	UartTXData[SerialTxCount+2] = IntToHex[Hesaplanan_CRC & 0x0F];
	UartTXData[SerialTxCount+3] = '\r';
	UartTXData[SerialTxCount+4] = '\n';
	UartTXData[SerialTxCount+5] = '\0';

	UCA0IFG = 0;
	SerialTxCount = 1;
	UCA0TXBUF = UartTXData[0];
}


#pragma vector = USCI_A0_VECTOR //UART
__interrupt void USCI_A0_ISR(void)
{
	switch(__even_in_range(UCA0IV,0x08))
	{
		case 0:break;                             // Vector 0 - no interrupt
		case 2:// Vector 2 - RXIFG
					if(UART_status & UART_Rx_Done || UCA0RXBUF == 0)break;
					Buffer = UCA0RXBUF;

					switch(Buffer)
					{
						case '$':
							UART_status |= UART_Rx_proses;
							UART_status &= ~UART_Rx_Done;
							CRCGeldi = SerialRxCount = 0;
							Hesaplanan_CRC = 0x00;
						break;
						case '\r':
						case '\n':
							if(SerialRxCount == 0 || !(UART_status & UART_Rx_proses) || CRCGeldi != 3)break;
							UART_status &= ~UART_Rx_proses;
							if(Hesaplanan_CRC == Gelen_CRC)
							{
								UART_status |= UART_Rx_Done;
								__bic_SR_register_on_exit(LPM_Deger);
								UartRXData[SerialRxCount] = '\0';
							}
						break;

						case '*':
							CRCGeldi = 1;
						break;

						default:
							if(UART_status & UART_Rx_proses)
							{
								switch(CRCGeldi)
								{
									case 0:
										Hesaplanan_CRC ^= Buffer;
										UartRXData[SerialRxCount++] = Buffer;
									break;
									case 1:
										if(Buffer > 47 && Buffer < 71) Gelen_CRC = (HexToInt[Buffer-48] << 4);
										else UART_status &= ~UART_Rx_proses;
										CRCGeldi = 2;
									break;
									case 2:
										if(Buffer > 47 && Buffer < 71) Gelen_CRC |= HexToInt[Buffer-48];
										else UART_status &= ~UART_Rx_proses;
										CRCGeldi = 3;
									break;
									default:
										UART_status &= ~UART_Rx_proses;
									break;
								}
							}
						break;
					}
					UCA0IFG &= ~UCRXIFG;

				break;

		case 4:
			if(UartTXData[SerialTxCount] != '\0') UCA0TXBUF = UartTXData[SerialTxCount++];
			UCA0IFG &= ~UCTXIFG;
		break;// Vector 4 - TXIFG

		case 8:
			if(UartTXData[SerialTxCount] == '\0') UART_status |= UART_Tx_Done;
			UCA0IFG &= ~BIT8;
	    break;

		default:
		break;
	}

}
