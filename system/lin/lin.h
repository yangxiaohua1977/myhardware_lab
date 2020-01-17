#ifndef __LIN_H
#define __LIN_H
#include "sys.h"
#include "uart.h"
struct LIN_CARD_CFG
{
	int uart_num;			//uart number
	int pin_remap;		//uart pin remap, for uart1 and uart2, pin_remap = 0 or 1; 
										//for uart3, in_remap  0, 1 or 3;
	int pclk_freq;    //peripheral clock for uart
	int baudrate;			//baud rate, bps
	USART_MODE uart_work;	
	USART_DMA  lin_dma;
	LIN lin_work;
};

int lin_initialize(void *);
#endif
