// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include <string.h>
#include "plic/plic_driver.h"
#include "encoding.h"
#include <unistd.h>
#include "stdatomic.h"
#include "util.h"

// Structures for registering different interrupt handlers
// for different parts of the application.
typedef void (*function_ptr_t) (void);

void no_interrupt_handler (void) {};

function_ptr_t g_ext_interrupt_handlers[PLIC_NUM_INTERRUPTS];

// Instance data for the PLIC.

plic_instance_t g_plic;

const char * instructions_msg_sirv = " \
\r\n\
\r\n\
\r\n\
\r\n\
          #    #  ######  #####   ######\r\n\
          #    #  #       #    #  #\r\n\
          ######  #####   #    #  #####\r\n\
          #    #  #       #####   #\r\n\
          #    #  #       #   #   #\r\n\
          #    #  ######  #    #  ######\r\n\
\r\n\
\r\n\
                  #    #  ######\r\n\
                  #    #  #\r\n\
                  #    #  #####\r\n\
                  # ## #  #\r\n\
                  ##  ##  #\r\n\
                  #    #  ######\r\n\
\r\n\
\r\n\
                   ####    ####\r\n\
                  #    #  #    #\r\n\
                  #       #    #\r\n\
                  #  ###  #    #\r\n\
                  #    #  #    #\r\n\
                   ####    ####\r\n\
\r\n\
\r\n\
                !! HummingBird !! \r\n\
\r\n\
   ######    ###    #####   #####          #     #\r\n\
   #     #    #    #     # #     #         #     #\r\n\
   #     #    #    #       #               #     #\r\n\
   ######     #     #####  #        #####  #     #\r\n\
   #   #      #          # #                #   #\r\n\
   #    #     #    #     # #     #           # #\r\n\
   #     #   ###    #####   #####             #\r\n\
\r\n\
\r\n\
                  ####### #     #\r\n\
                  #     # ##    #\r\n\
                  #     # # #   #\r\n\
                  #     # #  #  #\r\n\
                  #     # #   # #\r\n\
                  #     # #    ##\r\n\
                  ####### #     #\r\n\
\r\n\
#                                               #######\r\n\
#           #     ####   #    #  ######  ######    #       ##    #    #   ####\r\n\
#           #    #    #  #    #  #       #         #      #  #   ##   #  #    #\r\n\
#           #    #       ######  #####   #####     #     #    #  # #  #  #\r\n\
#           #    #       #    #  #       #         #     ######  #  # #  #  ###\r\n\
#           #    #    #  #    #  #       #         #     #    #  #   ##  #    #\r\n\
#######     #     ####   #    #  ######  ######    #     #    #  #    #   ####\r\n\
\r\n\
                  Gpio Demo\r\n\
                  Red LED will flashing and the frequency is 1HZ\r\n\
 ";

/*Entry Point for PLIC Interrupt Handler*/
void handle_m_ext_interrupt(){
  plic_source int_num  = PLIC_claim_interrupt(&g_plic);
  if ((int_num >=1 ) && (int_num < PLIC_NUM_INTERRUPTS)) {
    g_ext_interrupt_handlers[int_num]();
  }
  else {
    exit(1 + (uintptr_t) int_num);
  }
  PLIC_complete_interrupt(&g_plic, int_num);
}

/*Entry Point for Machine Timer Interrupt Handler*/
void handle_m_time_interrupt(){
  // disable the timer interrupt.
  clear_csr(mie, MIP_MTIP);
  volatile uint64_t * mtime       = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME);
  volatile uint64_t * mtimecmp    = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIMECMP);
  uint64_t now = *mtime;
  uint64_t then = now + RTC_FREQ / 2;
  *mtimecmp = then;
  //set CLINT_MTIMECMP.
  GPIO_REG(GPIO_OUTPUT_VAL) ^=  GPIO_REG(GPIO_OUTPUT_VAL);
  // Re-enable the timer interrupt.
  set_csr(mie, MIP_MTIP);
  // printf("%s\r\n",__func__);
}

void set_int (){

  // Disable the machine & timer interrupts until setup is done.
  clear_csr(mie, MIP_MEIP);
  clear_csr(mie, MIP_MTIP);

  for (int ii = 0; ii < PLIC_NUM_INTERRUPTS; ii ++){
    g_ext_interrupt_handlers[ii] = no_interrupt_handler;
  }
    // Set the machine timer to go off in 3 seconds.
    // The
    volatile uint64_t * mtime       = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIME);
    volatile uint64_t * mtimecmp    = (uint64_t*) (CLINT_CTRL_ADDR + CLINT_MTIMECMP);
    uint64_t now = *mtime;
    uint64_t then = now + 10;
    *mtimecmp = then;

    // Enable the Machine-External bit in MIE
    // set_csr(mie, MIP_MEIP);

    // Enable the Machine-Timer bit in MIE
    set_csr(mie, MIP_MTIP);

    // Enable interrupts in general.
    set_csr(mstatus, MSTATUS_MIE);
}

int main(int argc, char **argv)
{
  uint8_t a=0;

  printf("%s\r\n",instructions_msg_sirv);
  GPIO_REG(GPIO_OUTPUT_EN) = 0xffffffff; 
  GPIO_REG(GPIO_OUTPUT_VAL) = 0xffffffff;
    // Enable the Machine-Timer bit in MIE
    set_csr(mie, MIP_MTIP);
  while(1){
    if(a)
    {
      a = 0;
      GPIO_REG(GPIO_OUTPUT_VAL) = 0xffffffff;
    }
    else
    {
      a = 1;
      GPIO_REG(GPIO_OUTPUT_VAL) = 0x00000000;
    }
    delay_ms(500);
  }
  return 0;
}
