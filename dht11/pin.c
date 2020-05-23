#include "pin.h"
#include <stdio.h>
#include <util/delay.h>

uint32_t pulse_in(uint8_t pin, uint8_t value) {
  uint32_t duration = 0;

  while (digital_read(pin) == value) {
    duration++;
    _delay_us(1);
  }

  return duration;
}