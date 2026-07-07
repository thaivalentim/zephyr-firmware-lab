#include "app_events.h"

static K_SEM_DEFINE(app_event_sem, 0, K_SEM_MAX_LIMIT);

void app_event_publish(void)
{
    k_sem_give(&app_event_sem);
}

int app_event_take(k_timeout_t timeout)
{
    return k_sem_take(&app_event_sem, timeout);
}