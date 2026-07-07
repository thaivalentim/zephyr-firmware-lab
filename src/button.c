/*
Button module: debounced GPIO button input with interrupt-driven event publishing.
Imposes a 50ms debounce delay to suppress mechanical noise and double-presses.
*/

/*Zephyr*/
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

/* Application */
#include "app_events.h"
#include "logging_thread.h"

/* GPIO device tree specification for button */
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);

/* GPIO callback registration structure */
static struct gpio_callback button_cb;

/* Debounce state: timestamp of last valid press */
static int64_t last_press_time = 0;

/* Debounce interval in milliseconds */
#define DEBOUNCE_MS 50

/* Button press callback: debounced and published as event */
static void button_pressed( 
                        const struct device *dev, 
                        struct gpio_callback *cb,
                        gpio_port_pins_t pins) {
    ARG_UNUSED(dev);
    ARG_UNUSED(cb);
    ARG_UNUSED(pins);

    int64_t now = k_uptime_get();
    
    /* Reject press if within debounce window */
    if (now - last_press_time < DEBOUNCE_MS) {
        return;
    }
    
    last_press_time = now;
    
    app_event_publish();
    fifo_producer(LOG_SRC_BUTTON, LOG_EVT_BUTTON_PRESS, 0);
}

/* Initialize button GPIO and interrupt handler */
int button_init(void) {
    if (!device_is_ready(button.port)) {
        printk("[BUTTON] Device not ready\n");
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret < 0) {
        printk("[BUTTON] Failed to configure GPIO: %d\n", ret);
        return ret;
    }

    /* Initialize callback structure */
    gpio_init_callback(
                    &button_cb,
                    button_pressed,
                    BIT(button.pin)
    );

    /* Register callback with GPIO driver */
    ret = gpio_add_callback(button.port, &button_cb);
    if (ret < 0) {
        printk("[BUTTON] Failed to add callback: %d\n", ret);
        return ret;
    }

    /* Enable interrupts on button pin */
    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0) {
        printk("[BUTTON] Failed to configure interrupt: %d\n", ret);
        return ret;
    }

    return 0;
}