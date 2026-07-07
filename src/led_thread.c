/*
LED control thread: manages LED blinking modes with state machine.
Modes transition cyclically: OFF -> SLOW -> MEDIUM -> FAST -> OFF.
Each button press advances to the next mode.
*/

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "led_thread.h"
#include "app_events.h"
#include "led.h"
#include "logging_thread.h"

/* Thread configuration */
#define STACK_SIZE 500
#define PRIORITY 3

K_THREAD_STACK_DEFINE(led_thread_stack, STACK_SIZE);

static struct k_thread led_thread;

/* Current operational mode of the LED */
static led_mode_t current_mode = LED_OFF;

/*
Compute the next LED mode in the cycle.
Transitions: OFF -> SLOW -> MEDIUM -> FAST -> OFF
*/
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

/*
Get the blink period (toggle interval) for each mode.
- OFF: infinite (no timeout, waits for button)
- SLOW: 1 second (0.5s on, 0.5s off)
- MEDIUM: 500ms (0.25s on, 0.25s off)
- FAST: 200ms (0.1s on, 0.1s off)
*/
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

    printk("[LED THREAD] Running\n");

    while (1) {
        switch (current_mode) {
        case LED_SLOW:
        case LED_MEDIUM:
        case LED_FAST: {
            /* Blink modes: toggle and check for event */
            led_toggle();
            
            int ret = app_event_take(mode_timeout(current_mode));
            if (ret == 0) {
                /* Button pressed: transition to next mode */
                current_mode = next_mode(current_mode);
                fifo_producer(LOG_SRC_LED_THREAD, LOG_EVT_MODE_CHANGE, current_mode);
            }
            break;
        }
        case LED_OFF:
            /* OFF mode: ensure LED is off and wait indefinitely */
            led_off();

            int ret = app_event_take(K_FOREVER);
            if (ret == 0) {
                /* Button pressed: transition to next mode (SLOW) */
                current_mode = next_mode(current_mode);
                fifo_producer(LOG_SRC_LED_THREAD, LOG_EVT_MODE_CHANGE, current_mode);
            }
            break;
        default:
            /* Invalid state: recover to OFF */
            printk("[LED THREAD] Invalid mode: %d, recovering\n", current_mode);
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