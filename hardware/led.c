/*
 * STM32F103ZETB led.c
 *
 * Author:      David Yang, <yangxiaohua2009@outlook.com>
 * Copyright:   (C) 2020 David Yang
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Notes:
 *			2020-01-04, rev00, initial
 *
 */
#include "led.h"
#include "usart.h"
/*----------------------------------------------------
* led_init()
*	description: to initialize led port configuration
*	
* in param	 : None
* out param	 : None
*----------------------------------------------------
*/
void led_init(void)
{
	int cnt;
	printf("led_init()........\r\n");
	for(cnt=0; cnt < LED_NUM; cnt++) {
		gpio_init(led_configuration[cnt].bank, led_configuration[cnt].num,
							led_configuration[cnt].mode, led_configuration[cnt].cfg);
	}
}
/*
*----------------------------------------------------
*	LED_FLASH()
* description:
*	let led to flash (on/off)
* import para: int lednum, =0 led 0
*												 	 =1 led 1
*									........,=255, led 255
*							 int status, =0, off, =1, on
*outport para: int ret, = 0
*----------------------------------------------------
*/
int led_flash(int lednum, int status)
{
	if(lednum > LED_NUM) printf("led_flash(), lednum error\r\n");
	switch(lednum) {
		case 0:
			LED0 = status;
			break;
		case 1:
			LED1 = status;
			break;
		default:
			break;
	}
	return 0;
}





