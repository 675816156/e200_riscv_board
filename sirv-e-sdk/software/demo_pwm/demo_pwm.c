// See LICENSE for license details.

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include <string.h>
#include "plic/plic_driver.h"
#include "encoding.h"
#include <unistd.h>
#include "stdatomic.h"
// #include "util.h"

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
                  Pwm Demo\r\n\
                  Input RGB Color Value\r\n\
                  such as :0xffffff\r\n\
 ";

const int int_max = 2147483647;
const int int_min = -2147483648;

unsigned char isAlph(char num)
{
	if ((num >= 'A'&& num <= 'Z') || (num >= 'a'&& num <= 'z'))
		return 1;
	else
		return 0;
}

unsigned char isDigit(char num)
{
	if ((num >= '0'&& num <= '9'))
		return 1;
	else
		return 0;
}

char toLower(char num)
{
	char result = num;
	if (num >= 'A' && num <= 'Z')
		result += 32;
	return result;
}

int strtoLL(const char *num_str, char **endptr, int base)
{
	long long result = 0;
	long long value;
	if (!base)
	{
		if (*num_str == '0')
		{
			num_str++;
			if (*num_str == 'x' || *num_str == 'X')
			{
				base = 16;
				num_str++;
			}
			else
				base = 8;
		}
		else
			base = 10;
	}
	while (1)
	{
		if (isAlph(*num_str) || isDigit(*num_str))
		{
			value = isAlph(*num_str) ? toLower(*num_str) - 'a' + 10 : *num_str - '0';
			if (value >= base)
				break;
			result = result*base + value;
			if (result > int_max)
				result = int_max;
			if (result < int_min)
				result = int_min;
			num_str++;
		}
		else
			break;
	}
	if (endptr)
		*endptr = num_str;
	return result;
}

int Mystrtol(const char *num_str, char **endptr, int base)
{
	if (*num_str == '-')
	{
		long result = strtoLL(num_str + 1, endptr, base);
		if (result == int_max)
			return int_min;
		else
			return -result;
	}
	return strtoLL(num_str, endptr, base);
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

#define PWM1_LED_R  19
#define PWM1_LED_G  21
#define PWM1_LED_B  22


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

int main(int argc, char **argv)
{
  unsigned char uart_buf[10]={0},ra;
  uint8_t start_recv = 0, recv_cnt = 0;;
  uint32_t color,R,G,B;

  GPIO_REG(GPIO_INPUT_EN) |= (0x1<< UART0_RX_OFFSET);
  #if 1
  GPIO_REG(GPIO_IOF_SEL)    |= ( (1 << GREEN_LED_OFFSET) | (1 << BLUE_LED_OFFSET) | (1 << RED_LED_OFFSET));
  GPIO_REG(GPIO_IOF_EN )    |= ( (1 << GREEN_LED_OFFSET) | (1 << BLUE_LED_OFFSET) | (1 << RED_LED_OFFSET));
  GPIO_REG(GPIO_OUTPUT_XOR) &= ~( (1 << GREEN_LED_OFFSET) | (1 << BLUE_LED_OFFSET) | (1 << RED_LED_OFFSET));
  #else
  GPIO_REG(GPIO_IOF_SEL)    |= ( (1 << PWM1_LED_R) | (1 << PWM1_LED_G) | (1 << PWM1_LED_B));
  GPIO_REG(GPIO_IOF_EN )    |= ( (1 << PWM1_LED_R) | (1 << PWM1_LED_G) | (1 << PWM1_LED_B));
  GPIO_REG(GPIO_OUTPUT_XOR) &= ~( (1 << PWM1_LED_R) | (1 << PWM1_LED_G) | (1 << PWM1_LED_B));
  #endif

  printf("%s\r\n",instructions_msg_sirv);

  // Set up RGB PWM
  PWM0_REG(PWM_CFG)   = 0;
  // To balance the power consumption, make one left, one right, and one center aligned.
  PWM0_REG(PWM_CFG)   = (PWM_CFG_ENALWAYS) | (PWM_CFG_CMP2CENTER);
  PWM0_REG(PWM_COUNT) = 0;
  // Period is approximately 244 Hz
  // the LEDs are intentionally left somewhat dim, 
  // as the full brightness can be painful to look at.
  PWM0_REG(PWM_CMP0)  = 0;
  PWM0_REG(PWM_CMP1)  = 0xd9;
  PWM0_REG(PWM_CMP2)  = 0xd9;
  PWM0_REG(PWM_CMP3)  = 0xd9;
  R = 0xff;
  while (1){
    delay_ms(10);
    PWM0_REG(PWM_CMP1)  = R;   //G
    PWM0_REG(PWM_CMP2)  = B;   //B
    PWM0_REG(PWM_CMP3)  = G;   //G

    if(R > 0 && B == 0){
      R--;
      G++;
    }
    if(G > 0 && R == 0){
      G--;
      B++;
    }
    if(B > 0 && G == 0){
      R++;
      B--;
    }
  

    // ra = uart_recv();
    // // uart_send(ra);
    // if(start_recv == 1)
    // {
    //   uart_buf[recv_cnt++] = ra;
    //   if(recv_cnt>=6)
    //   {
    //     uart_buf[recv_cnt]=0;
    //     color = Mystrtol(uart_buf,0,16);
    //     printf("color is 0x%06x\r\n",color);
    //     R = color & 0x00ff0000;
    //     R = R >> 16;
    //     G = color & 0x0000ff00;
    //     G = G >> 8;
    //     B = color & 0x000000ff;
    //     printf("R:%02x G:%02x B:%02x\r\n",R,G,B);
    //     PWM0_REG(PWM_CMP1)  = R;   //G
    //     PWM0_REG(PWM_CMP2)  = B;   //B
    //     PWM0_REG(PWM_CMP3)  = G;   //G
    //     start_recv = 0;
    //     for(recv_cnt=0;recv_cnt<10;recv_cnt++)
    //     {
    //       uart_buf[recv_cnt] = 0x0;
    //     }
    //   }
    // }
    // if(ra == 'x' || ra == 'X' && start_recv == 0x0)
    // {
    //   // printf("start receive\r\n");
    //   start_recv = 1;
    //   recv_cnt = 0;
    //   //here we don't save 0x
    // }


  }

  return 0;

}
