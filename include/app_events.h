#pragma once

#include <zephyr/kernel.h>

enum app_event {
    APP_EVENT_BUTTON = BIT(0),
};

void app_event_publish(uint32_t events);

uint32_t app_event_wait(uint32_t events);