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
  clear_csr(mie, MIP_MTIP);
  // Re-enable the timer interrupt.
  // set_csr(mie, MIP_MTIP);
}

const char * instructions_msg = " \
\r\n\
                SIFIVE, INC.\r\n\
\r\n\
         5555555555555555555555555\r\n\
        5555                   5555\r\n\
       5555                     5555\r\n\
      5555                       5555\r\n\
     5555       5555555555555555555555\r\n\
    5555       555555555555555555555555\r\n\
   5555                             5555\r\n\
  5555                               5555\r\n\
 5555                                 5555\r\n\
5555555555555555555555555555          55555\r\n\
 55555           555555555           55555\r\n\
   55555           55555           55555\r\n\
     55555           5           55555\r\n\
       55555                   55555\r\n\
         55555               55555\r\n\
           55555           55555\r\n\
             55555       55555\r\n\
               55555   55555\r\n\
                 555555555\r\n\
                   55555\r\n\
                     5\r\n\
\r\n\
SiFive E-Series Software Development Kit 'demo_gpio' program.\r\n\
Every 2 second, the Timer Interrupt will invert the LEDs.\r\n\
(Arty Dev Kit Only): Press Buttons 0, 1, 2 to Set the LEDs.\r\n\
Pin 19 (HiFive1) or A5 (Arty Dev Kit) is being bit-banged\r\n\
for GPIO speed demonstration.\r\n\
\r\n\
 ";

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
                  Car Demo\r\n\
 ";

#define LEFT_FRONT_GPIO     4
#define LEFT_BACK_GPIO      5
#define RIGHT_FRONT_GPIO    6
#define RIGHT_BACK_GPIO     7

#define left     'C'
#define right    'D'
#define up       'A'
#define down     'B'
#define stop     'F'

const char *GoFront = "收到指令，向前!\n";
const char *GoBack  = "收到指令，向后!\n";
const char *GoLeft  = "收到指令，向左!\n";
const char *GoRight = "收到指令，向右!\n";
const char *Stop    = "收到指令，停止!\n";

//左电机向前走
void Left_moto_go(void)
{
    GPIO_Set(LEFT_FRONT_GPIO,1);
    GPIO_Set(LEFT_BACK_GPIO,0);
}
//左边电机向后转
void Left_moto_back(void)
{
    GPIO_Set(LEFT_FRONT_GPIO,0);
    GPIO_Set(LEFT_BACK_GPIO,1);
}
//左边电机停转 
void Left_moto_Stop(void)
{
    GPIO_Set(LEFT_FRONT_GPIO,0);
    GPIO_Set(LEFT_BACK_GPIO,0);
}
//右边电机向前走
void Right_moto_go(void)
{
    GPIO_Set(RIGHT_FRONT_GPIO,1);
    GPIO_Set(RIGHT_BACK_GPIO,0);
}
//右边电机向后走
void Right_moto_back(void)
{
    GPIO_Set(RIGHT_FRONT_GPIO,0);
    GPIO_Set(RIGHT_BACK_GPIO,1);
}

//右边电机停转  
void Right_moto_Stop(void)
{
    GPIO_Set(RIGHT_FRONT_GPIO,0);
    GPIO_Set(RIGHT_BACK_GPIO,0);
}

//前速前进
void  run (void)
{
    Left_moto_go() ;   //左电机往前走
    Right_moto_go() ;  //右电机往前走
}

//前速后退
void  backrun (void)
{
    Left_moto_back() ;   //左电机往后走
    Right_moto_back() ;  //右电机往后走
}

//右转
void  rightrun (void)
{
    Left_moto_go() ;   //左电机往前走
    Right_moto_back() ;  //右电机往后走
}

//左转
void   leftrun (void)
{


    Left_moto_back() ;   //左电机往前走
    Right_moto_go() ;  //右电机往前走
}
//STOP
void  stoprun (void)
{
    Left_moto_Stop() ;   //左电机往前走
    Right_moto_Stop() ;  //右电机往前走
}


//sirv-sdk cannot use scanf
static unsigned int uart_recv ( void )
{
    unsigned int data;
    while(1)
    {
        data=UART0_REG(UART_REG_RXFIFO);
        if((data&0x80000000)==0) break;
    }
    return(data&0xFF);
}

static void uart_send ( unsigned int data )
{
    while(1)
    {
        if(((UART0_REG(UART_REG_TXFIFO))&0x80000000)==0) break;
    }
    UART0_REG(UART_REG_TXFIFO) = data;
}

int main(int argc, char **argv)
{
  unsigned char uart_buf[10]={0},ra;
  uint8_t start_recv = 0, recv_cnt = 0;
  GPIO_REG(GPIO_INPUT_EN) |= (0x1<< UART0_RX_OFFSET);

  GPIO_REG(GPIO_OUTPUT_EN) |= ((0x1<< LEFT_FRONT_GPIO) | (0x1<< LEFT_BACK_GPIO) | (0x1 << RIGHT_FRONT_GPIO) | (0x1 << RIGHT_BACK_GPIO));
  GPIO_REG(GPIO_OUTPUT_VAL) &= ~((0x1<< LEFT_FRONT_GPIO) | (0x1<< LEFT_BACK_GPIO) | (0x1 << RIGHT_FRONT_GPIO) | (0x1 << RIGHT_BACK_GPIO));
  
  printf("%s\r\n",instructions_msg_sirv);

  while (1){
    ra = uart_recv();
    // uart_send(ra);
    if(start_recv == 1)
    {
      // printf("afdafs");
      uart_buf[recv_cnt++] = ra;
      if(recv_cnt>2)
      {
        uart_buf[recv_cnt]=0;
        if (uart_buf[0] == 'O' && uart_buf[1] == 'N')	//第一个字节为O，第二个字节为N，第三个字节为控制码
        {
              switch (uart_buf[2])
                {
                case up :						    // 前进
                    printf("%s",GoFront);
                    run();
                    break;
                case down:						// 后退
                    printf("%s",GoBack);
                    backrun();
                    break;
                case left:						// 左转
                    printf("%s",GoLeft);
                    leftrun();
                    break;
                case right:						// 右转
                    printf("%s",GoRight);
                    rightrun();
                    break;
                case stop:						// 停止
                    printf("%s",Stop);
                    stoprun();
                    break;
                }
        }
        start_recv = 0;
        for(recv_cnt=0;recv_cnt<10;recv_cnt++)
        {
          uart_buf[recv_cnt] = 0x0;
        }
      }
    }
    if(ra == 'O' && start_recv == 0x0)
    {
      start_recv = 1;
      recv_cnt = 0;
      uart_buf[recv_cnt++] = ra;
    }
  }

  return 0;

}
