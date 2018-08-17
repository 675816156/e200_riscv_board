#ifndef __UTIL_H
#define __UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include <string.h>
#include "plic/plic_driver.h"
#include "encoding.h"
#include <unistd.h>
#include "stdatomic.h"

void delay_us(uint32_t val);
void delay_ms(uint32_t val);
void GPIO_Set(uint32_t gpio,uint8_t val);
uint8_t GPIO_Get(uint32_t gpio);

#endif
