#include "sys.h" 
#include "uart.h" 
#include "delay.h" 
#include "led.h"
#include "gpio.h"
#include "beep.h"
#include "button.h"
#include "stdbool.h"
/*
*	uart_configuration()
*	description:	initialize uart configuration
*	in param	 :  none
*	out param	 :	none
*/
void uart_configuration(void) 
{
	struct UART_CFG uart_struct;	
	uart_struct.pclk_freq = 72;
	uart_struct.baudrate = 115200;
	uart_struct.uart_num = 1;
	uart_struct.pin_remap = 0;
	uart_struct.uart_work.on_off = 1;
	uart_struct.uart_work.data_mode = DATMOD_1STA_8DAT_1STP;
	uart_struct.uart_work.stop_bit = ONE_STOP_BIT;
	uart_struct.uart_work.rx_enable =  ENABLE;
	uart_struct.uart_work.tx_enable =  ENABLE;
	uart_struct.uart_work.rx_nonempty_irq =  ENABLE;	
	uart_initialize(&uart_struct); //?????? 115200 
	
}
/*
*	hex2asc()
*	description:	hex code translated to ascii code
*	in param	 :  int hexcode 
*	out param	 :	int asccode
*/
int hex2asc(int hexcode) 
{
	int asccode = 0;
	int tmp = 0;

	if ((hexcode >0x2F) && (hexcode < 0x3a)) {
	   asccode = 	tmp - 0x30;
	}
	if ((hexcode >0x40) && (hexcode < 0x47)) {
	   asccode = 	tmp - 0x41;
	}	
	if ((hexcode >0x60) && (hexcode < 0x67)) {
	   asccode = 	tmp - 0x61;
	}	
	return asccode;
}
int main(void) { 
	u8 t=0; 
	u8 i=0;
	int led_status, beep_status, button_value;
	Stm32_Clock_Init(9); //??????
	delay_init(72); //? ????
	uart_configuration();
	led_init();
	beep_init();
	button_init();
	led_status = STATUS_OFF;
	beep_status = STATUS_OFF;
	button_value = 0;
	while(1) { 
		if((USART_RX_STA & 0x8000) == 0x8000) {
			t = USART_RX_STA &0x00ff;
			printf("receive "); 
			for(i=0; i< t; i++){
				printf("%c", USART_RX_BUF[i]); 
			}	
			printf("\r\n"); 
			USART_RX_STA = 0;		
		}
		button_value = button_check();
		switch(button_value) {
			case WAKEUP_VALUE:
					for(i= 0; i<BEEP_NUM; i++) {
						beep_start(i, beep_status);
					}				
				break;
			case KEY0_VALUE:
					for(i= 0; i<LED_NUM; i++) {
						led_flash(i, led_status);
					}
				break;
			case KEY1_VALUE:
				break;
			default:
				break;
		}
		if(led_status == STATUS_OFF) led_status = STATUS_ON; else led_status = STATUS_OFF;
		if(beep_status == STATUS_OFF) beep_status = STATUS_ON; else beep_status = STATUS_OFF;

	}
}

