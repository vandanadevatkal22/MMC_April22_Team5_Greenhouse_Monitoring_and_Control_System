#include<stdio.h>
#include "main.h"




typedef struct
{
	unsigned int IMR;
	unsigned int EMR;
	unsigned int RTSR;
	unsigned int FTSR;
	unsigned int SWIER;
	unsigned int PR;
} EXTIRegDef;




void config_gpiox(GPIORegDef *gpiox, int pin, int mode, int speed)
{
	switch (mode & 0x3)
	{
		case e_gpi:
			/* Please refer the stm32f4xx.h for more info */
			if (mode & 0x30) /* Configure normal Pull Up / Down mode */
			{
				gpiox->PUPDR = (gpiox->PUPDR & ~(0x3 << (pin * 2))) | ((mode >> 0x4) << (pin * 2));
			}
			else /* Check for External Interrupt configuration */
			{
				/* Select if Interrupt of Event */
				switch ((mode >> 8) & 0x3)
				{
					case e_exti_int:
						EXTI->IMR = EXTI->IMR | (1 << pin);
						break;
					case e_exti_event:
						EXTI->EMR = EXTI->EMR | (1 << pin);
						break;	
					case e_exti_swi:
						EXTI->SWIER = EXTI->SWIER | (1 << pin);
						break;	
				}

				/* Select the Edges for detection */
				if ((mode >> 12) & 1)
				{
					EXTI->RTSR = EXTI->RTSR | (1 << pin);
				}
				if ((mode >> 13) & 1)
				{
					EXTI->FTSR = EXTI->FTSR | (1 << pin);
				}
			}
			break;
		case e_gpo:
			gpiox->OTYPER = (gpiox->OTYPER & ~(0x1 << pin)) | ((mode >> 0x4) << pin);
			break;
		case e_alt_func:
			break;
		case e_analog:
			break;
	}
}

void gpiox_set(GPIORegDef *gpiox, int pin)
{
}

void gpiox_toggle(GPIORegDef *gpiox, int pin)
{
}

int gpiox_read_pin(GPIORegDef *gpiox, int pin, int mode)
{
}

void config_gpio_interrupt(unsigned char irq_number, unsigned char state)
{
	if (state == ENABLE)
	{
		if (irq_number <= 31)
		{
			*NVIC_ISER0 = *NVIC_ISER0 | (1 << irq_number);
		}
		else if (irq_number <= 63)
		{
			*NVIC_ISER1 = *NVIC_ISER1 | (1 << (irq_number % 32));
		}
		else if (irq_number <= 95)
		{
			*NVIC_ISER1 = *NVIC_ISER1 | (1 << (irq_number % 32));
		}
	}
	else
	{
		if (irq_number <= 31)
		{
			*NVIC_ICER0 = *NVIC_ICER0 | (1 << irq_number);
		}
		else if (irq_number <= 63)
		{
			*NVIC_ICER1 = *NVIC_ICER1 | (1 << (irq_number % 32));
		}
		else if (irq_number <= 95)
		{
			*NVIC_ICER1 = *NVIC_ICER1 | (1 << (irq_number % 32));
		}
	}
}

void config_gpio_irq_priority(unsigned char irq_number, unsigned char irq_priority)
{
	// https://developer.arm.com/documentation/dui0497/a/cortex-m0-peripherals/nested-vectored-interrupt-controller/interrupt-priority-registers
	unsigned char ipr_no = irq_number / 4;
	unsigned char byte_offset  = irq_number % 4;
	unsigned char position = (8 * byte_offset) + (8 - NO_PR_BITS_IMPLEMENTED);

	NVIC_IPR_BASE_ADDRESS[ipr_no] |=  (irq_priority << position); 
}


typedef enum
{
	e_gpi,
	e_gpo,
	e_alt_func,
	e_analog,
	/* Please refer the stm32f4xx.h for more info on mode */
	e_exti_int = 1,
	e_exti_event,
	e_exti_swi
} Modes;


void config_gpio_irq_priority(unsigned char irq_number, unsigned char irq_priority);
void config_gpio_interrupt(unsigned char irq_number, unsigned char state);



void gpiox_clear(GPIORegDef *gpiox, int pin)
{
}

void gpiox_write(GPIORegDef *gpiox, int pin, int value)
{
}



static int key_press_flag;

static void init_config(void)
{
	config_rcc(GPIOD);
	config_gpiox(GPIOD, PIN_12, GPIO_OUTPUT_PP, GPIO_SPEED_VERY_HIGH);

	config_rcc(GPIOA);
	config_gpiox(GPIOA, PIN_0, GPIO_IT_FALLING, GPIO_SPEED_VERY_HIGH);

	config_gpio_irq_priority(IRQ_NO_EXTI0, NVIC_IRQ_PR_LVL_0);
	config_gpio_interrupt(IRQ_NO_EXTI0, ENABLE);
}

int main(void)
{
	int wait = LOOP_CYCLES;

	init_config();

	while (1)
	{
		if (key_press_flag)
		{
			if (!wait--)
			{
				wait = LOOP_CYCLES;
				gpiox_toggle(GPIOD, PIN_12);
				key_press_flag = 0;
			}
		}
	}
}



void config_rcc(void *ptr)
{
	if ((GPIORegDef *) ptr == GPIOD)
	{
		RCC->AHB1ENR = RCC->AHB1ENR | 0x8;
	}

	if ((GPIORegDef *) ptr == GPIOA)
	{
		RCC->AHB1ENR = RCC->AHB1ENR | 0x1;
	}
}


void config_rcc(void *ptr);




extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _la_data;

extern uint32_t _sbss;
extern uint32_t _ebss;

// Prototype of main

MEMORY
{
	FLASH(rx):ORIGIN =0x08000000,LENGTH =1024K
	SRAM(rwx):ORIGIN =0x20000000,LENGTH =128K
}


int main(void);

/* Function prototypes of STM32F407x system exception and IRQ handlers */





void exti0_irqhandler(void)
{
	if (EXTI->PR & (1 << PIN_0))
	{
	}
}




typedef struct
{
	unsigned int OSPEEDR;
	unsigned int PUPDR;
	unsigned int IDR;
	unsigned int ODR;
	unsigned int MODER;
	unsigned int OTYPER;
	unsigned int BSRR;
	unsigned int LCKR;
	unsigned int AFRL;
	unsigned int AFRH;
} GPIORegDef; 

SRCS := $(wildcard *.c)
INCS := $(wildcard *.h)
OBJS := $(SRCS:.c=.o)

TARGET = out.bin

ARCH_FLAGS = -mcpu=$(MCPU) -mthumb -mfloat-abi=soft
CFLAGS = -c
LDFLAGS = -T linker_file.ld -Wl,-Map=out.map -nostdlib






/* Output Functions */
void gpiox_set(GPIORegDef *gpiox, int pin);
void gpiox_clear(GPIORegDef *gpiox, int pin);
void gpiox_write(GPIORegDef *gpiox, int pin, int value);
void gpiox_toggle(GPIORegDef *gpiox, int pin);

/* GPIO Registers */

/* RCC Registers */
#define RCC_BASE_ADDR	0x40023800

typedef struct
{
	unsigned int CR;
	unsigned int PLLCFGR;
	unsigned int CFGR;
	unsigned int CIR;
	unsigned int AHB1RSTR;
	unsigned int AHB2RSTR;
	unsigned int AHB3RSTR;
	unsigned int : 32;
	unsigned int APB1RSTR;
	unsigned int APB2RSTR;
	unsigned int : 32;
	unsigned int : 32;
	unsigned int AHB1ENR;
	unsigned int AHB2ENR;
	unsigned int AHB3ENR;
	unsigned int : 32;
	unsigned int APB1ENR;
	unsigned int APB2ENR;
	unsigned int : 32;
	unsigned int : 32;
	unsigned int AHB1LPENR;
	unsigned int AHB2LPENR;
	unsigned int AHB3LPENR;
	unsigned int : 32;
	unsigned int APB1LPENR;
	unsigned int APB2LPENR;
	unsigned int : 32;
	unsigned int : 32;
	unsigned int BDCR;
	unsigned int CSR;
	unsigned int : 32;
	unsigned int SSCGR;
	unsigned int PLLI2SCFGR;
} RCCRegDef;

}> SRAM AT> FLASH


