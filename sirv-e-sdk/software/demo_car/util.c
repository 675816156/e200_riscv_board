#include "util.h"

void delay_us(uint32_t val)
{
    volatile uint64_t *  now = (volatile uint64_t*)(CLINT_CTRL_ADDR + CLINT_MTIME);
    volatile uint64_t then = *now + val / 30;
    while (*now < then) { };
}

void delay_ms(uint32_t val)
{
	delay_us(val*1000);
}

void GPIO_Set(uint32_t gpio,uint8_t val)
{
    if(gpio<0 || gpio >31)
    {
        return;
    }

    if(val)
	{
		GPIO_REG(GPIO_OUTPUT_VAL) |= (0x1<< gpio);
	}
	else
	{
		GPIO_REG(GPIO_OUTPUT_VAL) &= ~(0x1<< gpio);
	}
}

uint8_t GPIO_Get(uint32_t gpio)
{
    uint32_t ra=0;

    if(gpio<0 || gpio >31)
    {
        return 0;
    }

	ra = GPIO_REG(GPIO_INPUT_VAL);
	return ((ra & (1<<gpio)) >> gpio);
}

