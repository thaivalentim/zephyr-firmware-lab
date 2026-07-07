/*
Logging thread module: event log buffering and output.
Uses FIFO queue and memory slab for thread-safe, ISR-safe event logging.
Consumer thread dequeues and outputs messages to console in order.
*/

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "logging_thread.h"

/* Logging thread configuration */
#define STACK_SIZE 500
#define PRIORITY 2

K_THREAD_STACK_DEFINE(log_stack, STACK_SIZE);

static struct k_thread log_thread;

/* FIFO queue for log message buffering */
struct k_fifo queue;

/* Memory slab for log message allocation */
K_MEM_SLAB_DEFINE(log_slab, sizeof(log_message_t), 10, 4);

/* Producer: allocate and queue a log message (safe from ISR context) */
void fifo_producer(log_source_t source, log_event_t event, int32_t value) {
    log_message_t *tx_data;

    if (k_mem_slab_alloc(&log_slab, (void **) &tx_data, K_NO_WAIT) == 0) {
        tx_data->source = source;
        tx_data->event = event;
        tx_data->value = value;
        tx_data->timestamp = k_uptime_get_32();

        k_fifo_put(&queue, tx_data);
    }
}

/* Consumer thread: dequeues log messages and outputs them */
void fifo_consumer(void *p1, void *p2, void *p3) {
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    log_message_t *rx_data;

    printk("[LOGGING THREAD] Started\n");

    while (1) {
        rx_data = k_fifo_get(&queue, K_FOREVER);

        /* Format and print log message */
        printk("[LOG] src=%u evt=%u val=%d ts=%u ms\n", 
                rx_data->source,
                rx_data->event,
                rx_data->value,
                rx_data->timestamp);
        
        /* Free allocated memory back to slab */
        k_mem_slab_free(&log_slab, (void **)&rx_data);
    }
}

/* Initialize FIFO queue */
void fifo_init(void) {
    k_fifo_init(&queue);
}

/* Initialize logging thread */
void log_thread_init(void) {
    k_thread_create(&log_thread, log_stack, STACK_SIZE,
                    fifo_consumer, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);
}