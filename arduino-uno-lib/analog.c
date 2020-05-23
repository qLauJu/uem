#include "analog.h"
#include "bit.h"
#include "pin.h"
#include "sqr_wave.h"
#include <avr/io.h>

#ifndef NULL
#define NULL (void *)0x0
#endif

static const struct {
  volatile uint8_t *_TCCA;
  volatile uint8_t *_TCCB;
  uint8_t _COM1;
  uint8_t _COM0;
  volatile uint16_t *_OCR;
} map[] = {
    {NULL, NULL, 0, 0, NULL},                               // 0
    {NULL, NULL, 0, 0, NULL},                               // 1
    {NULL, NULL, 0, 0, NULL},                               // 2
    {&TCCR2A, &TCCR2B, COM2B1, COM2B0, (uint16_t *)&OCR2B}, // 3
    {NULL, NULL, 0, 0, NULL},                               // 4
    {&TCCR0A, &TCCR0B, COM0B1, COM0B0, (uint16_t *)&OCR0B}, // 5
    {&TCCR0A, &TCCR0B, COM0A1, COM0A0, (uint16_t *)&OCR0A}, // 6
    {NULL, NULL, 0, 0, NULL},                               // 7
    {NULL, NULL, 0, 0, NULL},                               // 8
    {&TCCR1A, &TCCR1B, COM1A1, COM1A0, &OCR1A},             // 9
    {&TCCR1A, &TCCR1B, COM1B1, COM1B0, &OCR1B},             // 10
    {&TCCR2A, &TCCR1B, COM2A1, COM2A0, (uint16_t *)&OCR2A}, // 11
    {NULL, NULL, 0, 0, NULL},                               // 12
    {NULL, NULL, 0, 0, NULL},                               // 13
    {NULL, NULL, 0, 0, NULL},                               // 14
    {NULL, NULL, 0, 0, NULL},                               // 15
    {NULL, NULL, 0, 0, NULL},                               // 16
    {NULL, NULL, 0, 0, NULL},                               // 17
    {NULL, NULL, 0, 0, NULL},                               // 18
    {NULL, NULL, 0, 0, NULL},                               // 19
};

void AnalogWrite(uint8_t pin, uint16_t dc) {
  PinMode(pin, OUTPUT);

  if (map[pin]._TCCA == NULL) {
    if (dc < 128) {
      DigitalWrite(pin, LOW);
    } else {
      DigitalWrite(pin, HIGH);
    }
    return;
  }

  sbi(*map[pin]._TCCA, WGM00);
  sbi(*map[pin]._TCCA, WGM01); // Define fast PWM
  sbi(*map[pin]._TCCB, CS01);  // define prescaler 8
  sbi(*map[pin]._TCCA, map[pin]._COM1);
  *map[pin]._OCR = dc;
}

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

typedef struct {
  uint16_t a;
  uint8_t b;
} tuple2;

tuple2 get_prescaler(double freq) {
  tuple2 r;

  if (freq >= 31250.0 && freq <= 8000000.0)
    r.a = 1, r.b = 1;

  if (freq >= 3906.25 && freq <= 1000000.0)
    r.a = 8, r.b = 2;

  if (freq >= 488.28125 && freq <= 125000.0)
    r.a = 64, r.b = 3;

  if (freq >= 122.0703125 && freq <= 31250.0)
    r.a = 256, r.b = 4;

  if (freq >= 30.517578125 && freq <= 7812.5)
    r.a = 1024, r.b = 5;

  return r;
}

tuple2 get_prescaler2(double freq) {
  tuple2 r;

  if (freq >= 31250.0 && freq <= 8000000.0)
    r.a = 1, r.b = 1;

  if (freq >= 3906.25 && freq <= 1000000.0)
    r.a = 8, r.b = 2;

  if (freq >= 976.5625 && freq <= 250000.0)
    r.a = 32, r.b = 3;

  if (freq >= 488.28125 && freq <= 125000.0)
    r.a = 64, r.b = 4;

  if (freq >= 244.140625 && freq <= 62500.0)
    r.a = 128, r.b = 5;

  if (freq >= 122.0703125 && freq <= 31250.0)
    r.a = 256, r.b = 6;

  if (freq >= 30.517578125 && freq <= 7812.5)
    r.a = 1024, r.b = 7;

  return r;
}

inline static uint16_t get_ocr(uint16_t ps) { return F_CPU / 2 * ps - 1; }

void sqr_wave(uint8_t pin, double freq) {
  tuple2 ps;
  uint16_t ocr;

  PinMode(pin, OUTPUT);

  switch (pin) {
  case 3:
  case 11:
    ps = get_prescaler2(freq);
    ocr = get_ocr(ps.a);
    TCCR2B = ps.b; // set prescaler
    cbi(TCCR2A, WGM20);
    sbi(TCCR2A, WGM21); // set CTC
    cbi(TCCR2B, WGM22);
    sbi(TCCR2A, COM2A0); // mudança de estado na igualdade de comparaçao
    *(uint8_t *)&OCR2A = (uint8_t)ocr;
    break;

  case 5:
  case 6:
    ps = get_prescaler(freq);
    ocr = get_ocr(ps.a);
    TCCR0B = ps.b;
    cbi(TCCR0A, WGM00);
    sbi(TCCR0A, WGM01); // set CTC
    cbi(TCCR0B, WGM02);

    sbi(TCCR0A, COM0A0); // mudança de estado na igualdade de comparaçao
    *(uint8_t *)&OCR0A = (uint8_t)ocr;

    break;

  case 9:
  case 10:
    ps = get_prescaler(freq);
    ocr = get_ocr(ps.a);
    TCCR1B = ps.b;
    sbi(TCCR1B, WGM12);

    sbi(TCCR1A, COM1A0); // mudança de estado na igualdade de comparaçao
    *(uint8_t *)&OCR1A = (uint8_t)ocr;

    break;

  default:
    DigitalWrite(pin, HIGH);
  }
}
