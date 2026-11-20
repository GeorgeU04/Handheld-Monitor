#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "ST7789.h"
#include "font.h"
#include <stdbool.h>
#include <stdint.h>

void writeString(const struct ST7789 *screen, const uint8_t *str,
                 const struct font *font, uint16_t x, uint16_t y,
                 uint16_t color);
void writeChar(const struct ST7789 *screen, uint8_t c, const struct font *font,
               uint16_t x, uint16_t y, uint16_t color);

void drawRectangle(const struct ST7789 *screen, uint16_t x, uint16_t y,
                   uint16_t width, uint16_t height, bool filled,
                   uint16_t color);

void drawHorizontalLine(const struct ST7789 *screen, uint16_t x, uint16_t y,
                        uint16_t length, uint16_t color);
void drawVerticalLine(const struct ST7789 *screen, uint16_t x, uint16_t y,
                      uint16_t length, uint16_t color);
void setBackgroundColor(const struct ST7789 *screen, uint16_t backgroundColor);

extern uint16_t currentBackgroundColor;
#endif // !GRAPHICS_H
