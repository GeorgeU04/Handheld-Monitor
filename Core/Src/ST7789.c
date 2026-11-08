#include "ST7789.h"
#include "SPI.h"
#include <stdint.h>

void initScreen(struct ST7789 *screen, GPIO_TypeDef *CSPort, uint16_t CSPin,
                GPIO_TypeDef *DCPort, uint16_t DCPin, GPIO_TypeDef *RSTPort,
                uint16_t RSTPin) {
  screen->CSPort = CSPort;
  screen->CSPin = CSPin;
  screen->DCPort = DCPort;
  screen->DCPin = DCPin;
  screen->RSTPort = RSTPort;
  screen->RSTPin = RSTPin;
  screen->RSTPort->BSRR = (1 << screen->RSTPin);
}

void writeCommand(struct ST7789 *screen, uint8_t command) {
  screen->CSPort->BSRR = (1 << screen->CSPin << 16);
  screen->DCPort->BSRR = (1 << screen->DCPin << 16); // Reset for command
  SPI1Send(&command, 1);
  screen->CSPort->BSRR = (1 << screen->CSPin);
}

void writeData(struct ST7789 *screen, uint8_t data) {
  screen->CSPort->BSRR = (1 << screen->CSPin << 16);
  screen->DCPort->BSRR = (1 << screen->DCPin); // Set for data
  SPI1Send(&data, 1);
  screen->CSPort->BSRR = (1 << screen->CSPin);
}

void setup(const struct ST7789 *screen) {}
