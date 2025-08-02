#include "led.h"

void toggle_led(void) {
    GPIOJ->ODR ^= LED_PIN;
    for (int i = 0; i < 1000000; i++) {} // Delay
}
