#include "ADC.h"

#include "stm32l476xx.h"

#include <stdint.h>

void ADC_Wakeup(void) {
    int wait_time;

    // To start ADC operations, the following sequence should be applied
    // DEEPPWD = 0: ADC not in deep-power down
    // DEEPPWD = 1: ADC in deep-power-down (default reset state)
    if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD)
        ADC1->CR &= ~ADC_CR_DEEPPWD; // Exit deep power down mode if still in that state

    // Enable the ADC internal voltage regulator
    // Before performing any operation such as launching a calibration or enabling the ADC, the ADC
    // voltage regulator must first be enabled and the software must wait for the regulator start-up
    // time.
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage regulator start-up time
    // The software must wait for the startup time of the ADC voltage regulator (T_ADCVREG_STUP)
    // before launching a calibration or enabling the ADC.
    // T_ADCVREG_STUP = 20 us
    wait_time = 20 * (80000000 / 1000000);
    while (wait_time != 0) {
        wait_time--;
    }
}

void ADC_Common_Configuration() {
    // [TODO]

// 1. In the configuration register of the system configuration controller (SYSCFG − >CFGR1),
// enable the I/O analog switch voltage booster.
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;

// 2. Next, modify the ADC common control register.
// (a) Enable VREFINT.
    ADC123_COMMON->CCR |= ADC_CCR_VREFEN;

// (b) Ensure that the clock is not divided.
    ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;

// (c) Ensure that the ADC clock scheme is set to HCLK/1 synchronous clock mode.
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;

// (d) Ensure that all ADCs are operating in independent mode.
    ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;

}

void ADC_Pin_Init(void) {
    // [TODO]

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    
    GPIOA->MODER |= GPIO_MODER_MODE1;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;
	GPIOA->ASCR |= GPIO_ASCR_ASC1;
}

void ADC_Init(void) {
    // [TODO] Enable & Reset ADC Clock
    // 1. Enable the ADC clock.
    RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;

    // 2. Using one of the RCC reset registers, reset the ADC. Then, clear the reset bit.
	RCC->AHB2RSTR |= RCC_AHB2RSTR_ADCRST;
	RCC->AHB2RSTR &= ~RCC_AHB2RSTR_ADCRST;


    // Other ADC Initialization
    ADC_Pin_Init();
    ADC_Common_Configuration();
    ADC_Wakeup();

    // [TODO] Other Configuration
    // 3. Before modifying any of the ADC settings, ensure that it is disabled (just like how we
    // disabled UART before modifying its settings in Lab 4).
	ADC1->CR &= ~ADC_CR_ADEN;       

    // 4. In the ADC configuration register, configure the ADC to have 12-bit resolution and rightalignment.
	ADC1->CFGR &= ~ADC_CFGR_RES;        // 00: 12-bit res
	ADC1->CFGR &= ~ADC_CFGR_ALIGN;      // 0: Right Align

    // 5. In the ADC regular sequence register, set the sequence length to 1. That is, there will be
    // only 1 conversion in the conversion sequence. Then, ensure that channel 6 is used for the
    // first conversion.
	ADC1->SQR1 &= ~ADC_SQR1_L;                          // 0000: 1 conversion 
	ADC1->SQR1 &= ADC_SQR1_SQ1;                         // 0110: Channel 6 for 1st conversion
	ADC1->SQR1 |= ADC_SQR1_SQ1_2 | ADC_SQR1_SQ1_1;

    // 6. In the ADC differential mode selection register, ensure that channel 6 is set to single-ended
    // mode.
	ADC1->DIFSEL &= ~ADC_DIFSEL_DIFSEL_6;    // 0: Single Ended Mode

    // 7. In the ADC sample time register, set the sampling time for channel 6 to be 24.5 ADC
    // clock cycles.
	ADC1->SMPR1 &= ~ADC_SMPR1_SMP6;                         // 011: 24.5 clk cycles
	ADC1->SMPR1 |= ADC_SMPR1_SMP6_1 | ADC_SMPR1_SMP6_0;

    // 8. In the ADC configuration register, ensure that the ADC is in single conversion mode and
    // that hardware trigger detection is disabled.
    ADC1->CFGR &= ~ADC_CFGR_CONT;           // 0: single conversion mode
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;          // 00: Hardware trigger detection Disabled

    // 9. Now that we are done with the configuration, enable the ADC. Before moving on, wait
    // until the ADC is ready (this information is available in the interrupt and status register).
	ADC1->CR |= ADC_CR_ADEN;

    while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) {}       // Wait until the ADC is ready.
    
}
