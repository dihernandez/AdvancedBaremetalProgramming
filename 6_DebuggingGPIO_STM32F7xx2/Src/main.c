#include "stm32f7xx.h"
#include "uart.h"
#include <stdio.h>

#define GPIOAEN		(1U<<0)
#define PIN0		(1U<<0)
#define BTN_PIN		PIN0

uint8_t button_state;

int main() {

	// Push Button B1 is PA0
	// GPIOA is on bus AHB1
	/*Enable clock access to GPIOC*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA0 as input pin*/
	GPIOA->MODER &= ~(1U<<0);
	GPIOA->MODER &= ~(1U<<1);

	while(1) {
		/*Check if BTN is pressed*/
		/*ush button is active low*/
		if(GPIOA->IDR & BTN_PIN) {
			button_state = 0;
		} else {
			button_state = 1;
		}

		for(int i = 0; i < 90000; i++) {}
	}

}
