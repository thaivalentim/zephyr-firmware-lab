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
#include "app_events.h"

int main(void)
{
	/* Declarações gerais */
	int led_ret, button_ret;
	//int state; /* Variável usada no polling */

	led_ret = led_init();
	button_ret = button_init();

	printk("Hello World! \n");

	if(led_ret < 0 || button_ret < 0) {
		printk("A inicialização falhou: %d, %d \n", led_ret, button_ret);

		while(1) {
			k_msleep(1000);
		}
	}

	/* Aplicação é reativa aqui. O hardware precisa identificar
	uma interrupção para que a função de callback acione um evento */
	while(1){
		uint32_t event = app_event_wait(APP_EVENT_BUTTON);

		if (event & APP_EVENT_BUTTON) { /* '&' é um operador BitMask */
			printk("Botão pressionado! \n");
			printk("Evento recebido: %d \n", event);
			led_toggle();
		}
	}

	/*
	Implementação de polling para leitura do estado do botão e acionamento do led.
	while (1) {
		state = button_get_state();
		
		if(state < 0) {
			printk("Falha ao ler o estado do botão: %d \n", state);
			printk("Aguardando 1 segundo para tentar novamente. \n");
			k_msleep(1000);

			continue;
		}

		if(state) {
			led_on();
			printk("Botão pressionado e led aceso! \n");
		} else {
			led_off();
		}

		k_msleep(100);
	}
	*/
	
	return 0;
}
