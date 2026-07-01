/* 
This aplication have the purpose to control a led with a button,
in order for the developer to learn how Zephyr works and how to use it.
*/

/*Zephyr*/
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* A estrutura obtém informações sobre o pino GPIO
usado para controlar o led por meio da device tree. */
static const struct gpio_dt_spec led = 
                GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

int led_init(void) {
    if(!device_is_ready(led.port)) {
        return -ENODEV; // -ENODEV signifca "No Device"
    }

    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if(ret < 0) {
        return ret; // Retorna o código de erro específico se a configuração do pino falhar
    }

    return 0; // 0 significa sucesso
}

void led_on(void) {
    gpio_pin_set_dt(&led, 1);
}

void led_off(void) {
    gpio_pin_set_dt(&led, 0);
}

void led_toggle(void) {
    gpio_pin_toggle_dt(&led);
}