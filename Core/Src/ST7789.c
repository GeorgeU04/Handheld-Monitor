#include "ST7789.h"
#include "SPI.h"
#include "Timer.h"
#include <stddef.h>
#include <stdint.h>

void initST7789Screen(struct ST7789 *screen, GPIO_TypeDef *CSPort,
                      uint16_t CSPin, GPIO_TypeDef *DCPort, uint16_t DCPin,
                      GPIO_TypeDef *RSTPort, uint16_t RSTPin) {
  screen->CSPort = CSPort;
  screen->CSPin = CSPin;
  screen->DCPort = DCPort;
  screen->DCPin = DCPin;
  screen->RSTPort = RSTPort;
  screen->RSTPin = RSTPin;
  screen->RSTPort->BSRR = (1 << screen->RSTPin);
}

static void writeCommand(const struct ST7789 *screen, uint8_t command) {
  screen->CSPort->BSRR = (1 << screen->CSPin << 16);
  screen->DCPort->BSRR = (1 << screen->DCPin << 16); // Reset for command
  SPI1Send(&command, 1);
  screen->CSPort->BSRR = (1 << screen->CSPin);
}

static void writeData(const struct ST7789 *screen, uint8_t data) {
  screen->CSPort->BSRR = (1 << screen->CSPin << 16);
  screen->DCPort->BSRR = (1 << screen->DCPin); // Set for data
  SPI1Send(&data, 1);
  screen->CSPort->BSRR = (1 << screen->CSPin);
}

void ST7789setup(const struct ST7789 *screen) {
  writeCommand(screen, ST77XX_SWRESET);
  delayMS(150);

  writeCommand(screen, ST77XX_SLPOUT);
  delayMS(10);

  writeCommand(screen, ST77XX_COLMOD);
  writeData(screen, 0x55); // 16-bit color
  delayMS(10);

  writeCommand(screen, ST77XX_MADCTL);
  writeData(screen, ST77XX_MADCTL_RGB); // RGB order, default scan direction

  writeCommand(screen, ST77XX_CASET);
  writeData(screen, 0x00);
  writeData(screen, 0); // XSTART = 0
  writeData(screen, 0);
  writeData(screen, 240); // XEND = 240

  writeCommand(screen, ST77XX_RASET);
  writeData(screen, 0x00);
  writeData(screen, 0); // YSTART = 0
  writeData(screen, 0x01);
  writeData(screen, 0x40); // YEND = 320

  writeCommand(screen, ST77XX_INVON);
  delayMS(10);

  writeCommand(screen, ST77XX_NORON);
  delayMS(10);

  writeCommand(screen, ST77XX_DISPON);
  delayMS(10);
}

void fillSolid(const struct ST7789 *screen, uint16_t color) {
  writeCommand(screen, ST77XX_RAMWR);

  screen->CSPort->BSRR = (1 << screen->CSPin << 16);
  screen->DCPort->BSRR = (1 << screen->DCPin); // Set for data
  for (size_t i = 0; i < 320 * 240; i++) {
    SPI1Transfer((uint8_t)(color >> 8)); // Send high byte
    SPI1Transfer((uint8_t)color);        // Send low byte
  }
  screen->CSPort->BSRR = (1 << screen->CSPin);
}
