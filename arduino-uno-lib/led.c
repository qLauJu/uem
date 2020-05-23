#include "led.h"
#include "delay.h"

void Led_init(uint8_t led) { PinMode(led, OUTPUT); }

void Led_set(uint8_t led, uint8_t state) { DigitalWrite(led, state); }

void Led_swap(uint8_t led) { DigitalWrite(led, !DigitalRead(led)); }

void Led_blink(uint8_t led, uint32_t duration) {
  Led_swap(led);
  delay_ms(duration);
}
