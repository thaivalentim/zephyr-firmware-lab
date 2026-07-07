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
	/* Declarações gerais */
	int led_ret, button_ret;
	
	led_ret = led_init();
	button_ret = button_init();

	/* Inicializa os dispositivos */
	if(led_ret < 0 || button_ret < 0) {
		printk("A inicialização falhou: %d, %d \n", led_ret, button_ret);

		while(1) {
			k_msleep(1000);
		}
	}

	/* Inicializa as threads e a queue */
	fifo_init();
	led_thread_init();
	log_thread_init();

	fifo_producer(LOG_SRC_MAIN, LOG_EVT_INIT, 0);

	printk("[MAIN THREAD] executando! \n");

	return 0;
}