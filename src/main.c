/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/* Zephyr */
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* Application */
#include "led.h"
#include "button.h"
#include "led_thread.h"
#include "logging_thread.h"

int main(void)
{
	int led_ret, button_ret;
	
	/* Initialize hardware resources */
	led_ret = led_init();
	if (led_ret < 0) {
		printk("[MAIN] LED initialization failed: %d\n", led_ret);
		return led_ret;
	}
	
	button_ret = button_init();
	if (button_ret < 0) {
		printk("[MAIN] Button initialization failed: %d\n", button_ret);
		return button_ret;
	}

	/* Initialize application infrastructure */
	fifo_init();
	led_thread_init();
	log_thread_init();

	/* Log application start */
	fifo_producer(LOG_SRC_MAIN, LOG_EVT_INIT, 0);

	printk("[MAIN] Application initialized successfully\n");

	return 0;
}