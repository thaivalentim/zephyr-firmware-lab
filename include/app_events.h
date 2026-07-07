#pragma once

#include <zephyr/kernel.h>

void app_event_publish(void);

int app_event_take(k_timeout_t timeout);