#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"
#include "gpio.h"

#define BEEP_NUM 1

#define BEEP PBout(8)

static const struct GPIO_CFG beep_configuration[BEEP_NUM] = {
	{0x0B, 0x08, OUT_MODE_2MHZ, PUSH_PULL},
};

void beep_init(void);
int beep_start(int beep_num, int status);
#endif
