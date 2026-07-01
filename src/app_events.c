#include "app_events.h"

/* Cria o objeto de evento em tempo de compilação*/
K_EVENT_DEFINE(app_events);

typedef uint32_t app_event_t; /* Tipo do evento */

/* Permite que outros módulos publiquem eventos */
void app_event_publish(uint32_t events)
{
    k_event_post(&app_events, events);
}

/* Aguarda por determinado evento */
app_event_t app_event_wait(uint32_t events)
{
    return k_event_wait(
        &app_events,
        events,
        true,
        K_FOREVER
    );
}