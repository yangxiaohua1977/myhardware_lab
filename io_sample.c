#include "sys.h" 
#include "usart.h" 
#include "delay.h" 
#include "led.h"
#include "gpio.h"
#include "beep.h"
#include "button.h"
#include "stdbool.h"
int main(void) { 
	u8 t=0; 
	u8 i=0;
	int led_status, beep_status, button_value;
	Stm32_Clock_Init(9); //??????
	delay_init(72); //? ????
	uart_init(72,115200); //?????? 115200 
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
				printf("%2x", USART_RX_BUF[i]); 
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

