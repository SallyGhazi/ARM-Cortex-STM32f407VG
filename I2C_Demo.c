#include <stm32f407xx.h>

/*====  I2C (inter-integrated circuit) ====*/
void Sysclk(void);
void initI2C(void);
void Start(void);
void write(unsigned int data);
void address(unsigned int add);
void Stop(void);
void print(unsigned int *data , unsigned int size);
void delay_MS(int delay);

void Sysclk(void)
{
	// Power Clock Enable
	RCC->AHB1ENR |= (1<<28); 
	// HSE Clock Enable
	RCC->CR |= (1<<16);
	// HSE Flag 
	while((RCC->CR & (1<<17)) == 0);
	// PLL ON  
	RCC->CR |= (1<<24);
	// PLL Source
	RCC->PLLCFGR |= (1<<22); 
	// Setting M & N Value 64 MHz
	RCC->PLLCFGR = 0x00401004; //(4<<0) | (64<<6);
	// PLL Flag 
	while((RCC->CR & (1<<24)) == 0);
	// PLL as System CLK 
	RCC->CFGR |= (1<<1);
	// AHB1 DIV Value 
	RCC->CFGR |= (9<<4);
	// PLL as System CLK Flag
	while((RCC->CFGR & (1<<3)) == 0);
}

/*
Steps for I2C Configuration

[+] SysCLK Configure
[+] Configure GPIO with Alternate Function
[+] Initialize I2C
[+] I2C Start Condition
[+] I2C Address
[+] I2C Write
[+] I2C Stop Condition
[+] I2C String
=========================================
[+] I2C CLK Enable
[+] Enable CLK for GPIO
[+] Select Alternate Function
[+] Select the Moder for GPIO
[+] Set APB Frequency
[+] Give CCR Value
[+] Set Trise Value
[+] Enable I2C
*/
void initI2C(void)
{
	RCC->APB1ENR |= (1<<21);
	RCC->AHB1ENR |= (1<<1);
	GPIOA->AFR[1] = (4<<0)|(4<<4);
	GPIOA->MODER = (2<<16)|(2<<18);
	I2C1->CR2 = (16<<0);
	I2C1->CCR = (80<<0);
	I2C1->TRISE = (17<<0);
	I2C1->CR1 = (1<<0);
}

void Start(void)
{
	I2C1->CR1 |= (1<<8);
	while((I2C1->SR1 & (1<<0)) == 0);
}

void address(unsigned int add)
{
	I2C1->DR = (add & 0xFF);
	while((I2C1->SR1 & (1<<1)) == 0);
	unsigned int temp = I2C1->SR1 | I2C1->SR2;
	
}
void write(unsigned int data)
{
	I2C1->DR = (data & 0xFF);
	while((I2C1->SR1 & (1<<7)) == 0);
}

void Stop(void)
{
	I2C1->CR1 |= (1<<9);

}
void print(unsigned int *data , unsigned int size)
{
	while((I2C1->SR1 & (1<<7)) == 0);
	while(size)
  {
		while((I2C1->SR1 & (1<<7)) == 0);
		I2C1->DR = (volatile unsigned int)*data++;
		size--;
	}
	while((I2C1->SR1 & (1<<2)) == 0);
}


void delay_MS(int delay)
{
	int i;
	for( ;delay>>0 ; delay--)
	for(i=0;i<3195;i++);
}


int main (void)
{
	Sysclk();
	initI2C();
	while(1)
  {
		Start();
		address(0x4E);
		write(0x01);
		Stop();
		delay_MS(25);
  }
}