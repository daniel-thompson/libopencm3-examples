/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/scb.h>

static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO4);
}

static void switch_to_bootloader(void)
{
	char * const marker = (char *)0x20004800; /* RAM@18K */
	const char key[] = "remain-in-loader";

	memcpy(marker, key, sizeof(key));
	scb_reset_system(); /* Will never return. */
}

static const char key[] = "remain-in-loader";

int main(void)
{
	int i, j;

	gpio_setup();

	/* Blink the LED (PA4) on the board. */
	for (i = 0; i < 10; i++) {
		gpio_toggle(GPIOA, GPIO4);	/* LED on/off */
		for (j = 0; j < 800000; j++)	/* Wait a bit. */
			__asm__("nop");
	}

	switch_to_bootloader(); /* Will never return. */
	return 0;
}
