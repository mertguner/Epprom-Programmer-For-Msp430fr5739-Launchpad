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
	WDTCTL = WDTPW + WDTHOLD;				// WatchDog Timer Kapalý

	CSCTL0_H = 0xA5;						// CPU Registerýna yazmak için A5h Okumak için 96h Yazýlýr

	PJSEL1 &= ~(BIT4 + BIT5);							// Kristal Pinleri XIN-XOUT Fonksiyonuna göre ayarlanýyor
	PJSEL0 |= (BIT4 + BIT5);				// Kristal Pinleri XIN-XOUT Fonksiyonuna göre ayarlanýyor

	CSCTL1 = DCORSEL + DCOFSEL0 + DCOFSEL1;			// DCO Frekansý 24Mhz
	CSCTL2 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK;
											// ACLK = XT1; SMCLK = DCO; MCLK = DCO
	CSCTL3 = DIVA__1 + DIVS__1 + DIVM__1;	// ACLK = SMCLK = MCLK = XT1/1;
	CSCTL4 = XT1DRIVE_0 + XTS + SMCLKOFF + XT2OFF + XT2BYPASS;// XT2 Kristali ve SMClock Kapalý
	CSCTL5 = 0;
	    do {
	        CSCTL5 &= ~XT1OFFG;
	        SFRIFG1 &= ~OFIFG;
	    } while (SFRIFG1 & OFIFG);          // oscillator Hata Bayraðý Kontrol Ediliyor
	CSCTL6 = 0;



	CSCTL0_H = 0x01;  						// OSC Registerý Kitleniyor



    REFCTL0 |= REFTCOFF;					// Sýcaklýk Sensörü Kapatýlýyor.
	REFCTL0 &= ~(REFON/* + BGMODE*/);		// Ref Regülatör Kapatýlýyor. Sadece ADC Ölçümleri Sýrasýnda Açýlacak


}





