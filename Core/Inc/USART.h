#ifndef USART_H
#define USART_H

#include "stm32l4xx.h"
#include <stdint.h>

void USART2SendByte(uint8_t byte);
void USART2Init(uint32_t baud);
void USART2SendString(uint8_t *string);
#endif // !USART_H
