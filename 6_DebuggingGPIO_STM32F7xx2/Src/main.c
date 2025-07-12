#include "stm32f7xx.h"
#include "uart.h"
#include "led.h"
#include <stdio.h>

#define GPIOAEN		(1U<<0)
#define PIN0		(1U<<0)
#define BTN_PIN		PIN0

uint8_t button_state;

int main() {

	// Push Button B1 is PA0
	// GPIOA is on bus AHB1
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;

	/*Set PA0 as input pin*/
	GPIOA->MODER &= ~(1U<<0);
	GPIOA->MODER &= ~(1U<<1);

	led_init();
	while(1) {
		/*Check if BTN is pressed*/
		/*push button is active high*/
		if(GPIOA->IDR & BTN_PIN) {
			button_state = 1;
			toggle_led();
		} else {
			button_state = 0;
		}

		for(int i = 0; i < 90000; i++) {}
	}

}
