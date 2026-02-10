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

	BNO085_Initialization();
	BNO085_enableRotationVector(2500); //enable rotation vector at 400Hz
}

static __vo uint32_t timer_100ms = 0;
static __vo uint32_t last_100ms = 0;
static __vo uint32_t last_200ms = 0;
static __vo uint32_t last_500ms = 0;
static __vo uint32_t last_1s = 0;
static __vo uint32_t last_5s = 0;

void hwMain(void)
{
  timer_100ms = millis()/100;

  //0.1초
  if(timer_100ms != last_100ms)
  {
    last_100ms = timer_100ms;

    btnMain();
//    bno085Main();
    // 0.2초
    if(timer_100ms - last_200ms >= 2)
    {
      last_200ms = timer_100ms;
//      bno085Main();
    }

    //0.5초
    if(timer_100ms - last_500ms >= 5)
    {
      last_500ms = timer_100ms;

    }

    //1초
    if(timer_100ms - last_1s >= 10)
    {
      last_1s = timer_100ms;

//      bno085Main();
    }

    //5초
    if(timer_100ms - last_5s >= 50)
    {
      last_5s = timer_100ms;
    }

  }
	cliMain();
	BNO085_Main();
}
