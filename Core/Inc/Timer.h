#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
void TIMER2Init(void);
void delayUS(uint32_t delay);
void delayMS(uint32_t delay);

#endif // !TIMER_H
