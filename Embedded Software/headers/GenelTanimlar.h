#include <msp430.h>
//Bit = 0: Port pin is switched to input direction
//Bit = 1: Port pin is switched to output direction
void PinDefault();
signed int HW_Mul(volatile signed int A,volatile signed int B);

#define PinOutput(port,pin){port |= pin;}
#define PinInput(port,pin){port &= ~pin;}

#define PinHigh(port,pin){port |= pin;}
#define PinLow(port,pin){port &= ~pin;}

#define PinResistorEnable(port,pin){port |= pin;}
#define PinResistorDisable(port,pin){port &= ~pin;}

#define PinResistorPullUp(port,pin){port |= pin;}
#define PinResistorPullDown(port,pin){port &= ~pin;}


#define TimeOutStart(a,b) {TA1CTL = TASSEL__ACLK + MC__UP + ID__8 + TAIE + TACLR; TimeOutReg |= a; TA1CCR0 = b;}
#define TimeOutStop() TA1CTL &= ~(MC__UP + TAIE)


#define Led1(a) {if(a)P1OUT |= BIT0; else P1OUT &= ~BIT0;}
#define Led2(a) {if(a)P1OUT |= BIT1; else P1OUT &= ~BIT1;}
#define Led1_Toggle P1OUT ^= BIT0;
#define Led2_Toggle P1OUT ^= BIT1;


#define TRUE 1
#define FALSE 0

#define Fclock 4000000

#define LPM_Deger LPM0_bits


#define EEPROM_Read BIT0
#define EEPROM_Write BIT1



static const unsigned int IntToHex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
static const unsigned int HexToInt[23] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15};
