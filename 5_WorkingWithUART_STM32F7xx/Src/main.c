#include "stm32f7xx.h"
#include "uart.h"
#include <stdio.h>

int main() {
	uart1_txt_init();
	led_init();

	while(1) {
		printf("Hello from main function\n\r");
	}

}
