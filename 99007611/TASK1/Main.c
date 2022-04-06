
#include "stm32f4xx.h"
#include "gpio.h"
#include "rcc.h" 

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

static void init_config(void)
{
	config_rcc(GPIOD);
	config_gpiox(GPIOD, PIN_12, GPIO_OUTPUT_PP, GPIO_SPEED_VERY_HIGH);

	config_rcc(GPIOA);
	config_gpiox(GPIOA, PIN_0, GPIO_IT_FALLING, GPIO_SPEED_VERY_HIGH);

	config_gpio_irq_priority(IRQ_NO_EXTI0, NVIC_IRQ_PR_LVL_0);
	config_gpio_interrupt(IRQ_NO_EXTI0, ENABLE);
}
