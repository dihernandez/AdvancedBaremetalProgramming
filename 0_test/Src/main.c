#include "stm32f4xx.h"

#define GPIODEN (1U << 3)
#define PIN12 (1U << 12)
#define LED_PIN PIN12

int main() {
    /* Enable clock access to GPIOD */
    RCC->AHB1ENR |= GPIODEN;

    /* Set PD12 to output mode */
    GPIOD->MODER |= (1U << 24); // Setting MODER12 to 01 for output mode
    GPIOD->MODER &= ~(1U << 25);

    while (1) {
        GPIOD->ODR ^= LED_PIN;
        for (int i = 0; i < 1000000; i++) {} // Delay
    }
}
