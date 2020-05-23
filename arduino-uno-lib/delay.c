#include "delay.h"

extern void delay_1us(void);
extern void delay_1ms(void);

void delay_ms(uint32_t ms) {
  for (uint32_t i = 0; i < ms; ++i)
    delay_1ms();
}

void delay_us(uint32_t us) {
  for (uint32_t i = 0; i < us; ++i)
    delay_1us();
}