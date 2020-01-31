/*
 * STM32F103ZETB smart_card.c
 *
 * Author:      David Yang, <yangxiaohua2009@outlook.com>
 * Copyright:   (C) 2020 David Yang
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Notes:
 *			2020-01-17, rev00, initial
 *
 */
#include "smart_card.h"
#include "gpio.h"

extern const struct GPIO_CFG uart2_gpio_configuration[10];
extern const struct GPIO_CFG uart3_gpio_configuration[15];

/*
*	smartcard_initialize()
*	description:	initialize smartcard
*	in param	 :  void *p, pointer to SMART_CARD_CFG struct
*	out param	 :	int ret, =0, success, =1, fail
*/
int smartcard_initialize(void *p)
{
		struct SMART_CARD_CFG *smart_struct;
	int baudrate_m, baudrate_s;
	float tmp;
	int index = 0;
	smart_struct = p;
	/*
	*	Check the pclk frequency 
	*/
	if((smart_struct->uart_num != 1) && smart_struct->pclk_freq > 36 ) {
		return -1;
	}
	/*
	*	Check uart number 
	*/
	if((smart_struct->uart_num != 2) && (smart_struct->uart_num != 3)) {
		return -1;
	}
	/*
	* baudrate
	*/
	tmp = (float)(smart_struct->pclk_freq * 1000000) / (16 * smart_struct->baudrate);
	baudrate_m = (int)tmp;
	baudrate_s = (tmp - baudrate_m) * 16;
	baudrate_m = baudrate_m << 4;
	baudrate_m += baudrate_s;
	/*
	* uart2 for smart card mode
	*/
	if(smart_struct->uart_num == 2) {
			// check pin remap or not
			if(smart_struct->pin_remap == 0) {
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
			if(smart_struct->uart_work.data_mode == 1) { //start +9dat+nstop
				USART2->CR1 |= 0x01000;
			}
			
			USART2->CR1 |= (1<<10); //pce = 1
			//stop bits
			USART2->CR2 &= ~0x3000;
			USART2->CR2 |= 0x3000;   //must set stop bit to on-half mode
			
			//half duplex or duplex
			USART2->CR3 &= ~0x0008;
			if(smart_struct->uart_work.half_duplex == 1) {
				USART2->CR3 |= 0x0008;
			}
			//wake mode
			USART2->CR1 &= ~0x0800;
			if(smart_struct->uart_work.wake_mode == 1) {
				USART2->CR1 |= 0x0800;
			}
			//receive wakeup
			USART2->CR1 &= ~0x02;
			if(smart_struct->uart_work.rec_wakeup == 1) {
				USART2->CR1 |= 0x02;
			}
			//parity check
			USART2->CR1 &= ~0x200;
			if(smart_struct->uart_work.parity_select == 1) {
				USART2->CR1 |= 0x200;
			}	
			USART2->CR1 &= ~0x400;
			if(smart_struct->uart_work.parity_enable == 1) {
				USART2->CR1 |= 0x400;
			}	
			//rts enable
			USART2->CR3 &= ~(1<<8);
			if(smart_struct->uart_work.rts_enable == 1) {
				USART2->CR3 |= (1<<8);
			}	
			//cts enable
			USART2->CR3 &= ~(1<<9);
			if(smart_struct->uart_work.cts_enable == 1) {
				USART2->CR3 |= (1<<9);
			}					
			//tx and rx enable
			USART2->CR1 &= ~0x08;
			if(smart_struct->uart_work.tx_enable == 1) {
				USART2->CR1 |= 0x08;
			}	
			USART2->CR1 &= ~0x04;
			if(smart_struct->uart_work.rx_enable == 1) {
				USART2->CR1 |= 0x04;
			}							
			//idle irq set
			USART2->CR1 &= ~0x10;
			if(smart_struct->uart_work.idle_irq == 1) {
				USART2->CR1 |= 0x10;
			}				

			//trans complete irq set
			USART2->CR1 &= ~0x40;
			if(smart_struct->uart_work.tx_complete_irq == 1) {
				USART2->CR1 |= 0x40;
			}	
			//trans buffer empty irq set
			USART2->CR1 &= ~0x80;
			if(smart_struct->uart_work.tx_empty_irq == 1) {
				USART2->CR1 |= 0x80;
			}	
			//parity error irq set
			USART2->CR1 &= ~0x100;
			if(smart_struct->uart_work.parity_err_irq == 1) {
				USART2->CR1 |= 0x100;
			}	
			//cts irq set
			USART2->CR3 &= ~(1<<10);
			if(smart_struct->uart_work.cts_irq == 1) {
				USART2->CR3 |= (1<<10);
			}	
			
			USART2->CR3 &= ~0x0A; //clear iren and hdsel
			USART2->CR2 &= ~0x4000; //clear linen
			//clk enable
			USART2->CR2 &= ~0x800; 
			if(smart_struct->smart_work.clk_en == 1) {
				USART2->CR2 |= 0x800;  
			}		
			//guard time
			USART2->GTPR &= ~0xf000; 
			USART2->GTPR |= ((smart_struct->smart_work.guard_time &0xff) << 8);
			//nack enable
			USART2->CR3 &= ~(1<<4); 
			if(smart_struct->smart_work.nack == 1) {
				USART2->CR3 |= (1<<4);  
			}				
			//smartcard enable
			USART2->CR3 &= ~(1<<5); 
			if(smart_struct->smart_work.enable == 1) {
				USART2->CR3 |= (1<<5);  
			}	
			
			//enable uart
			USART2->CR1 &= ~0x2000;			
			if(smart_struct->uart_work.on_off == 1) {
				USART2->CR1 |= 0x2000;  
			}
			//rx nonempty irq set
			USART2->CR1 &= ~0x20;

			if(smart_struct->uart_work.rx_nonempty_irq == 1) {
				USART2->CR1 |= 0x20;
				MY_NVIC_Init(3,3,USART2_IRQn,2);
			}									
	}
	/*
	* uart3 for smart card mode
	*/	
	if(smart_struct->uart_num == 3) {
			// check pin remap or not
			if(smart_struct->pin_remap == 0) {
				AFIO->MAPR &= ~0x08;
				for(index=0; index<5; index++) {
					gpio_init(uart3_gpio_configuration[index].bank, uart3_gpio_configuration[index].num,
										uart3_gpio_configuration[index].mode, uart3_gpio_configuration[index].cfg);
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
			USART3->BRR = baudrate_m;		// set baudrate
			
			//set uart2 data mode
			USART3->CR1 &= ~0x01000;
			if(smart_struct->uart_work.data_mode == 1) { //start +9dat+nstop
				USART3->CR1 |= 0x01000;
			}
			
			USART3->CR1 |= (1<<10); //pce = 1
			//stop bits
			USART3->CR2 &= ~0x3000;
			USART3->CR2 |= 0x3000;   //must set stop bit to on-half mode

			//half duplex or duplex
			USART3->CR3 &= ~0x0008;
			if(smart_struct->uart_work.half_duplex == 1) {
				USART3->CR3 |= 0x0008;
			}
			//wake mode
			USART3->CR1 &= ~0x0800;
			if(smart_struct->uart_work.wake_mode == 1) {
				USART3->CR1 |= 0x0800;
			}
			//receive wakeup
			USART3->CR1 &= ~0x02;
			if(smart_struct->uart_work.rec_wakeup == 1) {
				USART3->CR1 |= 0x02;
			}
			//parity check
			USART3->CR1 &= ~0x200;
			if(smart_struct->uart_work.parity_select == 1) {
				USART3->CR1 |= 0x200;
			}	
			USART3->CR1 &= ~0x400;
			if(smart_struct->uart_work.parity_enable == 1) {
				USART3->CR1 |= 0x400;
			}	
			//rts enable
			USART3->CR3 &= ~(1<<8);
			if(smart_struct->uart_work.rts_enable == 1) {
				USART3->CR3 |= (1<<8);
			}	
			//cts enable
			USART3->CR3 &= ~(1<<9);
			if(smart_struct->uart_work.cts_enable == 1) {
				USART3->CR3 |= (1<<9);
			}					
			//tx and rx enable
			USART3->CR1 &= ~0x08;
			if(smart_struct->uart_work.tx_enable == 1) {
				USART3->CR1 |= 0x08;
			}	
			USART3->CR1 &= ~0x04;
			if(smart_struct->uart_work.rx_enable == 1) {
				USART3->CR1 |= 0x04;
			}							
			//idle irq set
			USART3->CR1 &= ~0x10;
			if(smart_struct->uart_work.idle_irq == 1) {
				USART3->CR1 |= 0x10;
			}				

			//trans complete irq set
			USART3->CR1 &= ~0x40;
			if(smart_struct->uart_work.tx_complete_irq == 1) {
				USART3->CR1 |= 0x40;
			}	
			//trans buffer empty irq set
			USART3->CR1 &= ~0x80;
			if(smart_struct->uart_work.tx_empty_irq == 1) {
				USART3->CR1 |= 0x80;
			}	
			//parity error irq set
			USART3->CR1 &= ~0x100;
			if(smart_struct->uart_work.parity_err_irq == 1) {
				USART3->CR1 |= 0x100;
			}	
			//cts irq set
			USART3->CR3 &= ~(1<<10);
			if(smart_struct->uart_work.cts_irq == 1) {
				USART3->CR3 |= (1<<10);
			}	
			USART3->CR3 &= ~0x0A; //clear iren and hdsel
			USART3->CR2 &= ~0x4000; //clear linen		

			//clk enable
			USART3->CR2 &= ~0x800; 
			if(smart_struct->smart_work.clk_en == 1) {
				USART3->CR2 |= 0x800;  
			}		
			//guard time
			USART3->GTPR &= ~0xf000; 
			USART3->GTPR |= ((smart_struct->smart_work.guard_time &0xff) << 8);			
			
			//nack enable
			USART3->CR3 &= ~(1<<4); 
			if(smart_struct->smart_work.nack == 1) {
				USART3->CR3 |= (1<<4);  
			}		
			//smartcard enable
			USART3->CR3 &= ~(1<<5); 
			if(smart_struct->smart_work.enable == 1) {
				USART3->CR3 |= (1<<5);  
			}	
			
			//enable uart
			USART3->CR1 &= ~0x2000;			
			if(smart_struct->uart_work.on_off == 1) {
				USART3->CR1 |= 0x2000;  
			}
			//rx nonempty irq set
			USART3->CR1 &= ~0x20;

			
			if(smart_struct->uart_work.rx_nonempty_irq == 1) {
				USART3->CR1 |= 0x20;
				MY_NVIC_Init(3,3,USART3_IRQn,2);
			}				
	}
	return 0;
}
