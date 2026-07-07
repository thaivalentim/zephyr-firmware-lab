#pragma once

/* Initialize LED GPIO */
int led_init(void);

/* Turn LED on */
void led_on(void);

/* Turn LED off */
void led_off(void);

/* Toggle LED state */
void led_toggle(void);