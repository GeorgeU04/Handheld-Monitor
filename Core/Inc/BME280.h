#ifndef BME280_H
#define BME280_H

#include "stm32l432xx.h"
#include <stdint.h>

struct weatherData {
  int32_t temperature;
  uint32_t pressure;
  uint32_t humidity;
};

struct BME280 {
  uint16_t CSPin;
  GPIO_TypeDef *CSPort;
  struct weatherData data;
};

void readWeatherData(struct BME280 *sensor);
void initBME280(struct BME280 *sensor, GPIO_TypeDef *CSPort, uint16_t CSPin);
void readWeatherData(struct BME280 *sensor);
void writeRegister(const struct BME280 *sensor, uint8_t *data,
                   uint8_t *address);
#endif // !BME280_H
