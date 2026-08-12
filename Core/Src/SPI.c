#include "SPI.h"

void initSPI1(void) {
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable SPI1
  SPI1->CR1 &= ~SPI_CR1_BIDIMODE;     // Enable 2 line unidirectional data mode
  SPI1->CR1 &= ~SPI_CR1_RXONLY;       // Enable full duplex
  SPI1->CR1 |= SPI_CR1_SSM;           // Enable software slave management
  SPI1->CR1 |= SPI_CR1_SSI;           // Keep internal NSS high
  SPI1->CR1 &= ~SPI_CR1_LSBFIRST;     // Set data to be MSB first
  SPI1->CR1 &= ~SPI_CR1_BR_Msk; // Clear baud rate control for prescaler of 2
  SPI1->CR1 |= SPI_CR1_MSTR;    // Enable Master config
  SPI1->CR1 &= ~SPI_CR1_CPOL;   // Set CPOL to low
  SPI1->CR1 &= ~SPI_CR1_CPHA;   // Set first clock transition as data capture
  SPI1->CR2 &= ~SPI_CR2_DS_Msk; // Clear data size
  SPI1->CR2 |= 0b0111 << SPI_CR2_DS_Pos; // Set 8 bit data size
  SPI1->CR2 &= ~SPI_CR2_FRF;             // Set frame format as Motorola
  SPI1->CR2 |= SPI_CR2_FRXTH; // Set RXNE to generate if FIFO level is >= 1/4
  SPI1->CR1 |= SPI_CR1_SPE;   // Enable SPI
}
