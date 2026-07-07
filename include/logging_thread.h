#pragma once

#include <zephyr/kernel.h>
#include <stdint.h>

/* Log message source */
typedef enum {
    LOG_SRC_MAIN,
    LOG_SRC_BUTTON,
    LOG_SRC_LED_THREAD,
} log_source_t;

/* Log event type */
typedef enum {
    LOG_EVT_INIT,
    LOG_EVT_BUTTON_PRESS,
    LOG_EVT_MODE_CHANGE,
    LOG_EVT_LED_TOGGLE,
    LOG_EVT_ERROR,
    LOG_EVT_INFO,
} log_event_t;

/* Log message structure */
typedef struct {
    void *fifo_reserved;   /* Reserved for FIFO internal use */
    log_source_t source;   /* Source of the log message */
    log_event_t event;     /* Event type */
    int32_t value;         /* Contextual value (e.g., LED mode) */
    uint32_t timestamp;    /* Timestamp in milliseconds */
} log_message_t;

/* Initialize the FIFO queue */
void fifo_init(void);

/* Initialize the logging thread */
void log_thread_init(void);

/* Produce a log message (thread-safe from ISR) */
void fifo_producer(log_source_t source, log_event_t event, int32_t value);
