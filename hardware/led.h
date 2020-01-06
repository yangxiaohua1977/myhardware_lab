#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#include "gpio.h"

#define LED_NUM			2

/*
*	LED GPIO Bank, Number, mode, configuration
*/
#define LED0_BANK		0x0b
#define LED0_NUM    0x05
#define LED0_MODE   OUT_MODE_50MHZ
#define LED0_CFG    PUSH_PULL 

#define LED1_BANK		0x0e
#define LED1_NUM    0x05
#define LED1_MODE   OUT_MODE_50MHZ
#define LED1_CFG    PUSH_PULL 

//LED Port
#define LED0 PBout(5)	// DS0
#define LED1 PEout(5)	// DS1	

static const struct GPIO_CFG led_configuration[LED_NUM] = {
	{LED0_BANK, LED0_NUM, LED0_MODE, LED0_CFG},
	{LED1_BANK, LED1_NUM, LED1_MODE, LED1_CFG},	
};

void led_init(void);
int led_flash(int lednum, int status);
#endif

















