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
	//REGMAP = 0
	{0x0A, 0x09, OUT_MODE_50MHZ, PUSH_PULL},   //uart1 tx, PA9
	{0x0A, 0x0A, INPUT_MODE, PUSH_PULL},			 //uart1 rx, PA10
	//REGMAP = 1
	{0x0B, 0x06, OUT_MODE_50MHZ, PUSH_PULL},   //uart1 tx, PB6
	{0x0B, 0x07, INPUT_MODE, PUSH_PULL},			 //uart1 rx, PB7
};
const struct GPIO_CFG uart2_gpio_configuration[10] = {
	//REGMAP = 0
	{0x0A, 0x00, INPUT_MODE, PUSH_PULL},   //uart2 cts, PA0
	{0x0A, 0x01, OUT_MODE_50MHZ, PUSH_PULL},			 //uart2 rts, PA1
	{0x0A, 0x02, OUT_MODE_50MHZ, PUSH_PULL},   //uart2 tx, PA2
	{0x0A, 0x03, INPUT_MODE, PUSH_PULL},			 //uart2 rx, PA3
	{0x0A, 0x04, OUT_MODE_50MHZ, PUSH_PULL},			 //uart2 ck, PA4
	//REGMAP = 1
	{0x0d, 0x03, INPUT_MODE, PUSH_PULL},   		 //uart2 cts, PA0
	{0x0d, 0x04, OUT_MODE_50MHZ, PUSH_PULL},	 //uart2 rts, PA1
	{0x0d, 0x05, OUT_MODE_50MHZ, PUSH_PULL},   //uart2 tx, PA2
	{0x0d, 0x06, INPUT_MODE, PUSH_PULL},			 //uart2 rx, PA3
	{0x0d, 0x07, OUT_MODE_50MHZ, PUSH_PULL},	 //uart2 ck, PA4	
};
const struct GPIO_CFG uart3_gpio_configuration[15] = {
	//REGMAP = 00
	{0x0b, 0x0d, INPUT_MODE, PUSH_PULL},   		 //uart2 cts, PB13
	{0x0b, 0x0e, OUT_MODE_50MHZ, PUSH_PULL},	 //uart2 rts, PB14
	{0x0b, 0x0a, OUT_MODE_50MHZ, PUSH_PULL},   //uart2 tx, PB10
	{0x0b, 0x0b, INPUT_MODE, PUSH_PULL},			 //uart2 rx, PB11
	{0x0b, 0x0c, OUT_MODE_50MHZ, PUSH_PULL},	 //uart2 ck, PB12
	//REGMAP = 01
	{0x0b, 0x0d, INPUT_MODE, PUSH_PULL},   		 //uart2 cts, PB13
	{0x0b, 0x0e, OUT_MODE_50MHZ, PUSH_PULL},	 //uart2 rts, PB14
	{0x0C, 0x0a, OUT_MODE_50MHZ, PUSH_PULL},   //uart2 tx, PC10
	{0x0C, 0x0b, INPUT_MODE, PUSH_PULL},			 //uart2 rx, PC11
	{0x0C, 0x0c, OUT_MODE_50MHZ, PUSH_PULL},	 //uart2 ck, PC12
	//REGMAP = 11
	{0x0D, 0x0B, INPUT_MODE, PUSH_PULL},   		 //uart2 cts, PD11
	{0x0D, 0x0C, OUT_MODE_50MHZ, PUSH_PULL},	 //uart2 rts, PD12
	{0x0D, 0x08, OUT_MODE_50MHZ, PUSH_PULL},   //uart2 tx, PD8
	{0x0D, 0x09, INPUT_MODE, PUSH_PULL},			 //uart2 rx, PD9
	{0x0D, 0x0A, OUT_MODE_50MHZ, PUSH_PULL},	 //uart2 ck, P10	
};

static const struct GPIO_CFG uart4_gpio_configuration[2] = {
	{0x0c, 0x0a, OUT_MODE_50MHZ, PUSH_PULL},   //uart2 tx, PC10
	{0x0c, 0x0b, INPUT_MODE, PUSH_PULL},			 //uart2 rx, PC11
};
static const struct GPIO_CFG uart5_gpio_configuration[2] = {
	{0x0c, 0x0C, OUT_MODE_50MHZ, PUSH_PULL},   //uart2 tx, PC12
	{0x0D, 0x02, INPUT_MODE, PUSH_PULL},			 //uart2 rx, PD2
};
/*
*	uart_initialize()
*	description:	initialize uart
*	in param	 :  void *p, pointer to uart_cfg struct
*	out param	 :	int ret, =0, success, =1, fail
*/
int uart_initialize(void *p)
{
	struct UART_CFG *uart_struct;
	int baudrate_m, baudrate_s;
	float tmp;
	int index = 0;
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
			//rts enable
			USART1->CR3 &= ~(1<<8);
			if(uart_struct->uart_work.rts_enable == 1) {
				USART1->CR3 |= (1<<8);
			}	
			//cts enable
			USART1->CR3 &= ~(1<<9);
			if(uart_struct->uart_work.cts_enable == 1) {
				USART1->CR3 |= (1<<9);
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
			//cts irq set
			USART1->CR3 &= ~(1<<10);
			if(uart_struct->uart_work.cts_irq == 1) {
				USART1->CR3 |= (1<<10);
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
		case 2: //uart2
			// check pin remap or not
			if(uart_struct->pin_remap == 0) {
				AFIO->MAPR &= ~0x08;
				for(index=0; index<5; index++) {
					gpio_init(uart2_gpio_configuration[index].bank, uart2_gpio_configuration[index].num,
										uart2_gpio_configuration[index].mode, uart2_gpio_configuration[index].cfg);
				}		
			}
			else {
				AFIO->MAPR &= ~0x08;
				AFIO->MAPR |= 0x08;
				
				for(index=5; index<10; index++) {
					gpio_init(uart2_gpio_configuration[index].bank, uart2_gpio_configuration[index].num,
										uart2_gpio_configuration[index].mode, uart2_gpio_configuration[index].cfg);
				}		 
			}
			RCC->APB1RSTR |= 1 << 17;   // reset uart2
			RCC->APB1RSTR &= ~(1 << 17);
			
			RCC->APB1ENR|=1<<17;  //enable uart2 clock
			USART2->BRR = baudrate_m;		// set baudrate
			
			//set uart2 data mode
			USART2->CR1 &= ~0x01000;
			if(uart_struct->uart_work.data_mode == 1) { //start +9dat+nstop
				USART2->CR1 |= 0x01000;
			}
			//stop bits
			USART2->CR2 &= ~0x3000;
			switch(uart_struct->uart_work.stop_bit) {
				case ONE_STOP_BIT:
					break;
				case HALF_STOP_BIT:
					USART2->CR2 |= 0x1000;
					break;	
				case TWO_STOP_BIT:
					USART2->CR2 |= 0x2000;
					break;
				case ONEHALF_STOP_BIT:
					USART2->CR2 |= 0x3000;
					break;	
				default:
					break;
			}
			//half duplex or duplex
			USART2->CR3 &= ~0x0008;
			if(uart_struct->uart_work.half_duplex == 1) {
				USART2->CR3 |= 0x0008;
			}
			//wake mode
			USART2->CR1 &= ~0x0800;
			if(uart_struct->uart_work.wake_mode == 1) {
				USART2->CR1 |= 0x0800;
			}
			//receive wakeup
			USART2->CR1 &= ~0x02;
			if(uart_struct->uart_work.rec_wakeup == 1) {
				USART2->CR1 |= 0x02;
			}
			//parity check
			USART2->CR1 &= ~0x200;
			if(uart_struct->uart_work.parity_select == 1) {
				USART2->CR1 |= 0x200;
			}	
			USART2->CR1 &= ~0x400;
			if(uart_struct->uart_work.parity_enable == 1) {
				USART2->CR1 |= 0x400;
			}	
			//rts enable
			USART2->CR3 &= ~(1<<8);
			if(uart_struct->uart_work.rts_enable == 1) {
				USART2->CR3 |= (1<<8);
			}	
			//cts enable
			USART2->CR3 &= ~(1<<9);
			if(uart_struct->uart_work.cts_enable == 1) {
				USART2->CR3 |= (1<<9);
			}					
			//tx and rx enable
			USART2->CR1 &= ~0x08;
			if(uart_struct->uart_work.tx_enable == 1) {
				USART2->CR1 |= 0x08;
			}	
			USART2->CR1 &= ~0x04;
			if(uart_struct->uart_work.rx_enable == 1) {
				USART2->CR1 |= 0x04;
			}							
			//idle irq set
			USART2->CR1 &= ~0x10;
			if(uart_struct->uart_work.idle_irq == 1) {
				USART2->CR1 |= 0x10;
			}				

			//trans complete irq set
			USART2->CR1 &= ~0x40;
			if(uart_struct->uart_work.tx_complete_irq == 1) {
				USART2->CR1 |= 0x40;
			}	
			//trans buffer empty irq set
			USART2->CR1 &= ~0x80;
			if(uart_struct->uart_work.tx_empty_irq == 1) {
				USART2->CR1 |= 0x80;
			}	
			//parity error irq set
			USART2->CR1 &= ~0x100;
			if(uart_struct->uart_work.parity_err_irq == 1) {
				USART2->CR1 |= 0x100;
			}	
			//cts irq set
			USART2->CR3 &= ~(1<<10);
			if(uart_struct->uart_work.cts_irq == 1) {
				USART2->CR3 |= (1<<10);
			}				
			//enable uart
			USART2->CR1 &= ~0x2000;			
			if(uart_struct->uart_work.on_off == 1) {
				USART2->CR1 |= 0x2000;  
			}
			//rx nonempty irq set
			USART2->CR1 &= ~0x20;
			if(uart_struct->uart_work.rx_nonempty_irq == 1) {
				USART2->CR1 |= 0x20;
				MY_NVIC_Init(3,3,USART2_IRQn,2);
			}						
			break;
		case 3: //uart3
			// check pin remap or not
			switch(uart_struct->pin_remap) {
				case 0:
					AFIO->MAPR &= ~0x30;
					for(index=0; index<5; index++) {
						gpio_init(uart3_gpio_configuration[index].bank, uart3_gpio_configuration[index].num,
											uart3_gpio_configuration[index].mode, uart3_gpio_configuration[index].cfg);
					}	
				break;
				case 1:
					AFIO->MAPR &= ~0x30;
					AFIO->MAPR |= 0x10;
					for(index=5; index<10; index++) {
						gpio_init(uart3_gpio_configuration[index].bank, uart3_gpio_configuration[index].num,
											uart3_gpio_configuration[index].mode, uart3_gpio_configuration[index].cfg);
					}	
				break;
				case 3:
					AFIO->MAPR &= ~0x30;
					AFIO->MAPR |= 0x30;
					for(index=10; index<15; index++) {
						gpio_init(uart3_gpio_configuration[index].bank, uart3_gpio_configuration[index].num,
											uart3_gpio_configuration[index].mode, uart3_gpio_configuration[index].cfg);
					}	
				break;
				default:
					return -1;
			}

			RCC->APB1RSTR |= 1 << 18;   // reset uart3
			RCC->APB1RSTR &= ~(1 << 18);
			
			RCC->APB1ENR|=1<<18;  //enable uart3 clock
			USART3->BRR = baudrate_m;		// set baudrate
			
			//set uart3 data mode
			USART3->CR1 &= ~0x01000;
			if(uart_struct->uart_work.data_mode == 1) { //start +9dat+nstop
				USART3->CR1 |= 0x01000;
			}
			//stop bits
			USART3->CR2 &= ~0x3000;
			switch(uart_struct->uart_work.stop_bit) {
				case ONE_STOP_BIT:
					break;
				case HALF_STOP_BIT:
					USART3->CR2 |= 0x1000;
					break;	
				case TWO_STOP_BIT:
					USART3->CR2 |= 0x2000;
					break;
				case ONEHALF_STOP_BIT:
					USART3->CR2 |= 0x3000;
					break;	
				default:
					break;
			}
			//half duplex or duplex
			USART3->CR3 &= ~0x0008;
			if(uart_struct->uart_work.half_duplex == 1) {
				USART3->CR3 |= 0x0008;
			}
			//wake mode
			USART3->CR1 &= ~0x0800;
			if(uart_struct->uart_work.wake_mode == 1) {
				USART3->CR1 |= 0x0800;
			}
			//receive wakeup
			USART3->CR1 &= ~0x02;
			if(uart_struct->uart_work.rec_wakeup == 1) {
				USART3->CR1 |= 0x02;
			}
			//parity check
			USART3->CR1 &= ~0x200;
			if(uart_struct->uart_work.parity_select == 1) {
				USART3->CR1 |= 0x200;
			}	
			USART3->CR1 &= ~0x400;
			if(uart_struct->uart_work.parity_enable == 1) {
				USART3->CR1 |= 0x400;
			}	
			//rts enable
			USART3->CR3 &= ~(1<<8);
			if(uart_struct->uart_work.rts_enable == 1) {
				USART3->CR3 |= (1<<8);
			}	
			//cts enable
			USART3->CR3 &= ~(1<<9);
			if(uart_struct->uart_work.cts_enable == 1) {
				USART3->CR3 |= (1<<9);
			}				
			//tx and rx enable
			USART3->CR1 &= ~0x08;
			if(uart_struct->uart_work.tx_enable == 1) {
				USART3->CR1 |= 0x08;
			}	
			USART3->CR1 &= ~0x04;
			if(uart_struct->uart_work.rx_enable == 1) {
				USART3->CR1 |= 0x04;
			}							
			//idle irq set
			USART3->CR1 &= ~0x10;
			if(uart_struct->uart_work.idle_irq == 1) {
				USART3->CR1 |= 0x10;
			}				

			//trans complete irq set
			USART3->CR1 &= ~0x40;
			if(uart_struct->uart_work.tx_complete_irq == 1) {
				USART3->CR1 |= 0x40;
			}	
			//trans buffer empty irq set
			USART3->CR1 &= ~0x80;
			if(uart_struct->uart_work.tx_empty_irq == 1) {
				USART3->CR1 |= 0x80;
			}	
			//parity error irq set
			USART3->CR1 &= ~0x100;
			if(uart_struct->uart_work.parity_err_irq == 1) {
				USART3->CR1 |= 0x100;
			}	
						//cts irq set
			USART3->CR3 &= ~(1<<10);
			if(uart_struct->uart_work.cts_irq == 1) {
				USART3->CR3 |= (1<<10);
			}	
			//enable uart
			USART3->CR1 &= ~0x2000;			
			if(uart_struct->uart_work.on_off == 1) {
				USART3->CR1 |= 0x2000;  
			}
			//rx nonempty irq set
			USART3->CR1 &= ~0x20;
			if(uart_struct->uart_work.rx_nonempty_irq == 1) {
				USART3->CR1 |= 0x20;
				MY_NVIC_Init(3,3,USART3_IRQn,2);
			}					
			break;
		case 4: //uart4
			//  pin map
			for(index=0; index<2; index++) {
				gpio_init(uart4_gpio_configuration[index].bank, uart4_gpio_configuration[index].num,
									uart4_gpio_configuration[index].mode, uart4_gpio_configuration[index].cfg);
			}	

			RCC->APB1RSTR |= 1 << 19;   // reset uart3
			RCC->APB1RSTR &= ~(1 << 19);
			
			RCC->APB1ENR|=1<<19;  //enable uart3 clock
			UART4->BRR = baudrate_m;		// set baudrate
			
			//set uart3 data mode
			UART4->CR1 &= ~0x01000;
			if(uart_struct->uart_work.data_mode == 1) { //start +9dat+nstop
				UART4->CR1 |= 0x01000;
			}
			//stop bits
			UART4->CR2 &= ~0x3000;
			switch(uart_struct->uart_work.stop_bit) {
				case ONE_STOP_BIT:
					break;
				case HALF_STOP_BIT:
					UART4->CR2 |= 0x1000;
					break;	
				case TWO_STOP_BIT:
					UART4->CR2 |= 0x2000;
					break;
				case ONEHALF_STOP_BIT:
					UART4->CR2 |= 0x3000;
					break;	
				default:
					break;
			}
			//half duplex or duplex
			UART4->CR3 &= ~0x0008;
			if(uart_struct->uart_work.half_duplex == 1) {
				UART4->CR3 |= 0x0008;
			}
			//wake mode
			UART4->CR1 &= ~0x0800;
			if(uart_struct->uart_work.wake_mode == 1) {
				UART4->CR1 |= 0x0800;
			}
			//receive wakeup
			UART4->CR1 &= ~0x02;
			if(uart_struct->uart_work.rec_wakeup == 1) {
				UART4->CR1 |= 0x02;
			}
			//parity check
			UART4->CR1 &= ~0x200;
			if(uart_struct->uart_work.parity_select == 1) {
				UART4->CR1 |= 0x200;
			}	
			UART4->CR1 &= ~0x400;
			if(uart_struct->uart_work.parity_enable == 1) {
				UART4->CR1 |= 0x400;
			}	
			//tx and rx enable
			UART4->CR1 &= ~0x08;
			if(uart_struct->uart_work.tx_enable == 1) {
				UART4->CR1 |= 0x08;
			}	
			UART4->CR1 &= ~0x04;
			if(uart_struct->uart_work.rx_enable == 1) {
				UART4->CR1 |= 0x04;
			}							
			//idle irq set
			UART4->CR1 &= ~0x10;
			if(uart_struct->uart_work.idle_irq == 1) {
				UART4->CR1 |= 0x10;
			}				

			//trans complete irq set
			UART4->CR1 &= ~0x40;
			if(uart_struct->uart_work.tx_complete_irq == 1) {
				UART4->CR1 |= 0x40;
			}	
			//trans buffer empty irq set
			UART4->CR1 &= ~0x80;
			if(uart_struct->uart_work.tx_empty_irq == 1) {
				UART4->CR1 |= 0x80;
			}	
			//parity error irq set
			UART4->CR1 &= ~0x100;
			if(uart_struct->uart_work.parity_err_irq == 1) {
				UART4->CR1 |= 0x100;
			}	
			//enable uart
			USART3->CR1 &= ~0x2000;			
			if(uart_struct->uart_work.on_off == 1) {
				USART3->CR1 |= 0x2000;  
			}
			//rx nonempty irq set
			UART4->CR1 &= ~0x20;
			if(uart_struct->uart_work.rx_nonempty_irq == 1) {
				UART4->CR1 |= 0x20;
				MY_NVIC_Init(3,3,UART4_IRQn,2);
			}				
			break;
		case 5: //UART5
			//  pin map
			for(index=0; index<2; index++) {
				gpio_init(uart5_gpio_configuration[index].bank, uart5_gpio_configuration[index].num,
									uart5_gpio_configuration[index].mode, uart5_gpio_configuration[index].cfg);
			}	

			RCC->APB1RSTR |= 1 << 20;   // reset uart3
			RCC->APB1RSTR &= ~(1 << 20);
			
			RCC->APB1ENR|=1<<20;  //enable uart3 clock
			UART5->BRR = baudrate_m;		// set baudrate
			
			//set uart3 data mode
			UART5->CR1 &= ~0x01000;
			if(uart_struct->uart_work.data_mode == 1) { //start +9dat+nstop
				UART5->CR1 |= 0x01000;
			}
			//stop bits
			UART5->CR2 &= ~0x3000;
			switch(uart_struct->uart_work.stop_bit) {
				case ONE_STOP_BIT:
					break;
				case HALF_STOP_BIT:
					UART5->CR2 |= 0x1000;
					break;	
				case TWO_STOP_BIT:
					UART5->CR2 |= 0x2000;
					break;
				case ONEHALF_STOP_BIT:
					UART5->CR2 |= 0x3000;
					break;	
				default:
					break;
			}
			//half duplex or duplex
			UART5->CR3 &= ~0x0008;
			if(uart_struct->uart_work.half_duplex == 1) {
				UART5->CR3 |= 0x0008;
			}
			//wake mode
			UART5->CR1 &= ~0x0800;
			if(uart_struct->uart_work.wake_mode == 1) {
				UART5->CR1 |= 0x0800;
			}
			//receive wakeup
			UART5->CR1 &= ~0x02;
			if(uart_struct->uart_work.rec_wakeup == 1) {
				UART5->CR1 |= 0x02;
			}
			//parity check
			UART5->CR1 &= ~0x200;
			if(uart_struct->uart_work.parity_select == 1) {
				UART5->CR1 |= 0x200;
			}	
			UART5->CR1 &= ~0x400;
			if(uart_struct->uart_work.parity_enable == 1) {
				UART5->CR1 |= 0x400;
			}	
			//tx and rx enable
			UART5->CR1 &= ~0x08;
			if(uart_struct->uart_work.tx_enable == 1) {
				UART5->CR1 |= 0x08;
			}	
			UART5->CR1 &= ~0x04;
			if(uart_struct->uart_work.rx_enable == 1) {
				UART5->CR1 |= 0x04;
			}							
			//idle irq set
			UART5->CR1 &= ~0x10;
			if(uart_struct->uart_work.idle_irq == 1) {
				UART5->CR1 |= 0x10;
			}				

			//trans complete irq set
			UART5->CR1 &= ~0x40;
			if(uart_struct->uart_work.tx_complete_irq == 1) {
				UART5->CR1 |= 0x40;
			}	
			//trans buffer empty irq set
			UART5->CR1 &= ~0x80;
			if(uart_struct->uart_work.tx_empty_irq == 1) {
				UART5->CR1 |= 0x80;
			}	
			//parity error irq set
			UART5->CR1 &= ~0x100;
			if(uart_struct->uart_work.parity_err_irq == 1) {
				UART5->CR1 |= 0x100;
			}	
			//enable uart
			UART5->CR1 &= ~0x2000;			
			if(uart_struct->uart_work.on_off == 1) {
				UART5->CR1 |= 0x2000;  
			}
			//rx nonempty irq set
			UART5->CR1 &= ~0x20;
			if(uart_struct->uart_work.rx_nonempty_irq == 1) {
				UART5->CR1 |= 0x20;
				MY_NVIC_Init(3,3,UART5_IRQn,2);
			}					
			break;
		default:
			return -1;		
	}
	/*
	*	To-Do, UART DMA configuration
	*/
	return 0;
}
