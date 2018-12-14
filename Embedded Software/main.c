#include <msp430.h> 
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "headers/GenelTanimlar.h"
#include "headers/main.h"
#include "headers/init.h"
#include "IIC/iic.h"
#include "UART/uart.h"
#include "IO/IO.h"


/*-------- EXTERNLER --------*/
extern unsigned char UartTXData[UartTxBufferLeght], UartRXData[UartRxBufferLeght];
extern unsigned char IICTXData[iccTxBufferLeght],IICRXData[iccRxBufferLeght];
extern unsigned int UART_status,iic_status;
/*-------- EXTERNLER --------*/

unsigned int i,Modul_Status = 0,iic_Device_Adress,iic_Adress;

/*
 * main.c
 */


int main(void) {
	IO_Init();
	CPU_Init();
	Uart_Init();
	IIC_Init();
	__enable_interrupt();
	//Led1(1);
	//Led2(2);
	while(1)
	{


		if(iic_status & iccDone)
		{
			iic_status &= ~iccDone;

			if(iic_status & RXProses)
			{
				iic_status &= ~RXProses;

				if(Modul_Status & EEPROM_Read){
					Modul_Status &= ~EEPROM_Read;

					UartTXData[0] = '$';
					UartTXData[1] = 'E';
					UartTXData[2] = 'O';
					UartTXData[3] = ',';
					i = 0;
					do{
						UartTXData[(i*3)+4] = IntToHex[(unsigned int)IICRXData[i]>>4];
						UartTXData[(i*3)+5] = IntToHex[(unsigned int)IICRXData[i]&0x0F];
						UartTXData[(i*3)+6] = ',';
					}while(i++ < 15);

					uart_Write_With_CRC();

					for(i = iccRxBufferLeght; i>0;i--)IICRXData[i] = 0;
				}

			}

			if(iic_status & TXProses)
			{
				iic_status &= ~TXProses;


				if(Modul_Status & EEPROM_Write)
				{
					Modul_Status &= ~EEPROM_Write;
					UartTXData[0] = '$';
					UartTXData[1] = 'Y';
					UartTXData[2] = ',';
					UartTXData[3] = '0';
					UartTXData[4] = ',';
					uart_Write_With_CRC();
				}
			}
		}

//---------------------------------------------------------------------------
		if(UART_status & UART_Tx_Done)
		{
			UART_status &= ~UART_Tx_Done;
			for(i = UartTxBufferLeght; i > 0;i--)UartTXData[i]=0;
		}

		if(UART_status & UART_Rx_Done) //$PUBX,40,GLL,0,0,0,0,0,0*5C\r\n
		{
			UART_status &= ~UART_Rx_Done;
			UCA0IFG = 0;

				if(UartRXData[0] == 'E'){
					if(UartRXData[1] == 'Y')
					{
						Modul_Status |= EEPROM_Write;

						i = (HexToInt[UartRXData[3]-(unsigned int)48] << 4) | HexToInt[UartRXData[4]-(unsigned int)48];
						IICTXData[0] = (HexToInt[UartRXData[6]-(unsigned int)48] << 4) | HexToInt[UartRXData[7]-(unsigned int)48];
						IICTXData[1] = (HexToInt[UartRXData[9]-(unsigned int)48] << 4) | HexToInt[UartRXData[10]-(unsigned int)48];
						IICTXData[2] = (HexToInt[UartRXData[12]-(unsigned int)48] << 4) | HexToInt[UartRXData[13]-(unsigned int)48];
						IICTXData[3] = (HexToInt[UartRXData[15]-(unsigned int)48] << 4) | HexToInt[UartRXData[16]-(unsigned int)48];
						IICTXData[4] = (HexToInt[UartRXData[18]-(unsigned int)48] << 4) | HexToInt[UartRXData[19]-(unsigned int)48];
						IICTXData[5] = (HexToInt[UartRXData[21]-(unsigned int)48] << 4) | HexToInt[UartRXData[22]-(unsigned int)48];
						IICTXData[6] = (HexToInt[UartRXData[24]-(unsigned int)48] << 4) | HexToInt[UartRXData[25]-(unsigned int)48];
						IICTXData[7] = (HexToInt[UartRXData[27]-(unsigned int)48] << 4) | HexToInt[UartRXData[28]-(unsigned int)48];
						//iic_Write(0x50,i,8);
					}
					if(UartRXData[1] == 'O')
					{
						iic_Device_Adress = (HexToInt[UartRXData[3]-(unsigned int)48] << 4) | HexToInt[UartRXData[4]-(unsigned int)48];
						iic_Adress = (HexToInt[UartRXData[6]-(unsigned int)48] << 4) | HexToInt[UartRXData[7]-(unsigned int)48];
						iic_Read(iic_Device_Adress,iic_Adress,16);
						Modul_Status |= EEPROM_Read;
					}
				}



		}


		if(iic_status == 0 && UART_status == 0)__bis_SR_register(LPM_Deger);                                        // Enter LPM0, enable interrupts
		__no_operation();                                                          // For debugger
	}

}
#pragma vector=  PORT1_VECTOR,PORT2_VECTOR,PORT3_VECTOR,TIMER0_B1_VECTOR,TIMER2_B1_VECTOR,TIMER2_B0_VECTOR,TIMER1_B1_VECTOR,TIMER1_B0_VECTOR,TIMER0_A1_VECTOR,DMA_VECTOR,TIMER1_A0_VECTOR,WDT_VECTOR,COMP_D_VECTOR,UNMI_VECTOR,SYSNMI_VECTOR,PORT4_VECTOR,ADC10_VECTOR,RTC_VECTOR
__interrupt void ISR_trap(void)
 {
 }
