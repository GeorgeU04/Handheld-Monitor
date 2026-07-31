#include "main.h"
#include "USART.h"
#include <stdint.h>

// TODO: Make a real ms delay with timer
void delay(uint32_t delay) {
  while (delay--) {
    __NOP();
  }
}

void initPins(void) {
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;     // Enable GPIOB
  GPIOB->MODER &= ~GPIO_MODER_MODER4;      // Clear mode
  GPIOB->MODER |= GPIO_MODER_MODER4_0;     // Set to output mode
  GPIOB->OTYPER &= ~GPIO_OTYPER_OT4;       // Set to output push-pull
  GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED4; // Set to low speed
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4;       // Set no pull-up no pull-down
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;     // Enable GPIOA
  GPIOA->MODER &= ~GPIO_MODER_MODER2;      // Clear PA2 Mode
  GPIOA->MODER |= GPIO_MODER_MODER2_1;     // Set PA2 as alternate function
  GPIOA->MODER &= ~GPIO_MODER_MODER15;     // Clear PA15 Mode
  GPIOA->MODER |= GPIO_MODER_MODER15_1;    // Set PA15 as alternate function
  GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2;  // Set PA2 as very high speed
  GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED15; // Set PA15 as very high speed
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;       // Set PA2 no pull-up no pull-down
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD15;      // Set PA15 no pull-up no pull-down
  GPIOA->AFR[0] |=
      (0b0111 << GPIO_AFRL_AFSEL2_Pos); // Set alternate function of AF7 for PA2
  GPIOA->AFR[1] |=
      (0b0011
       << GPIO_AFRH_AFSEL15_Pos); // Set alternate function of AF3 for PA15
}

int main(void) {
  initPins();
  USART2Init(115200);
  GPIOB->BSRR |= GPIO_BRR_BR4; // Set to high
  while (1) {
    USART2SendString((uint8_t *)"Hello\r\n");
    delay(10000);
  }
  return 0;
}
