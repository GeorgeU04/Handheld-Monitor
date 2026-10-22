#ifndef SPI_H
#define SPI_H

#include "main.h"
#include <stddef.h>
#include <stdint.h>

void initSPI1(void);
void SPI1Send(const uint8_t *data, size_t size);
void SPI1Read(uint8_t *buffer);
#endif // !SPI_H
