#pragma once

#include "bit.h"
#include <avr/io.h>
#include <stdint.h>

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#define LOW 0x0
#define HIGH 0x1

static const struct {
  volatile uint8_t *_DDR;
  volatile uint8_t *_PORT;
  volatile uint8_t *_PIN;
} arduino_pin_map[] = {
    {&DDRD, &PORTD, &PIND}, // 0
    {&DDRD, &PORTD, &PIND}, // 1
    {&DDRD, &PORTD, &PIND}, // 2
    {&DDRD, &PORTD, &PIND}, // 3
    {&DDRD, &PORTD, &PIND}, // 4
    {&DDRD, &PORTD, &PIND}, // 5
    {&DDRD, &PORTD, &PIND}, // 6
    {&DDRD, &PORTD, &PIND}, // 7
    {&DDRB, &PORTB, &PINB}, // 8
    {&DDRB, &PORTB, &PINB}, // 9
    {&DDRB, &PORTB, &PINB}, // 10
    {&DDRB, &PORTB, &PINB}, // 11
    {&DDRB, &PORTB, &PINB}, // 12
    {&DDRB, &PORTB, &PINB}, // 13
    {&DDRB, &PORTB, &PINB}, // 14
    {&DDRB, &PORTB, &PINB}, // 15
};

#define pin_mode(pin, mode)                                                    \
  {                                                                            \
    switch (mode) {                                                            \
    case INPUT:                                                                \
      cbi(*arduino_pin_map[pin]._DDR, pin % 8);                                \
      break;                                                                   \
    case OUTPUT:                                                               \
      sbi(*arduino_pin_map[pin]._DDR, pin % 8);                                \
      break;                                                                   \
    case INPUT_PULLUP:                                                         \
      cbi(*arduino_pin_map[pin]._DDR, pin % 8);                                \
      sbi(*arduino_pin_map[pin]._PORT, pin % 8);                               \
      break;                                                                   \
    }                                                                          \
  }

#define digital_write(pin, value)                                              \
  {                                                                            \
    switch (value) {                                                           \
    case HIGH:                                                                 \
      sbi(*arduino_pin_map[pin]._PORT, pin % 8);                               \
      break;                                                                   \
    case LOW:                                                                  \
      cbi(*arduino_pin_map[pin]._PORT, pin % 8);                               \
      break;                                                                   \
    }                                                                          \
  }

#define digital_read(pin) is_bit_set(*arduino_pin_map[pin]._PIN, pin % 8)

uint32_t pulse_in(uint8_t pin, uint8_t value);