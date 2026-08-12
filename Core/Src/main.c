#include "main.h"
#include "USART.h"
#include "stm32l432xx.h"
#include <stdint.h>

// TODO: Make a real ms delay with timer
void delay(uint32_t delay) {
  while (delay--) {
    __NOP();
  }
}

void initPins(void) {
  /*
   * Enable GPIO clocks
   */
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // Enable GPIOA
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // Enable GPIOB

  /*
   * ============================================================
   * USART2
   * ============================================================
   */

  /*
   * PA2 - USART2_TX - AF7
   */
  GPIOA->MODER &= ~GPIO_MODER_MODER2;  // Clear PA2 mode
  GPIOA->MODER |= GPIO_MODER_MODER2_1; // Set PA2 as alternate function

  GPIOA->OTYPER &= ~GPIO_OTYPER_OT2;      // Set PA2 push-pull
  GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2; // Set PA2 very high speed
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;      // Set PA2 no pull-up/down

  GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2; // Clear PA2 alternate function
  GPIOA->AFR[0] |= (0b0111 << GPIO_AFRL_AFSEL2_Pos); // Set PA2 AF7 USART2_TX

  /*
   * PA15 - USART2_RX - AF3
   */
  GPIOA->MODER &= ~GPIO_MODER_MODER15;  // Clear PA15 mode
  GPIOA->MODER |= GPIO_MODER_MODER15_1; // Set PA15 as alternate function

  GPIOA->OTYPER &= ~GPIO_OTYPER_OT15;      // Set PA15 push-pull
  GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED15; // Set PA15 very high speed
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD15;      // Set PA15 no pull-up/down

  GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL15; // Clear PA15 alternate function
  GPIOA->AFR[1] |= (0b0011 << GPIO_AFRH_AFSEL15_Pos); // Set PA15 AF3 USART2_RX

  /*
   * ============================================================
   * SPI1
   * ============================================================
   */

  /*
   * PB3 - SPI1_SCK - AF5
   */
  GPIOB->MODER &= ~GPIO_MODER_MODER3;  // Clear PB3 mode
  GPIOB->MODER |= GPIO_MODER_MODER3_1; // Set PB3 as alternate function

  GPIOB->OTYPER &= ~GPIO_OTYPER_OT3;      // Set PB3 push-pull
  GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED3; // Set PB3 very high speed
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD3;      // Set PB3 no pull-up/down

  GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL3; // Clear PB3 alternate function
  GPIOB->AFR[0] |= (0b0101 << GPIO_AFRL_AFSEL3_Pos); // Set PB3 AF5 SPI1_SCK

  /*
   * PB4 - SPI1_MISO - AF5
   *
   * Needed for reading data back from the BME280.
   */
  GPIOB->MODER &= ~GPIO_MODER_MODER4;  // Clear PB4 mode
  GPIOB->MODER |= GPIO_MODER_MODER4_1; // Set PB4 as alternate function

  GPIOB->OTYPER &= ~GPIO_OTYPER_OT4;      // Set PB4 push-pull
  GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED4; // Set PB4 very high speed
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD4;      // Set PB4 no pull-up/down

  GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL4; // Clear PB4 alternate function
  GPIOB->AFR[0] |= (0b0101 << GPIO_AFRL_AFSEL4_Pos); // Set PB4 AF5 SPI1_MISO

  /*
   * PB5 - SPI1_MOSI - AF5
   */
  GPIOB->MODER &= ~GPIO_MODER_MODER5;  // Clear PB5 mode
  GPIOB->MODER |= GPIO_MODER_MODER5_1; // Set PB5 as alternate function

  GPIOB->OTYPER &= ~GPIO_OTYPER_OT5;      // Set PB5 push-pull
  GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED5; // Set PB5 very high speed
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD5;      // Set PB5 no pull-up/down

  GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL5; // Clear PB5 alternate function
  GPIOB->AFR[0] |= (0b0101 << GPIO_AFRL_AFSEL5_Pos); // Set PB5 AF5 SPI1_MOSI

  /*
   * ============================================================
   * ST7789 Display Control Pins
   * ============================================================
   */

  /*
   * PA11 - ST7789 CS
   */
  GPIOA->MODER &= ~GPIO_MODER_MODER11;  // Clear PA11 mode
  GPIOA->MODER |= GPIO_MODER_MODER11_0; // Set PA11 as output

  GPIOA->OTYPER &= ~GPIO_OTYPER_OT11;       // Set PA11 push-pull
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED11; // Set PA11 low speed
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD11;       // Set PA11 no pull-up/down

  /*
   * PA8 - ST7789 DC
   */
  GPIOA->MODER &= ~GPIO_MODER_MODER8;  // Clear PA8 mode
  GPIOA->MODER |= GPIO_MODER_MODER8_0; // Set PA8 as output

  GPIOA->OTYPER &= ~GPIO_OTYPER_OT8;       // Set PA8 push-pull
  GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED8; // Set PA8 low speed
  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD8;       // Set PA8 no pull-up/down

  /*
   * PB1 - ST7789 RESET
   */
  GPIOB->MODER &= ~GPIO_MODER_MODER1;  // Clear PB1 mode
  GPIOB->MODER |= GPIO_MODER_MODER1_0; // Set PB1 as output

  GPIOB->OTYPER &= ~GPIO_OTYPER_OT1;       // Set PB1 push-pull
  GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED1; // Set PB1 low speed
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD1;       // Set PB1 no pull-up/down

  /*
   * ============================================================
   * BME280 Control Pins
   * ============================================================
   */

  /*
   * PB0 - BME280 CS
   */
  GPIOB->MODER &= ~GPIO_MODER_MODER0;  // Clear PB0 mode
  GPIOB->MODER |= GPIO_MODER_MODER0_0; // Set PB0 as output

  GPIOB->OTYPER &= ~GPIO_OTYPER_OT0;       // Set PB0 push-pull
  GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED0; // Set PB0 low speed
  GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD0;       // Set PB0 no pull-up/down

  /*
   * ============================================================
   * Initial Output States
   * ============================================================
   *
   * Both SPI devices start deselected.
   */

  GPIOA->BSRR = GPIO_BSRR_BS11; // ST7789 CS high
  GPIOB->BSRR = GPIO_BSRR_BS0;  // BME280 CS high

  GPIOA->BSRR = GPIO_BSRR_BS8; // ST7789 DC high - data mode
  GPIOB->BSRR = GPIO_BSRR_BS1; // ST7789 RESET high
}

int main(void) {
  initPins();
  USART2Init(115200);
  GPIOB->BSRR |= LED_PIN; // Set to high
  while (1) {
    USART2SendString((uint8_t *)"Hello\r\n");
    delay(10000);
  }
  return 0;
}
