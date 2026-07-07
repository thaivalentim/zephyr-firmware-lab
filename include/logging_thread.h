#pragma once

#include <zephyr/kernel.h>

typedef enum {
    LOG_SRC_MAIN,
    LOG_SRC_BUTTON,
    LOG_SRC_LED_THREAD,
} log_source_t;

typedef enum {
    LOG_EVT_INIT,
    LOG_EVT_BUTTON_PRESS,
    LOG_EVT_MODE_CHANGE,
    LOG_EVT_LED_TOGGLE,
    LOG_EVT_ERROR,
    LOG_EVT_INFO,
} log_event_t;

void fifo_init(void);
void log_thread_init(void);
void fifo_producer(log_source_t source, log_event_t event, int32_t value);
