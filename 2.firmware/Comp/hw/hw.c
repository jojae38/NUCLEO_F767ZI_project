/*
 * hw.c
 *
 *  Created on: Jan 27, 2026
 *      Author: jojae
 */


#include "hw.h"

void hwInit(void)
{


  ledInit();
  btnInit();
	cliInit();
	bno085Init();
}

void hwMain(void)
{
  btnMain();
	cliMain();
	bno085Main();
}
