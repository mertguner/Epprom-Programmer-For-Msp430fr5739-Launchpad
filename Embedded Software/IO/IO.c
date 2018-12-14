#include <msp430.h>
#include "IO.h"
#include "../headers/GenelTanimlar.h"

void IO_Init()
{
	PinDefault();

	//-------------------------------- Ledler 3 Adet --------------------------------//
	PinLow(P1OUT,BIT0 | BIT1);
    PinOutput(P1DIR, BIT0 | BIT1);
    /*-------------------------------- Ledler 3 Adet --------------------------------*/

}

