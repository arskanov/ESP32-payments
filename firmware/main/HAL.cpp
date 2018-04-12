/*
 * HAL.cpp
 *
 *  Created on: 30 Jan 2018
 *      Author: artturi
 */

#include "HAL.h"

extern "C" {
	#include "driver/gpio.h"
}


#define LED_BUILTIN 2

#define BLINK_GPIO (gpio_num_t)LED_BUILTIN

void hal_init_builtin_led() {
	/* Config LED GPIO */
	gpio_pad_select_gpio(BLINK_GPIO);
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_level(BLINK_GPIO, 0);
}

void hal_toggle_builtin_led() {
	static int pinstate = 0;
	pinstate = !pinstate;
	gpio_set_level(BLINK_GPIO, pinstate);
}

void hal_set_builtin_led(bool state) {
	gpio_set_level(BLINK_GPIO, state);
}
