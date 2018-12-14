/*
 * GenelTanimlar.c
 *
 *  Created on: 03 Eki 2012
 *      Author: mguner
 */

#include "GenelTanimlar.h"


signed int HW_Mul(volatile signed int A,volatile signed int B)
{
MPYS = A;
OP2 = B;
_nop();
return RESLO;
}


void PinDefault()
{
	/*Port1*/
	P1OUT=0x00; P1DIR = 0x00; P1REN =0xFF; P1SEL0 = 0x00; P1SEL1 = 0x00;
	/*Port2*/
	P2OUT=0x00; P2DIR = 0x00; P2REN =0xFF; P2SEL0 = 0x00; P2SEL1 = 0x00;
	/*Port3*/
	P3OUT=0x00; P3DIR = 0x00; P3REN =0xFF; P3SEL0 = 0x00; P3SEL1 = 0x00;
	/*Port4*/
	P4OUT=0x00; P4DIR = 0x00; P4REN =0xFF; P4SEL0 = 0x00; P4SEL1 = 0x00;

	PJOUT=0x00; PJDIR = 0x00; PJREN =0xFF; PJSEL0 = 0x00; PJSEL1 = 0x00;
}
