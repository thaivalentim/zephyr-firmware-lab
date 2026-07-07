#pragma once

typedef enum {
    LED_OFF,
    LED_SLOW,
    LED_MEDIUM,
    LED_FAST
} led_mode_t;

void led_thread_init(void);