#include "uart.h"

/*Using USART1 which has PA9 for USART1TX on APB2ENR*/

#define UART1EN		(1U<<4)
#define GPIOAEN		(1U<<0)
#define CR1_TE		(1U<<3)
#define CR1_RE		(1U<<2)
#define CR1_UE		(1U<<0)
#define SR_TXE		(1U<<7)

// LED 2 Blink, PJ5
#define GPIOJEN (1U << 9)
#define PIN5 (1U << 5)
#define LED_PIN PIN5

#define UART_BAUDRATE 115200
#define CLK			  16000000


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static void uart1_write(int ch);
static void toggle_led(void);

int __io_putchar(int ch) {
	uart1_write(ch);
	return ch;
}

void led_init(void) {
    /* Enable clock access to GPIOJ */
    RCC->AHB1ENR |= GPIOJEN;

    /* Set PJ5 to output mode */
    GPIOJ->MODER |= (1U << 10); // Setting MODER5 to 01 for output mode
    GPIOJ->MODER &= ~(1U << 11);
}

void uart1_txt_init(void) {
	/*********************Configure UART GPIO pin*****************************/
	/*1.Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;
	/*2.Set PA9 mode to alternate function mode*/
	GPIOA->MODER &= ~(1U<<18);
	GPIOA->MODER |= (1U<<19);
	/*3.Set PA9 alternate function type to AF7(UART1_TX)*/
	GPIOA->AFR[1] |= (1U<<4);
	GPIOA->AFR[1] |= (1U<<5);
	GPIOA->AFR[1] |= (1U<<6);
	GPIOA->AFR[1] &= ~(1U<<7);
	/*********************Configure UART Module*****************************/
	/*4.Enable clock access to UART1*/
	RCC->APB2ENR |= UART1EN;
	/*5.Set baudrate*/
	uart_set_baudrate(CLK, UART_BAUDRATE);
	/*6.Set transfer direction*/
	USART1->CR1 = CR1_TE; // clear all other bits
	/*7.Enable UART module*/
	USART1->CR1 |= CR1_UE;
}

static void uart1_write(int ch) {
	/*Make sure the transmit data register is empty*/
	while(!(USART1->ISR & SR_TXE)) {}
	/*Write to transmit data register*/
	USART1->TDR = (ch & 0xFF);
	toggle_led();
}


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate) {
	return (periph_clk +( baudrate/2U ))/baudrate;
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate) {
	USART1->BRR = compute_uart_bd(periph_clk, baudrate);
}

static void toggle_led(void) {
    GPIOJ->ODR ^= LED_PIN;
    for (int i = 0; i < 1000000; i++) {} // Delay
}
