#include "graphics.h"
#include "SPI.h"
#include "ST7789.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

uint16_t currentBackgroundColor =
    ST77XX_WHITE; // White is default background color

void writeChar(const struct ST7789 *screen, uint8_t c, const struct font *font,
               uint16_t x, uint16_t y, uint16_t color) {
  setBoundCursor(screen, x, x + font->width - 1, y, y + font->height - 1);
  writeCommand(screen, ST77XX_RAMWR);

  screen->CSPort->BSRR = (1 << screen->CSPin << 16);
  screen->DCPort->BSRR = (1 << screen->DCPin); // Set for data

  int8_t index = getCharIndex(c);
  if (index == -1) { // Treat any error as a space
    for (size_t i = 0; i < font->width * font->height; i++) {
      SPI1Transfer((uint8_t)(currentBackgroundColor >> 8)); // Send high byte
      SPI1Transfer((uint8_t)currentBackgroundColor);        // Send low byte
    }
  } else {
    for (size_t i = 0; i < font->height * font->bytesPerRow; i++) {
      for (size_t j = 0; j < font->width; j++) {
        if (j % 8 == 0 && j != 0)
          i++;
        if ((font->glyph[font->height * index * font->bytesPerRow + i] >>
             (7 - (j % 8))) &
            1U) {
          SPI1Transfer((uint8_t)(color >> 8)); // Send high byte
          SPI1Transfer((uint8_t)color);        // Send low byte
        } else {
          SPI1Transfer(
              (uint8_t)(currentBackgroundColor >> 8));   // Send high byte
          SPI1Transfer((uint8_t)currentBackgroundColor); // Send low byte
        }
      }
    }
  }
  screen->CSPort->BSRR = (1 << screen->CSPin);
};

void writeString(const struct ST7789 *screen, const uint8_t *str,
                 const struct font *font, uint16_t x, uint16_t y,
                 uint16_t color) {
  while (*str) {
    writeChar(screen, *str++, font, x, y, color);
    x += font->width;
  }
}

void setBackgroundColor(const struct ST7789 *screen, uint16_t backgroundColor) {
  currentBackgroundColor = backgroundColor;
  fillSolid(screen, backgroundColor);
}

void drawVerticalLine(const struct ST7789 *screen, uint16_t x, uint16_t y,
                      uint16_t length, uint16_t color) {
  setBoundCursor(screen, x, x, y, y + length - 1);
  writeCommand(screen, ST77XX_RAMWR);
  screen->CSPort->BSRR = (1 << screen->CSPin << 16);
  screen->DCPort->BSRR = (1 << screen->DCPin); // Set for data
  for (size_t i = 0; i < length; i++) {
    SPI1Transfer((uint8_t)(color >> 8)); // Send high byte
    SPI1Transfer((uint8_t)color);        // Send low byte
  }
}

void drawHorizontalLine(const struct ST7789 *screen, uint16_t x, uint16_t y,
                        uint16_t length, uint16_t color) {
  setBoundCursor(screen, x, x + length - 1, y, y);
  writeCommand(screen, ST77XX_RAMWR);
  screen->CSPort->BSRR = (1 << screen->CSPin << 16);
  screen->DCPort->BSRR = (1 << screen->DCPin); // Set for data
  for (size_t i = 0; i < length; i++) {
    SPI1Transfer((uint8_t)(color >> 8)); // Send high byte
    SPI1Transfer((uint8_t)color);        // Send low byte
  }
}

void drawRectangle(const struct ST7789 *screen, uint16_t x, uint16_t y,
                   uint16_t width, uint16_t height, bool filled,
                   uint16_t color) {
  if (filled) {
    setBoundCursor(screen, x, x + width - 1, y, y + height - 1);
    writeCommand(screen, ST77XX_RAMWR);
    screen->CSPort->BSRR = (1 << screen->CSPin << 16);
    screen->DCPort->BSRR = (1 << screen->DCPin); // Set for data
    for (size_t i = 0; i < height * width; i++) {
      SPI1Transfer((uint8_t)(color >> 8)); // Send high byte
      SPI1Transfer((uint8_t)color);        // Send low byte
    }
  } else {
    drawHorizontalLine(screen, x, y, width - 1, color);
    drawHorizontalLine(screen, x, y + height - 1, width - 1, color);
    drawVerticalLine(screen, x, y, height - 1, color);
    drawVerticalLine(screen, x + width - 1, y, height - 1, color);
  }
  screen->CSPort->BSRR = (1 << screen->CSPin);
}
