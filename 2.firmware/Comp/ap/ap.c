/*
 * ap.c
 *
 *  Created on: Jan 27, 2026
 *      Author: jojae
 */
#include "ap.h"

void apInit(void)
{
	hwInit();
}

void apMain(void)
{
	while(true)
	{
		hwMain();

		if(getBtnState(0) > 200)
		{
		  resetBtnState(0);
		  ledToggle(0);
		}
	}
}
