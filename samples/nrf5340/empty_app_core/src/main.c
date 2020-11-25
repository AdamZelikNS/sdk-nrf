/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <drivers/gpio.h>
#include <init.h>
#include <nrf.h>
#include <nrfx.h>

/** @brief Power OFF entire RAM and suspend CPU forever.
 *
 * Function operates only on `register` variables, so GCC will not use
 * RAM after function prologue. This is also true even with optimizations
 * disabled. Interrupts are disabled to make sure that they will never
 * be executed when RAM is powered off.
 *
 * @param reg_begin  Address to `POWER` register of the first NRF_VMC->RAM item
 * @param reg_last   Address to `POWER` register of the last NRF_VMC->RAM item
 */
void disable_ram_and_wfi(register volatile uint32_t *reg_begin,
			 register volatile uint32_t *reg_last)
{
	__disable_irq();

	do {
		*reg_begin = 0;
		reg_begin += sizeof(NRF_VMC->RAM[0]) / sizeof(reg_begin[0]);
	} while (reg_begin <= reg_last);

	__DSB();
	do {
		__WFI();
	} while (1);
}

void main(void)
{
	/* Power off RAM and suspend CPU */
	disable_ram_and_wfi(&NRF_VMC->RAM[0].POWER,
			    &NRF_VMC->RAM[ARRAY_SIZE(NRF_VMC->RAM) - 1].POWER);
}

/** @brief Allow access to specific GPIOs for the network core.
 *
 * Function is executed very early during system initialization to make sure
 * that the network core is not started yet. More pins can be added if the
 * network core needs them.
 */
static int network_gpio_allow(const struct device *dev)
{
	ARG_UNUSED(dev);

	uint32_t i_net;

	if (IS_ENABLED(CONFIG_NCS_SAMPLE_EMPTY_APP_CORE_CHILD_IMAGE_K32SRC_XTAL_ENABLED))
        {
		const uint32_t pin_XL1_num = 0; /* P0.00 */ // #define PIN_XL1 0
		const uint32_t pin_XL2_num = 1; /* P0.01 */ // #define PIN_XL2 1
                
		NRF_P0_S->PIN_CNF[pin_XL1_num] = (GPIO_PIN_CNF_MCUSEL_Peripheral <<
						  GPIO_PIN_CNF_MCUSEL_Pos);
		NRF_P0_S->PIN_CNF[pin_XL2_num] = (GPIO_PIN_CNF_MCUSEL_Peripheral <<
						  GPIO_PIN_CNF_MCUSEL_Pos);
		i_net = 2; /* GPIOs from 2 to 31 will be forwarded */
        }
        else
        {
		i_net = 0; /* GPIOs from 0 to 31 will be forwarded */ 
        }

	/* Allow Network MCU to use all or most of the GPIOs */
	for (uint32_t i = i_net; i < ARRAY_SIZE(NRF_P0_S->PIN_CNF); i++) {
		NRF_P0_S->PIN_CNF[i] = (GPIO_PIN_CNF_MCUSEL_NetworkMCU <<
					GPIO_PIN_CNF_MCUSEL_Pos);
	}

	for (uint32_t i = 0; i < P1_PIN_NUM; i++) {
		NRF_P1_S->PIN_CNF[i] = (GPIO_PIN_CNF_MCUSEL_NetworkMCU <<
					GPIO_PIN_CNF_MCUSEL_Pos);
	}

/* TODO: Remove debug pragmas below */
#if IS_ENABLED(CONFIG_NCS_SAMPLE_EMPTY_APP_CORE_CHILD_IMAGE_K32SRC_XTAL_ENABLED)
#pragma message "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Empty CHILD: XT - Enabled ~ ~ ~ ~ ~ ~ ~ ~ ~ ~"
#else
#pragma message "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ Empty CHILD: XT - Disabled ~ ~ ~ ~ ~ ~ ~ ~ ~ ~"
#endif
	return 0;
}

SYS_INIT(network_gpio_allow, PRE_KERNEL_1, CONFIG_KERNEL_INIT_PRIORITY_OBJECTS);
