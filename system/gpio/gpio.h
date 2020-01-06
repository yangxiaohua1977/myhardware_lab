#ifndef __GPIO_H
#define __GPIO_H
#include "sys.h"

/*
* gpio mode 
*/
#define INPUT_MODE 0x00
#define OUT_MODE_10MHZ 0x01
#define OUT_MODE_2MHZ 0x02
#define OUT_MODE_50MHZ 0x03
/*
* gpio configuration-input
*/
#define ANALOG 0x00
#define FLOAT 0x01
#define PULL_UP_DN 0x02
#define RESERVED 0x03
/*
* gpio configuration-output
*/
#define PUSH_PULL 0x00
#define OPEN_DRAIN 0x01
#define SEC_PUSH_PULL 0x02
#define SEC_OPEN_DRAIN 0x03

struct GPIO_CFG
{
	int bank;
	int num;
	int mode;
	int cfg;
};

int gpio_init(int gpio_bank, int gpio_num, int gpio_mode, int gpio_cfg);
#endif
