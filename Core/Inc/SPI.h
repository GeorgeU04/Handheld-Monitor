#ifndef SPI_H
#define SPI_H

#include "main.h"
#include <stddef.h>
#include <stdint.h>

void SPI1init(void);
void SPI1Send(const uint8_t *data, size_t size);
void SPI1Read(uint8_t *buffer, size_t size);
uint8_t SPI1Transfer(uint8_t tx);
#endif // !SPI_H
