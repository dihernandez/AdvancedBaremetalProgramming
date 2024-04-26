#include "stm32f4xx.h"
#include "uart.h"
#include <stdio.h>


int main(void) {

	uart2_txt_init();
	led_init();

	while(1) {
		printf("Hello from main function\n\r");
	}

}
