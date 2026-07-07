#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/* Definições da Logging Thread estão abaixo */
#define STACK_SIZE 500
#define PRIORITY 2

K_THREAD_STACK_DEFINE(log_stack, STACK_SIZE);

static struct k_thread log_thread;

/* Declaração do objeto FIFO
para armazenar as mensagens de log */
struct k_fifo queue;

/* Indica qual é a fonte da mensagem */
typedef enum {
    LOG_SRC_MAIN,
    LOG_SRC_BUTTON,
    LOG_SRC_LED_THREAD,
} log_source_t;

/* Indica qual é o evento*/
typedef enum {
    LOG_EVT_INIT,
    LOG_EVT_BUTTON_PRESS,
    LOG_EVT_MODE_CHANGE,
    LOG_EVT_LED_TOGGLE,
    LOG_EVT_ERROR,
    LOG_EVT_INFO,
} log_event_t;

/* Estrutura da mensagem */
typedef struct {
    void *fifo_reserved;
    log_source_t source;
    log_event_t event;
    int32_t value;
    uint32_t timestamp;
} log_message_t;

/* Define um bloco de memória para alocação de mensagens */
K_MEM_SLAB_DEFINE(log_slab, sizeof(log_message_t), 10, 4);

/* Função para módulos produtores*/
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

/* Função para módulos consumidores (logging_thread apenas) */
void fifo_consumer(void *p1, void *p2, void *p3) {
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    log_message_t *rx_data;

    while(1) {
        rx_data = k_fifo_get(&queue, K_FOREVER);

        printk("[LOGGING THREAD] src=%d, evt=%d, val=%d, ts%u ms\n", 
                rx_data-> source,
                rx_data->event,
                rx_data->value,
                rx_data->timestamp);
        
        k_mem_slab_free(&log_slab, (void **)&rx_data);
    }
}

/* Inicializa a queue*/
void fifo_init(void) {
    k_fifo_init(&queue);
}

/* Inicializa a thread */
void log_thread_init(void) {
    k_thread_create(&log_thread, log_stack, STACK_SIZE,
                    fifo_consumer, NULL, NULL, NULL,
                    PRIORITY, 0, K_NO_WAIT);
}