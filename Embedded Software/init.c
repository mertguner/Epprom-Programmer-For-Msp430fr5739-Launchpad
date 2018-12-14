/*
 * init.c
 *
 *  Created on: 18 Ara 2012
 *      Author: Smit
 */
#include <msp430.h>
#include "headers/init.h"
#include "headers/GenelTanimlar.h"


void CPU_Init()
{
	WDTCTL = WDTPW + WDTHOLD;				// WatchDog Timer Kapal�

	CSCTL0_H = 0xA5;						// CPU Register�na yazmak i�in A5h Okumak i�in 96h Yaz�l�r

	PJSEL1 &= ~(BIT4 + BIT5);							// Kristal Pinleri XIN-XOUT Fonksiyonuna g�re ayarlan�yor
	PJSEL0 |= (BIT4 + BIT5);				// Kristal Pinleri XIN-XOUT Fonksiyonuna g�re ayarlan�yor

	CSCTL1 = DCORSEL + DCOFSEL0 + DCOFSEL1;			// DCO Frekans� 24Mhz
	CSCTL2 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK;
											// ACLK = XT1; SMCLK = DCO; MCLK = DCO
	CSCTL3 = DIVA__1 + DIVS__1 + DIVM__1;	// ACLK = SMCLK = MCLK = XT1/1;
	CSCTL4 = XT1DRIVE_0 + XTS + SMCLKOFF + XT2OFF + XT2BYPASS;// XT2 Kristali ve SMClock Kapal�
	CSCTL5 = 0;
	    do {
	        CSCTL5 &= ~XT1OFFG;
	        SFRIFG1 &= ~OFIFG;
	    } while (SFRIFG1 & OFIFG);          // oscillator Hata Bayra�� Kontrol Ediliyor
	CSCTL6 = 0;



	CSCTL0_H = 0x01;  						// OSC Register� Kitleniyor



    REFCTL0 |= REFTCOFF;					// S�cakl�k Sens�r� Kapat�l�yor.
	REFCTL0 &= ~(REFON/* + BGMODE*/);		// Ref Reg�lat�r Kapat�l�yor. Sadece ADC �l��mleri S�ras�nda A��lacak


}





