/*
This aplication have the purpose to control a led with a button,
in order for the developer to learn how Zephyr works and how to use it.
*/

/*Zephyr*/
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

/* Applicação */
#include "app_events.h"
#include "logging_thread.h"

/* A estrutura obtém informações sobre o pino GPIO
usado para controlar o led por meio da device tree. */
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

/* Registro do callback utilizado pelo driver GPIO para notificar eventos do botão */
static struct gpio_callback button_cb;

/* Função de callback chamada quando o botão é pressionado */
static void button_pressed( 
                        const struct device *dev, 
                        struct gpio_callback *cb,
                        gpio_port_pins_t pins) {
    ARG_UNUSED(dev);
    ARG_UNUSED(cb);
    ARG_UNUSED(pins);

    app_event_publish();
    fifo_producer(LOG_SRC_BUTTON, LOG_EVT_BUTTON_PRESS, 0);
}

/* Inicializa o GPIO associado ao botão */
int button_init(void) {
    if(!device_is_ready(button.port)) {
        return -ENODEV; // -ENODEV signifca "No Device"
    }

    int ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if(ret < 0) {
        return ret; // Retorna o código de erro específico se a configuração do pino falhar
    }

    /* Inicializa a estrutura do callback */
    gpio_init_callback(
                    &button_cb,
                    button_pressed,
                    BIT(button.pin)
    );

    /* Registra o callback no driver GPIO */
    ret = gpio_add_callback(button.port, &button_cb);
    if (ret < 0) {
        return ret; // Retorna o código de erro específico se a adição do callback no driver falhar
    }

    /* Habilita interrupções para o botão */
    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0) {
        return ret; // Retorna o código de erro específico se a configuração da interrupção falhar
    }

    return 0; // 0 significa sucesso
}