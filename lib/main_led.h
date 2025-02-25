#ifndef MAIN_LED
#define MAIN_LED

#include <stdint.h>

void led_init(uint32_t led_pin);
void led_blink(uint32_t led_pin, uint32_t random);

#endif  // MAIN_LED