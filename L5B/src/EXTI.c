#include "EXTI.h"

#include "DAC.h"

void EXTI_Init(void) {
    // [TODO] Configure EXTI for button'
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;      
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    
    GPIOC->MODER &= ~(GPIO_MODER_MODE13);          

    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13;        
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	EXTI->FTSR1 |= EXTI_FTSR1_FT13;

	EXTI->IMR1 |= EXTI_IMR1_IM13;

	NVIC_EnableIRQ(EXTI15_10_IRQn);		
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
}

#define DAC_MIN 0
#define DAC_MAX 4095
#define DAC_INCREMENT 256

static uint32_t dac_value = 0;
static enum {
    DOWN,
    UP,
} direction = UP;

// [TODO] Interrupt handler
void EXTI15_10_IRQHandler(void) {
	// Clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF13;

    if (dac_value > DAC_MAX - DAC_INCREMENT) {
        direction = DOWN;
    }
    if (dac_value < DAC_INCREMENT + DAC_MIN) {
        direction = UP;
    }

    if (direction == UP) {
        dac_value += DAC_INCREMENT;
    } else {
        dac_value -= DAC_INCREMENT;
    }
    
    DAC_Write_Value(dac_value);     // Write dac_value
}