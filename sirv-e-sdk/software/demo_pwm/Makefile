TARGET = demo_pwm
CFLAGS += -O2 -DUSE_PLIC -DUSE_M_TIME

BSP_BASE = ../../bsp

C_SRCS += demo_pwm.c

C_SRCS += $(BSP_BASE)/drivers/plic/plic_driver.c

include $(BSP_BASE)/env/common.mk
