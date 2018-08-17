TARGET = demo_car
CFLAGS += -O2 -DUSE_PLIC -DUSE_M_TIME

BSP_BASE = ../../bsp

C_SRCS += demo_car.c
C_SRCS += util.c

C_SRCS += $(BSP_BASE)/drivers/plic/plic_driver.c

include $(BSP_BASE)/env/common.mk
