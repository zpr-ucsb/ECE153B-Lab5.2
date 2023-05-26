#include "stm32l476xx.h"

static void DAC_Pin_Init(void) {
    // [TODO]
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    
    GPIOA->MODER |= GPIO_MODER_MODE4;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD4;
}

void DAC_Write_Value(uint32_t value) {
    // [TODO] Write DAC1Ch1 right-aligned 12-bit value
    DAC->DHR12R1 = value & DAC_DHR12R1_DACC1DHR; 
}

void DAC_Init(void) {
    DAC_Pin_Init();

    // [TODO] Enable DAC clock
    RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;

    //Before modifying any of the DAC settings, ensure that it is disabled
    DAC->CR &= ~DAC_CR_EN1;

    // [TODO] Enable software trigger mode
    DAC->CR &= ~DAC_CR_TSEL1;       //111: Software Trigger
    DAC->CR |= DAC_CR_TSEL1_2 | DAC_CR_TSEL1_1 | DAC_CR_TSEL1_0;

    // [TODO] Disable trigger
    DAC->CR &= ~DAC_CR_TEN1;        // 0: Trigger disabled
    
    // [TODO] DAC1 connected in normal mode to external pin only with buffer enabled
    DAC->MCR &= ~DAC_MCR_MODE1;     //000: Normal Mode and connected to external pin with Buffer Enabled

    // [TODO] Enable DAC channel 1
    DAC->CR |= DAC_CR_EN1;

    DAC_Write_Value(0);
}
