#ifndef __BUTTON_H
#define __BUTTON_H
#include "sys.h"
#include "gpio.h"

#define BUTTON_NUM		3
#define BUTTON_WAKEUP	  PAin(0)
#define BUTTON_KEY0			PEin(4)
#define BUTTON_KEY1			PEin(3)

#define WAKEUP_VALUE   1
#define KEY0_VALUE  	 2
#define KEY1_VALUE  	 3

static const struct GPIO_CFG button_configuration[BUTTON_NUM] = {
	{0x0A, 0, INPUT_MODE, PULL_UP_DN},
	{0x0E, 4, INPUT_MODE, PULL_UP_DN},	
	{0x0E, 3, INPUT_MODE, PULL_UP_DN},	
};

void button_init(void);
int button_check(void);
#endif
