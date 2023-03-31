#include <stm32f407xx.h>

/*==== USART, UART: Interrupts ====*/
void Sysclk(void);
void write(unsigned int ch);
void print(char *string);
unsigned int receive(void);
void init(void);

/*
Steps for UART Configuration
[+] Enable Power Clock
[+] Enable internal Voltage
[+] HSE ON
[+] Check HSE
[+] HSE as SysCLK
[+] Check HSE is used as sysCLK
[+] Enable Clock for PORT
[+] Select alternate function
[+] Setting the Alternate function
[+] UART CLK enable
[+] Set BAUDRATE
[+] ENABLE TX and RX
[+] Enable TXEIE
[+] Send data to data register
*/
void Sysclk(void)
{
	// Power Clock Enable
	RCC->AHB1ENR |= (1<<128); 
	// HSE Clock Enable
	RCC->CR |= (1<<16);
	// HSE Flag 
	while((RCC->CR & (1<<17)) == 0);
	// PLL ON  
	RCC->CR |= (1<<24);
	// PLL Source
	RCC->PLLCFGR |= (1<<22); 
	// Setting M & N Value 64 MHz
	RCC->PLLCFGR = (4<<0) | (64<<6);
	// PLL Flag 
	while((RCC->CR & (1<<25)) == 0);
	// PLL as System CLK 
	RCC->CFGR |= (1<<1);
	// AHB1 DIV Value 
	RCC->CFGR |= (9<<4);
	// PLL as System CLK Flag
	while((RCC->CFGR & (1<<3)) == 0);
}

/*
[+] UART CLK enable
[+] Enable Clock for PORTA [GPIOA CLK]
[+] Setting the Alternate function for PA2 TX, PA3 RX
[+] Set MODER Register 
[+] Set Boud Rate at 16MHz
[+] ENABLE TE and RE
[+] ENABLE UE
[+] TXEIE RXEIE Interrupts
*/
void init(void)
{
	RCC->APB1ENR |= (1<<17);
	RCC->AHB1ENR |= (1<<0);
	GPIOA->AFR[0] = (7<<8)|(7<<12);
	GPIOA->MODER = (1<<5)|(1<<7);
	USART2->BRR = 0x0683;
	USART2->CR1 = (1<<2)|(1<<3);
	USART2->CR1 |= (1<<13);
	USART2->CR1 = (1<<5)|(1<<7);
	
}
void write(unsigned int ch)
{
	USART2->DR = (ch & 0xFF);
	while((USART2->SR & (1<<7)) == 0);
}

void print(char *string)
{
	while(*string)
		write(*string++);

}
unsigned int receive(void)
{
	unsigned int mydata;
	while((USART2->SR & (1<<5)) == 0);
	mydata = USART2->DR;
	return mydata;
}

int main (void)
{
	Sysclk();
	init();
	while(1)
{
	unsigned int data = receive();
	write(data); //recive from USART
	print("Hello World"); //Transmit from USART
}
}