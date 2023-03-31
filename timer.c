#include <stm32f407xx.h>

/*==== Timer Setup and Creating a Precise Delay ====*/
void Sysclk(void);
void GPIO(void);
void delay_MS(unsigned int time);


/*
Steps for SysCLK Configuration
[+] Power interface CLK enable
[+] Enable the Voltage Regulator
[+] Enable Crystal (HSE)
[+] Check that the HSE is working
[+] Setting the PLL multiplier and divider value
[+] Enable the PLL as System Clock
[+] Check that PLL attains the desired Frequency
[+] Set the values for AHB, APB1, and APB2 pre-scalars.
[+] Check PLL is used as System Clock.
*/
void Sysclk(void)
{
	RCC->AHB1ENR |= (1<<128);
	RCC->CR |= (1<<16);
	while((RCC->CR & (1<<17)) == 0);
	RCC->CR |= (1<<24);
	RCC->PLLCFGR |= (1<<22);
	RCC->PLLCFGR = 0x00401004;
	while((RCC->CR & (1<<25)) == 0);
	RCC->CFGR |= (1<<1);
	RCC->CFGR |= (9<<4);
	while((RCC->CFGR & (1<<3)) == 0);
}

void GPIO(void)
{
	RCC->AHB1ENR |= (1<<3);
	GPIOD->MODER = (1<<24)|(1<<26)|(1<<28)|(1<<30);
}

void delay_MS(unsigned int time)
{
	RCC->AHB1ENR |= (1<<0);
	TIM2->PSC = 15999;
	TIM2->CR1 |= (1<<0);
	TIM2->EGR |= (1<<0);
	while((TIM2->SR & (1<<0)) == 0);
	while(TIM2->CNT < time);
}

int main (void)
{
	Sysclk();
	GPIO();
	while(1)
{
	GPIOD->BSRR = (1<<12)|(1<<13)|(1<<14)|(1<<15);
	for(int i = 0; i<5000000; i++);
	GPIOD->BSRR = (1<<28)|(1<<29)|(1<<30)|(1<<31);
	delay_MS(250);


}
	
}