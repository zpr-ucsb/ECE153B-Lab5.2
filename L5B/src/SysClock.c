#include "SysClock.h"

void System_Clock_Init(void) {
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_MSI;
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_7;
	RCC->CR |= RCC_CR_MSIRGSEL;
	RCC->CR |= RCC_CR_MSIPLLEN;
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
}
