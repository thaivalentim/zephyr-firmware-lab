#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "led_thread.h"
#include "app_events.h"
#include "led.h"
#include "logging_thread.h"

#define STACK_SIZE 500
#define PRIORITY 3

K_THREAD_STACK_DEFINE(led_thread_stack, STACK_SIZE);

static struct k_thread led_thread;

static led_mode_t current_mode = LED_OFF;

static led_mode_t next_mode(led_mode_t mode)
{
    switch (mode) {
    case LED_OFF:
        return LED_SLOW;
    case LED_SLOW:
        return LED_MEDIUM;
    case LED_MEDIUM:
        return LED_FAST;
    case LED_FAST:
    default:
        return LED_OFF;
    }
}

static k_timeout_t mode_timeout(led_mode_t mode)
{
    switch (mode) {
    case LED_SLOW:
        return K_MSEC(1000);
    case LED_MEDIUM:
        return K_MSEC(500);
    case LED_FAST:
        return K_MSEC(200);
    case LED_OFF:
    default:
        return K_FOREVER;
    }
}

static void led_thread_entry(void *arg1, void *arg2, void *arg3)
{
    ARG_UNUSED(arg1);
    ARG_UNUSED(arg2);
    ARG_UNUSED(arg3);

    printk("[LED THREAD] executando! \n");

    while (1) {
        switch (current_mode) {
        case LED_SLOW:
        case LED_MEDIUM:
        case LED_FAST: {
            led_toggle();
            
            if (app_event_take(mode_timeout(current_mode)) == 0) {
                current_mode = next_mode(current_mode);
                fifo_producer(LOG_SRC_LED_THREAD, LOG_EVT_MODE_CHANGE, current_mode);
            }

            break;
        }
        case LED_OFF:
            led_off();

            if (app_event_take(K_FOREVER) == 0) {
                current_mode = next_mode(current_mode);
                fifo_producer(LOG_SRC_LED_THREAD, LOG_EVT_MODE_CHANGE, current_mode);
            }

            break;
        default:
            printk("[LED THREAD] Modo inválido! \n");
            current_mode = LED_OFF;
            break;
        }
    }
}

void led_thread_init(void) {
    k_thread_create(&led_thread, led_thread_stack, K_THREAD_STACK_SIZEOF(led_thread_stack),
                    led_thread_entry, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);
}