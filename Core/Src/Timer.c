#include "Timer.h"

#include "stm32l432xx.h"
#include <stddef.h>
#include <stdint.h>

void TIMER2Init(void) {
  RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enable TIMER2
  TIM2->PSC = (SystemCoreClock / 1e6) - 1U;
  TIM2->ARR = UINT32_MAX;
  TIM2->EGR = TIM_EGR_UG; // Load PSC into the active prescaler
  TIM2->SR = 0;           // Clear the update flag raised by EGR
}

void delayUS(uint32_t delay) {
  TIM2->CNT = 0;            // Set count to 0
  TIM2->CR1 |= TIM_CR1_CEN; // Start Timer
  while (TIM2->CNT < delay)
    ;
}

void delayMS(uint32_t delay) {
  for (size_t i = 0; i < delay; i++) {
    delayUS(1000);
  }
}
