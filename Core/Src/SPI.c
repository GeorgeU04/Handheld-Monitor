#include "SPI.h"
#include "stm32l432xx.h"
#include <stddef.h>
#include <stdint.h>

void SPI1init(void) {
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

uint8_t SPI1Transfer(uint8_t tx) {
  while (!(SPI1->SR & SPI_SR_TXE)) // wait until buffer is empty
    ;

  // funny cast to 8 bit as register is 32 but we only want 8
  *(volatile uint8_t *)&SPI1->DR = tx;

  while (!(SPI1->SR & SPI_SR_RXNE)) // wait until buffer is not empty
    ;

  while (SPI1->SR & SPI_SR_BSY)
    ;

  // another funny cast to 8 bit as register is 32
  return *(volatile uint8_t *)&SPI1->DR;
}

void SPI1Send(const uint8_t *data, size_t size) {
  size_t i = 0;
  while (i < size)
    SPI1Transfer(data[i++]);
}

void SPI1Read(uint8_t *buffer, size_t size) {
  size_t i = 0;
  while (i < size)
    buffer[i++] = SPI1Transfer(0xFF);
}
