/*
 * STM32F103ZETB button.c
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
#include "button.h"
#include "usart.h"
#include "delay.h" 
/*----------------------------------------------------
*	beep_init()
* description: to initialize button port configuration
*	
* in param	 : None
* out param	 : None
*----------------------------------------------------
*/
void button_init(void)
{
	int cnt;
	printf("beep_init()........\r\n");
	for(cnt=0; cnt < BUTTON_NUM; cnt++) {
		gpio_init(button_configuration[cnt].bank, button_configuration[cnt].num,
							button_configuration[cnt].mode, button_configuration[cnt].cfg);
	}
	/*
	*	here to enable pull-up for KEY0 & KEY1 button pins
	*/
	GPIOE->ODR |= 3<<3;
}
/*----------------------------------------------------
*	button_check()
* description: to do button check
*	
* in param	 : None
* out param	 : return ret, button value
*----------------------------------------------------
*/
int button_check(void)
{
	int ret = 0;
	if((BUTTON_WAKEUP == 1) || (BUTTON_KEY0 == 0)  || (BUTTON_KEY1 == 0)) {
		printf("button_check(), get one button event!\r\n");
		delay_ms(10); //button debounce
		if(BUTTON_WAKEUP == 1) {
			printf("button_check(), wake-up button event!\r\n");
			ret = WAKEUP_VALUE;
		}
		if(BUTTON_KEY0 == 0) {
			printf("button_check(), key0 button event!\r\n");
			ret = KEY0_VALUE;
		}	
		if(BUTTON_KEY1 == 0) {
			printf("button_check(), key1 button event!\r\n");
			ret = KEY1_VALUE;
		}			
	}
	else {
		delay_ms(50); //button debounce
	}
	return ret;
}
