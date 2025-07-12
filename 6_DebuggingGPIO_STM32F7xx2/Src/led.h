#ifndef LED_H
#define LED_H

#include "stm32f7xx.h"  // Adjust as needed for your MCU

// LED 2 Blink, PJ5
#define GPIOJEN (1U << 9)
#define PIN5 (1U << 5)
#define LED_PIN PIN5

/* toggle LED 2 on/off*/
void toggle_led(void);

#endif /* LED_H */
