#include "dst_sensor.h"
#include "bit.h"
#include "delay.h"
#include "pin.h"
#include <avr/io.h>

void DstSensor_init(const DstSensor *dst) {
  PinMode(dst->trigger, OUTPUT);
  PinMode(dst->echo, INPUT);

  sbi(TCCR1B, CS01); // set prescaler to 8
}

uint32_t DstSensor_read(const DstSensor *dst) {
  uint32_t d = 0;

  DigitalWrite(dst->trigger, HIGH);
  delay_us(10);
  DigitalWrite(dst->trigger, LOW);

  while (DigitalRead(dst->echo) == LOW)
    ;
  TCNT1 = 0;

  while (DigitalRead(dst->echo) == HIGH)
    ;
  d = TCNT1;

  return d / 116;
}
