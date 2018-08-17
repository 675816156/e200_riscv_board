#!/bin/sh
riscv-none-embed-objdump -S $1 > $1.objdump
