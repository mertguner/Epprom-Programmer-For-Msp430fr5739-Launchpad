/*
 * txrxlpfi2c.h
 *
 *  Created on: Dec 6, 2011
 *      Author: Administrator
 */

#ifndef TXRXLPFI2C_H_
#define TXRXLPFI2C_H_



#define SDA_PIN BIT6                                  // msp430x261x UCB0SDA pin
#define SCL_PIN BIT7

#define iccRxBufferLeght 45
#define iccTxBufferLeght 45

#define iccDone BIT0
#define TXProses BIT1
#define RXProses BIT2
#define TXRegAdresW BIT3
#define TXRegAdresR BIT4
#define RepeatStart BIT5
#define ReadProsesStart BIT6
#define initStart BIT7
#define init BIT8

void IIC_Init();
void iic_Read(unsigned char Slave_Adress, unsigned char Reg_Adress, unsigned char Count);
void iic_Write(unsigned char Slave_Adress, unsigned char Reg_Adress, unsigned char Count);
void iic_PCA9534_Read();

#endif /* TXRXLPFI2C_H_ */
