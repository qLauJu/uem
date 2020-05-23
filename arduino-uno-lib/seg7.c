#include "seg7.h"
#include "pin.h"

void Seg7_init(const Seg7 *display) {
  PinMode(display->a, OUTPUT);
  PinMode(display->b, OUTPUT);
  PinMode(display->c, OUTPUT);
  PinMode(display->d, OUTPUT);
  PinMode(display->e, OUTPUT);
  PinMode(display->f, OUTPUT);
  PinMode(display->g, OUTPUT);
  PinMode(display->dp, OUTPUT);

  DigitalWrite(display->a, LOW);
  DigitalWrite(display->b, LOW);
  DigitalWrite(display->c, LOW);
  DigitalWrite(display->d, LOW);
  DigitalWrite(display->e, LOW);
  DigitalWrite(display->f, LOW);
  DigitalWrite(display->g, LOW);
  DigitalWrite(display->dp, LOW);
}

void Seg7_putdigit(const Seg7 *display, uint8_t digit) {
  switch (digit) {
  case 0x0: {
    DigitalWrite(display->a, LOW);
    DigitalWrite(display->b, LOW);
    DigitalWrite(display->c, LOW);
    DigitalWrite(display->d, LOW);
    DigitalWrite(display->e, LOW);
    DigitalWrite(display->f, LOW);
    DigitalWrite(display->g, HIGH);
  } break;
  case 0x1: {
    DigitalWrite(display->b, LOW);
    DigitalWrite(display->c, LOW);
    DigitalWrite(display->a, HIGH);
    DigitalWrite(display->d, HIGH);
    DigitalWrite(display->e, HIGH);
    DigitalWrite(display->f, HIGH);
    DigitalWrite(display->g, HIGH);
  } break;
  case 0x2: {
    DigitalWrite(display->a, LOW);
    DigitalWrite(display->b, LOW);
    DigitalWrite(display->d, LOW);
    DigitalWrite(display->e, LOW);
    DigitalWrite(display->g, LOW);
    DigitalWrite(display->c, HIGH);
    DigitalWrite(display->f, HIGH);
  } break;
  case 0x3: {
    DigitalWrite(display->e, HIGH);
    DigitalWrite(display->f, HIGH);
    DigitalWrite(display->a, LOW);
    DigitalWrite(display->b, LOW);
    DigitalWrite(display->c, LOW);
    DigitalWrite(display->d, LOW);
    DigitalWrite(display->g, LOW);
  } break;
  case 0x4: {
    DigitalWrite(display->a, HIGH);
    DigitalWrite(display->d, HIGH);
    DigitalWrite(display->e, HIGH);
    DigitalWrite(display->b, LOW);
    DigitalWrite(display->c, LOW);
    DigitalWrite(display->f, LOW);
    DigitalWrite(display->g, LOW);
  } break;
  case 0x5: {
    DigitalWrite(display->b, HIGH);
    DigitalWrite(display->e, HIGH);
    DigitalWrite(display->a, LOW);
    DigitalWrite(display->c, LOW);
    DigitalWrite(display->d, LOW);
    DigitalWrite(display->f, LOW);
    DigitalWrite(display->g, LOW);
  } break;
  case 0x6: {
    DigitalWrite(display->b, HIGH);
    DigitalWrite(display->a, LOW);
    DigitalWrite(display->c, LOW);
    DigitalWrite(display->d, LOW);
    DigitalWrite(display->e, LOW);
    DigitalWrite(display->f, LOW);
    DigitalWrite(display->g, LOW);
  } break;
  case 0x7: {
    DigitalWrite(display->a, LOW);
    DigitalWrite(display->b, LOW);
    DigitalWrite(display->c, LOW);
    DigitalWrite(display->d, HIGH);
    DigitalWrite(display->e, HIGH);
    DigitalWrite(display->f, HIGH);
    DigitalWrite(display->g, HIGH);
  } break;
  case 0x8: {
    DigitalWrite(display->a, LOW);
    DigitalWrite(display->b, LOW);
    DigitalWrite(display->c, LOW);
    DigitalWrite(display->d, LOW);
    DigitalWrite(display->e, LOW);
    DigitalWrite(display->f, LOW);
    DigitalWrite(display->g, LOW);
  } break;
  case 0x9: {
    DigitalWrite(display->a, LOW);
    DigitalWrite(display->b, LOW);
    DigitalWrite(display->c, LOW);
    DigitalWrite(display->d, LOW);
    DigitalWrite(display->e, HIGH);
    DigitalWrite(display->f, LOW);
    DigitalWrite(display->g, LOW);
  } break;
  }
}
