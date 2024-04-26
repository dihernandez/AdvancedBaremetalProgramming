#include "uart.h"

#define UART2EN		(1U<<17)
#define GPIOAEN		(1U<<0)
#define CR1_TE		(1U<<3)
#define CR1_RE		(1U<<2)
#define CR1_UE		(1U<<13)
#define SR_TXE		(1U<<7)

#define GPIODEN (1U << 3)
#define PIN12 (1U << 12)
#define LED_PIN PIN12


#define UART_BAUDRATE 115200
#define CLK			  16000000


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static void uart2_write(int ch);
static void toggle_led(void);

int __io_putchar(int ch) {
	uart2_write(ch);
	return ch;
}

void led_init(void) {
    /* Enable clock access to GPIOD */
    RCC->AHB1ENR |= GPIODEN;

    /* Set PD12 to output mode */
    GPIOD->MODER |= (1U << 24); // Setting MODER12 to 01 for output mode
    GPIOD->MODER &= ~(1U << 25);
}

void uart2_txt_init(void) {
	/*********************Configure UART GPIO pin*****************************/
	/*1.Enable clock access to GPIOA*/
	RCC->AHB1ENR |= GPIOAEN;
	/*2.Set PA2 mode to alternate function mode*/
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);
	/*3.Set PA2 alternate function type to AF7(UART2_TX)*/
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);
	/*********************Configure UART Module*****************************/
	/*4.Enable clock access to UART2*/
	RCC->APB1ENR |= UART2EN;
	/*5.Set baudrate*/
	uart_set_baudrate(CLK, UART_BAUDRATE);
	/*6.Set transfer direction*/
	USART2->CR1 = CR1_TE; // clear all other bits
	/*7.Enable UART module*/
	USART2->CR1 |= CR1_UE;
}

static void uart2_write(int ch) {
	/*Make sure the transmit data register is empty*/
	while(!(USART2->SR & SR_TXE)) {}
	/*Write to transmit data register*/
	USART2->DR = (ch & 0xFF);
	toggle_led();
}


static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate) {
	return (periph_clk +( baudrate/2U ))/baudrate;
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate) {
	USART2->BRR = compute_uart_bd(periph_clk, baudrate);
}

static void toggle_led(void) {
    GPIOD->ODR ^= LED_PIN;
    for (int i = 0; i < 1000000; i++) {} // Delay
}
