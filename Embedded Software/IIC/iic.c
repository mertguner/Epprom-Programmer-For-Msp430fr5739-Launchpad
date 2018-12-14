/*
 * txrxlpfi2c.c
 *
 *  Created on: Dec 5, 2011
 *      Author: Administrator
 */
#include <msp430.h>
#include "iic.h"
#include "../headers/GenelTanimlar.h"

unsigned char IICTXData[iccTxBufferLeght], IICRXData[iccRxBufferLeght];					// Define the Array for the received Data
unsigned char Reg_Adress_Value, Count_Value;
unsigned int iic_status = 0, Iic_HaberlesmeSayici = 0;
extern unsigned char TimeOutReg;

void IIC_Init() {
	UCB0CTLW0 = UCSWRST; 					// Activate the reset status
	UCB0CTLW0 |= UCMODE_3 + UCMST + UCSSEL__ACLK;			// I2C master mode
	UCB0CTLW1 |= UCASTP_2;
	UCB0I2CSA = 0x50;
	UCB0BRW = 0x0032; 						// Set baudrate = ACLK / 80 --> (8MHz/80 = 100kHz)
	P1SEL1 |= (BIT6 | BIT7);							// Configure I2C pins
	UCB0CTLW0 &= ~UCSWRST; 					// Deactivate the reset status
	UCB0IE = 0;
}

void iic_Read(unsigned char Slave_Adress, unsigned char Reg_Adress, unsigned char Count)
{
	IIC_Init();
	Reg_Adress_Value = Reg_Adress;
	Count_Value = Count;
	Iic_HaberlesmeSayici = 0;
	UCB0I2CSA = Slave_Adress;
	UCB0TBCNT = 0;
	UCB0CTLW0 |= UCTR+ UCTXSTT;
	iic_status = TXRegAdresR;
	UCB0IE |= UCTXIE0;
}


void iic_Write(unsigned char Slave_Adress, unsigned char Reg_Adress, unsigned char Count)
{
	IIC_Init();
	Reg_Adress_Value = Reg_Adress;
	UCB0I2CSA = Slave_Adress;
	Iic_HaberlesmeSayici = 0;
	if(Slave_Adress == 0x50)UCB0TBCNT = Count + 1;
	else UCB0TBCNT = Count + 1;
	UCB0CTLW0 |= UCTR + UCTXSTT;
	iic_status = TXRegAdresW;
	UCB0IE |= UCTXIE0 + UCSTPIE;
	//UCB0IFG |= UCTXIFG0;
}

#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR (void)
{

  switch(__even_in_range(UCB0IV,0x1E))
  {
  	  	case 0x00: break; // Vector 0: No interrupts break;
        case 0x02: break; // Vector 2: ALIFG break;
        case 0x04: break; // Vector 4: NACKIFG break;
        case 0x06: break; // Vector 6: STTIFG break;
        case 0x08:
        	UCB0IE = 0;
        	iic_status |= iccDone;
        	__bic_SR_register_on_exit(LPM_Deger);
        	break; // Vector 8: STPIFG break;
        case 0x0a: break; // Vector 10: RXIFG3 break;
        case 0x0c: break; // Vector 14: TXIFG3 break;
        case 0x0e: break; // Vector 16: RXIFG2 break;
        case 0x10: break; // Vector 18: TXIFG2 break;
        case 0x12: break; // Vector 20: RXIFG1 break;
        case 0x14: break; // Vector 22: TXIFG1 break;
        case 0x16:
        	IICRXData[Iic_HaberlesmeSayici++] = UCB0RXBUF;
        	//__delay_cycles(10);
        break; // Vector 24: RXIFG0 break;
        case 0x18:
        	switch(iic_status)
        	{
        	case TXProses:
        		UCB0TXBUF = IICTXData[Iic_HaberlesmeSayici++];
        	break;
        	case RepeatStart:
        		//iic_status &= ~RepeatStart;
        		iic_status = RXProses;
             	UCB0CTLW0 &= ~UCTR;
        		UCB0CTLW0 |= UCTXSTT;
        		UCB0TBCNT = Count_Value;
        		if(UCB0I2CSA == 0x21)UCB0TBCNT+=2;
        		UCB0IE &= ~UCTXIE0;
        		UCB0IE |= UCRXIE0 + UCSTPIE;
        	break;
        	case TXRegAdresR:
        	    //iic_status &= ~TXRegAdresR;
        		iic_status = RepeatStart;
        		UCB0TXBUF = Reg_Adress_Value;
        	break;
        	case TXRegAdresW:
        	    //iic_status &= ~TXRegAdresW;
        	    iic_status = TXProses;
        	    __delay_cycles(500);
        	    UCB0TXBUF = Reg_Adress_Value;
        	break;
        	}
        break; // Vector 26: TXIFG0 break;
        case 0x1a: break; // Vector 28: BCNTIFG break;
        case 0x1c: break; // Vector 30: clock low timeout break;
        case 0x1e: break; // Vector 32: 9th bit break;
        default: break;
  }
}
