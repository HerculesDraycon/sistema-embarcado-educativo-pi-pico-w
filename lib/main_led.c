#include <stdlib.h>
#include "main_led.h"
#include "pico/stdlib.h"

void led_init(uint32_t pino_led) {
    gpio_init(pino_led);
    gpio_set_dir(pino_led, GPIO_OUT);
}

void pisca_led(uint32_t pino_led, uint32_t random){
    for(int i=0; i<random; i++){
        gpio_put(pino_led, 1);
        sleep_ms(250);
        gpio_put(pino_led, 0);
        sleep_ms(250);
    }
}
