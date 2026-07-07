/*
LED module: low-level GPIO control abstraction.
Encapsulates the LED device configuration and provides basic control functions.
*/

/*Zephyr*/
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* GPIO device tree specification for LED */
static const struct gpio_dt_spec led = 
                GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

/* Initialize LED GPIO as output */
int led_init(void) {
    if (!device_is_ready(led.port)) {
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        return ret;
    }

    return 0;
}

/* Set LED to ON state */
void led_on(void) {
    gpio_pin_set_dt(&led, 1);
}

/* Set LED to OFF state */
void led_off(void) {
    gpio_pin_set_dt(&led, 0);
}

void led_toggle(void) {
    gpio_pin_toggle_dt(&led);
}