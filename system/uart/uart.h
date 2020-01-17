#ifndef __UART_H
#define __UART_H
#include "sys.h"

#define ONE_STOP_BIT 0
#define HALF_STOP_BIT 1
#define TWO_STOP_BIT 2
#define ONEHALF_STOP_BIT 3

typedef struct {
	int on_off;    //=0, uart mode disable; =1, uart mode enable. refer to USART_CR1.BIT13 
	
	int half_duplex; //=0, duplex; =1, half duplex; refer to USART_CR3.bit3
	//work mode
	int data_mode;   //=0, start+8dat+nstop;  =1, start+9dat+nstop. refer to USART_CR1.BIT12  
	int stop_bit;  //refer to USART_CR2.bit[13:12]    
	
	//wake-up method
	int wake_mode; //=0, waked-up by idle;  =1, waked-up by address. refer to USART_CR1.BIT11
	int rec_wakeup; //=0, normal rx mode; =1 idle mode; refer to USART_CR1.BIT1

	//parity check configuration
	int parity_enable; //=0, PCE disabled;  =1, PCE enabled. refer to USART_CR1.BIT10
	int parity_select; //=0, Even check; =1, Odd check; refer to USART_CR1.BIT9
	
	//IRQ setup
	int parity_err_irq; //=0, disabled; =1, enabled; refer to USART_CR1.BIT8
	int tx_empty_irq; //=0, disabled; =1 enabled;  refer to USART_CR1.BIT7
	int tx_complete_irq; //=0, disabled; =1, enabled; refer to USART_CR1.BIT6
	int rx_nonempty_irq; //=0, disabled; =1, enabled; refer to USART_CR1.BIT5
	int idle_irq; //=0, disabled; =1, enabled; refer to USART_CR1.BIT4
	int cts_irq;  //=0, disabled; =1, CTS irq enabled; refer to USART_CR3.BIT10 
	int err_irq;  //=0, disabled; =1, error irq enabled; refer to USART_CR3.BIT0
	//tx/rx enable
	int tx_enable; //=0, disable transmit; =1 enable transmit; refer to USART_CR1.BIT3
	int rx_enable; //=0, dsiable receive; =1 enable receive; refer to USART_CR1.BIT2
	
	//break
	int send_break; //=0, don't tx brk; =1 ready to tx brk; refer to USART_CR1.BIT0
	
	//flow control
	int cts_enable; //=0, disable; =1, enable cts; refer to USART_CR3.BIT9
	int rts_enable; //=0, disable; =1, enable rts; refer to USART_CR3.BIT8
	
	//address
	int address; //refer to USART_CR2.BIT[3:0]
}USART_MODE;
typedef struct {
	//DMA control
	int tx_dma_byte;
	int *txdma;
	int tx_dma;     //=0, disable; =1, enable; refer to USART_CR3.BIT7
	int rx_dma_byte;
	int *rxdma;
	int rx_dma;     //=0, disable; =1, enable; refer to USART_CR3.BIT6
} USART_DMA;
typedef struct {
	int enable;  //=0, disabled; =1, enabled; refer to USART_CR3.BIT5
	int clk_en; //=0, disable; =1, enable clock; refer to USART_CR2.BIT11	
	int nack;     //refer to USART_CR3.BIT5
	int guard_time; //refer to USART_GTPR.bit[8:15]
} SMART_CARD;
typedef struct {
	int enable;  //=0, disabled; =1, enabled; refer to USART_CR2.BIT14
	int clk_en; //=0, disable; =1, enable clock; refer to USART_CR2.BIT11
	int clk_pol;  //refer to USART_CR2.BIT10
	int clk_phase; //refer to USART_CR2.BIT9
	int last_clkbit; //refer to USART_CR2.BIT8
	int linbrk_irq;  //=0, disabled; =1, lin break detect irq enabled; refer to USART_CR2.bit6
	int lin_brklen;  //break length; refer to USART_CR2.bit5
} LIN;
typedef struct {
	int enable;  //=0, disabled; =1, enabled; refer to USART_CR3.BIT1
	int irda_lp; //=0, normal mode; =1, low power mode; refer to USART_CR3.BIT2
	int prediv;  //refer to USART_GTPR.bit[0:7]
} IRDA;
struct UART_CFG
{
	int uart_num;			//uart number
	int pin_remap;		//uart pin remap, for uart1 and uart2, pin_remap = 0 or 1; 
										//for uart3, in_remap  0, 1 or 3;
	int pclk_freq;    //peripheral clock for uart
	int baudrate;			//baud rate, bps
	USART_MODE uart_work;	
	USART_DMA  uart_dma;
};

int uart_initialize(void *);

#endif
