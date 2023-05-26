#include "PWM.h"


// [TODO]
void PWM_Init() {
	// Enable GPIO Port A Clock
	// [TODO]
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// Enable TIM2 Clock
	// [TODO]
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

	// Configure PA5
	// [TODO]
	GPIOA->MODER &= ~(GPIO_MODER_MODE5);        // Clear Port A, Pin 5 MODER
    GPIOA->MODER |= GPIO_MODER_MODE5_1;         // Set Port A, Pin 5 MODER to Alternative Mode
	GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5);	// Set Port A, Pin 5 OSPEEDR to Very High
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5);		// Clear Port A, Pin 5 PUPDR to set it to No Pull Up, No Pull Down
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL5);			// Clear AF1 for Port A, Pin 5
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;			// Choose AF1 (TIM2_CH1) for Port A, Pin 5

	// Configure PWM Output for TIM2 CH1
	// [TODO]
	TIM2->CCER |= TIM_CCER_CC1E;				// Enable CH1 for TIM2, not CH1N

	TIM2->CR1 &= ~(TIM_CR1_DIR);					// (a) [TIM2 CR1] Set the direction such that the timer counts up.
	TIM2->PSC = 50;											// (b) Set the prescaler value.
	TIM2->ARR = 4095;											// (c) Set the auto-reload value.
	TIM2->CCMR1 &= ~(TIM_CCMR1_OC1M);			// (d) 1. Clear the output compare mode bits. 
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1;			// (d) 2. Set the output compare mode bits to PWM mode 1: 0110.
	TIM2->CCMR1 |= TIM_CCMR1_OC1M_2;
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;				// (d) 3. Enable output preload.
	TIM2->CCER &= ~(TIM_CCER_CC1P);				// (e) Set the output polarity for compare 1 to active high.
	TIM2->CCER |= TIM_CCER_CC1E;				// (f) Enable the channel 1 output.
	TIM2->CCR1 = 1;								// (g) [TIM2 CCRx] Set the capture/compare value.
	
	TIM2->CR1 |= TIM_CR1_CEN;					// (h) Enable the counter.


}
 