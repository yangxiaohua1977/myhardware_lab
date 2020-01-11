/*
 * STM32F103ZETB gpio.c
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
#include "gpio.h"
#include "usart.h"
/*
*	gpio_init()
*	description:	to initialize gpio register
*	in param	 :  int gpio_bank, =0x0A, gpio A
*															 =0x0B, gpio B
*															 =0x0C, gpio C
*															 =0x0D, gpio D
*															 =0x0E, gpio E
*															 =0x0F, gpio F
*															 =0x10, gpio G
*								int gpio_num,  =0 to 0x0F, for gpiox 0 to gpiox 15
*								int gpio_mode, =0x00, input
*															 =0x01, 10MHZ output
*															 =0x02, 2MHZ output		
*															 =0x03, 50MHZ output	
*								int gpio_cfg, for input mode
*															 =0x00, analog input
*															 =0x01, float input
*															 =0x02, pull-up/pull-down input		
*															 =0x03, reserved
*															for output mode
*															 =0x00, push-pull
*															 =0x01, open-drain
*															 =0x02, second function push-pull	
*															 =0x03, second function open-drain	
*	out param	 :	int ret, =0, success, =1, fail
*/
int gpio_init(int gpio_bank, int gpio_num, int gpio_mode, int gpio_cfg)
{
	int num;
	if(gpio_bank < 0x0A || gpio_bank > 0x10) {
		printf("gpio_init(), the value of parameter gpio_bank error.\r\n");
		return 1;
	}
	if(gpio_num > 0x0f) {
		printf("gpio_init(), the value of parameter gpio_num error.\r\n");
		return 1;
	}
	if(gpio_mode > 0x03) {
		printf("gpio_init(), the value of parameter gpio_mode error.\r\n");
		return 1;
	}	
	if(gpio_cfg > 0x03) {
		printf("gpio_init(), the value of parameter gpio_cfg error.\r\n");
		return 1;
	}		

	num = gpio_bank - 0x0A + 2;
	RCC->APB2ENR &= ~(1<<num);
	RCC->APB2ENR |= (1 << num);  //enable peripherals clock
	switch(gpio_bank)
	{
		case 0x0A:   //GPIO A
			num = ((gpio_cfg << 2) | (gpio_mode)) & 0x0f;
			if(gpio_num < 8) {
				GPIOA->CRL &= ~(0x0f << (gpio_num * 4));
				GPIOA->CRL |= num << (gpio_num * 4);
			}
			else {
				gpio_num -= 8;
				GPIOA->CRH &= ~(0x0f << (gpio_num * 4));
				GPIOA->CRH |= num << (gpio_num * 4);
			}	
			break;
		case 0x0B:   //GPIO B
			if(gpio_num < 8) {
				GPIOB->CRL &= ~(0x0f << (gpio_num * 4));
				GPIOB->CRL |= num << (gpio_num * 4);
			}
			else {
				gpio_num -= 8;
				GPIOB->CRH &= ~(0x0f << (gpio_num * 4));
				GPIOB->CRH |= num << (gpio_num * 4);
			}
			break;
		case 0x0C:   //GPIO C
			if(gpio_num < 8) {
				GPIOC->CRL &= ~(0x0f << (gpio_num * 4));
				GPIOC->CRL |= num << (gpio_num * 4);
			}
			else {
				gpio_num -= 8;
				GPIOC->CRH &= ~(0x0f << (gpio_num * 4));
				GPIOC->CRH |= num << (gpio_num * 4);
			}
			break;
		case 0x0D:   //GPIO D
			if(gpio_num < 8) {
				GPIOD->CRL &= ~(0x0f << (gpio_num * 4));
				GPIOD->CRL |= num << (gpio_num * 4);
			}
			else {
				gpio_num -= 8;
				GPIOD->CRH &= ~(0x0f << (gpio_num * 4));
				GPIOD->CRH |= num << (gpio_num * 4);
			}
			break;
		case 0x0E:   //GPIO E
			if(gpio_num < 8) {
				GPIOE->CRL &= ~(0x0f << (gpio_num * 4));
				GPIOE->CRL |= num << (gpio_num * 4);
			}
			else {
				gpio_num -= 8;
				GPIOE->CRH &= ~(0x0f << (gpio_num * 4));
				GPIOE->CRH |= num << (gpio_num * 4);
			}
			break;
		case 0x0F:   //GPIO F
			if(gpio_num < 8) {
				GPIOF->CRL &= ~(0x0f << (gpio_num * 4));
				GPIOF->CRL |= num << (gpio_num * 4);
			}
			else {
				gpio_num -= 8;
				GPIOF->CRH &= ~(0x0f << (gpio_num * 4));
				GPIOF->CRH |= num << (gpio_num * 4);
			}
			break;
		case 0x10:   //GPIO G
			if(gpio_num < 8) {
				GPIOG->CRL &= ~(0x0f << (gpio_num * 4));
				GPIOG->CRL |= num << (gpio_num * 4);
			}
			else {
				gpio_num -= 8;
				GPIOG->CRH &= ~(0x0f << (gpio_num * 4));
				GPIOG->CRH |= num << (gpio_num * 4);
			}
			break;
		default:
			break;
	}
	return 0;
}
