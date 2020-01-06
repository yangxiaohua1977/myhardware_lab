/*
 * STM32F103ZETB beep.c
 *
 * Author:      David Yang, <yangxiaohua2009@outlook.com>
 * Copyright:   (C) 2020 David Yang
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Notes:
 *			2020-01-05, rev00, initial
 *
 */
#include "beep.h"
#include "usart.h"
/*----------------------------------------------------
*	beep_init()
* description: to initialize beep port configuration
*	
* in param	 : None
* out param	 : None
*----------------------------------------------------
*/
void beep_init(void)
{
	int cnt;
	printf("beep_init()........\r\n");
	for(cnt=0; cnt < BEEP_NUM; cnt++) {
		gpio_init(beep_configuration[cnt].bank, beep_configuration[cnt].num,
							beep_configuration[cnt].mode, beep_configuration[cnt].cfg);
	}
}
/*
*----------------------------------------------------
*	beep_start()
* description:
*	let led to flash (on/off)
* import para: int lednum, =0 led 0
*												 	 =1 led 1
*									........,=255, led 255
*							 int status, =0, off, =1, on
*outport para: int ret, = 0
*----------------------------------------------------
*/
int beep_start(int beep_num, int status)
{
	if(beep_num > BEEP_NUM) printf("beep_start(), beep_num error\r\n");
	switch(beep_num) {
		case 0:
			BEEP = status;
			break;
		default:
			break;
	}
	return 0;
}
