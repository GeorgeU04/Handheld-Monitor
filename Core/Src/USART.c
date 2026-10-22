#include "USART.h"
#include <stddef.h>

void USART2Init(const uint32_t baud) {
  RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; // Enable USART2
  RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;     // Clear clock selection
  RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;    // Set system clock as USART2 clock
  // Set 1 Start bit, 8 data bits, n stop bits
  USART2->CR1 &= ~USART_CR1_M0;
  USART2->CR1 &= ~USART_CR1_M1;
  USART2->CR1 |= USART_CR1_TE;          // Enable transmit mode
  USART2->CR1 &= ~USART_CR1_OVER8;      // Set oversampling by 16
  USART2->CR1 &= ~USART_CR1_PCE;        // Disable parity control
  USART2->CR2 &= ~USART_CR2_MSBFIRST;   // Disable MSB first
  USART2->CR2 &= ~USART_CR2_STOP;       // 1 stop bit
  USART2->BRR = SystemCoreClock / baud; // Set baud rate
  USART2->CR1 |= USART_CR1_UE;          // Enable USART
}

void USART2SendByte(const uint8_t byte) {
  USART2->CR1 |= USART_CR1_TE;
  USART2->TDR = byte;
  while (!(USART2->ISR & USART_ISR_TC))
    ;
};

void USART2SendString(const uint8_t *string) {
  USART2->CR1 |= USART_CR1_TE;
  while (*string) {
    USART2->TDR = *string++;
    while (!(USART2->ISR & USART_ISR_TC))
      ;
  }
}
