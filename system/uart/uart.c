/*
 * STM32F103ZETB uart.c
 *
 * Author:      David Yang, <yangxiaohua2009@outlook.com>
 * Copyright:   (C) 2020 David Yang
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Notes:
 *			2020-01-10, rev00, initial
 *
 */
#include "uart.h"
#include "gpio.h"

static const struct GPIO_CFG uart1_gpio_configuration[4] = {
	{0x0A, 0x09, OUT_MODE_50MHZ, PUSH_PULL},   //uart1 tx, PA9
	{0x0A, 0x0A, INPUT_MODE, PUSH_PULL},			 //uart1 rx, PA10
	{0x0B, 0x06, OUT_MODE_50MHZ, PUSH_PULL},   //uart1 tx, PB6
	{0x0B, 0x07, INPUT_MODE, PUSH_PULL},			 //uart1 rx, PB7
};
int uart_initialize(void *p)
{
	struct UART_CFG *uart_struct;
	int baudrate_m, baudrate_s;
	float tmp;
	
	uart_struct = p;
	/*
	*	Check the pclk frequency 
	*/
	if((uart_struct->uart_num != 1) && uart_struct->pclk_freq > 36 ) {
		return -1;
	}
	/*
	* baudrate
	*/
	tmp = (float)(uart_struct->pclk_freq * 1000000) / (16 * uart_struct->baudrate);
	baudrate_m = (int)tmp;
	baudrate_s = (tmp - baudrate_m) * 16;
	baudrate_m = baudrate_m << 4;
	baudrate_m += baudrate_s;
	
	switch(uart_struct->uart_num) {
		case 1:  //uart1
			// check pin remap or not
			if(uart_struct->pin_remap == 0) {
				AFIO->MAPR &= ~0x04;
				
				gpio_init(uart1_gpio_configuration[0].bank, uart1_gpio_configuration[0].num,
									uart1_gpio_configuration[0].mode, uart1_gpio_configuration[0].cfg);
				gpio_init(uart1_gpio_configuration[1].bank, uart1_gpio_configuration[1].num,
									uart1_gpio_configuration[1].mode, uart1_gpio_configuration[1].cfg);				
			}
			else {
				AFIO->MAPR &= ~0x04;
				AFIO->MAPR |= 0x04;
				
				gpio_init(uart1_gpio_configuration[2].bank, uart1_gpio_configuration[2].num,
									uart1_gpio_configuration[2].mode, uart1_gpio_configuration[2].cfg);
				gpio_init(uart1_gpio_configuration[3].bank, uart1_gpio_configuration[3].num,
									uart1_gpio_configuration[3].mode, uart1_gpio_configuration[3].cfg);		 
			}
			RCC->APB2RSTR |= 1 << 14;   // reset uart1
			RCC->APB2RSTR &= ~(1 << 14);
			
			RCC->APB2ENR|=1<<14;  //enable uart1 clock
			USART1->BRR = baudrate_m;		// set baudrate
			
			//set uart1 data mode
			USART1->CR1 &= ~0x01000;
			if(uart_struct->uart_work.data_mode == 1) { //start +9dat+nstop
				USART1->CR1 |= 0x01000;
			}
			//stop bits
			USART1->CR2 &= ~0x3000;
			switch(uart_struct->uart_work.stop_bit) {
				case ONE_STOP_BIT:
					break;
				case HALF_STOP_BIT:
					USART1->CR2 |= 0x1000;
					break;	
				case TWO_STOP_BIT:
					USART1->CR2 |= 0x2000;
					break;
				case ONEHALF_STOP_BIT:
					USART1->CR2 |= 0x3000;
					break;	
				default:
					break;
			}
			//half duplex or duplex
			USART1->CR3 &= ~0x0008;
			if(uart_struct->uart_work.half_duplex == 1) {
				USART1->CR3 |= 0x0008;
			}
			//wake mode
			USART1->CR1 &= ~0x0800;
			if(uart_struct->uart_work.wake_mode == 1) {
				USART1->CR1 |= 0x0800;
			}
			//receive wakeup
			USART1->CR1 &= ~0x02;
			if(uart_struct->uart_work.rec_wakeup == 1) {
				USART1->CR1 |= 0x02;
			}
			//parity check
			USART1->CR1 &= ~0x200;
			if(uart_struct->uart_work.parity_select == 1) {
				USART1->CR1 |= 0x200;
			}	
			USART1->CR1 &= ~0x400;
			if(uart_struct->uart_work.parity_enable == 1) {
				USART1->CR1 |= 0x400;
			}	
			//tx and rx enable
			USART1->CR1 &= ~0x08;
			if(uart_struct->uart_work.tx_enable == 1) {
				USART1->CR1 |= 0x08;
			}	
			USART1->CR1 &= ~0x04;
			if(uart_struct->uart_work.rx_enable == 1) {
				USART1->CR1 |= 0x04;
			}							
			//idle irq set
			USART1->CR1 &= ~0x10;
			if(uart_struct->uart_work.idle_irq == 1) {
				USART1->CR1 |= 0x10;
			}				

			//trans complete irq set
			USART1->CR1 &= ~0x40;
			if(uart_struct->uart_work.tx_complete_irq == 1) {
				USART1->CR1 |= 0x40;
			}	
			//trans buffer empty irq set
			USART1->CR1 &= ~0x80;
			if(uart_struct->uart_work.tx_empty_irq == 1) {
				USART1->CR1 |= 0x80;
			}	
			//parity error irq set
			USART1->CR1 &= ~0x100;
			if(uart_struct->uart_work.parity_err_irq == 1) {
				USART1->CR1 |= 0x100;
			}	
			//enable uart
			USART1->CR1 &= ~0x2000;			
			if(uart_struct->uart_work.on_off == 1) {
				USART1->CR1 |= 0x2000;  
			}
			//rx nonempty irq set
			USART1->CR1 &= ~0x20;
			if(uart_struct->uart_work.rx_nonempty_irq == 1) {
				USART1->CR1 |= 0x20;
				MY_NVIC_Init(3,3,USART1_IRQn,2);
			}			
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;		
	}
	
}
