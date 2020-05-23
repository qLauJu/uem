#include <avr/io.h>
#include <bit.h>
#include <pin.h>
#include <stdio.h>

static const struct {
  volatile uint8_t *_DDR;
  volatile uint8_t *_PORT;
  volatile uint8_t *_PIN;
} map[] = {
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

/*
 * Converts Arduino's Pin to ATMEGA Pin
 */
static inline uint8_t AtmelPinFor(uint8_t pin) { return pin % 8; }

/*
 * Sets Arduino's Pin as INPUT or OUTPUT
 */
inline void PinMode(uint8_t pin, uint8_t mode) {
  uint8_t atmel_pin = AtmelPinFor(pin);
  switch (mode) {
  case INPUT:
    cbi(*map[pin]._DDR, atmel_pin);
    break;

  case OUTPUT:
    sbi(*map[pin]._DDR, atmel_pin);
    break;

  case PULL_UP:
    cbi(*map[pin]._DDR, atmel_pin);
    sbi(*map[pin]._PORT, atmel_pin);
    break;
  }
}

/*
 * Writes HIGH or LOW to Arduino's Pin
 */
inline void DigitalWrite(uint8_t pin, uint8_t value) {
  uint8_t atmel_pin = AtmelPinFor(pin);

  switch (value) {
  case HIGH:
    sbi(*map[pin]._PORT, atmel_pin);
    break;

  case LOW:
    cbi(*map[pin]._PORT, atmel_pin);
    break;
  }
}

/*
 * Reads from Arduino's Pin.
 */
inline uint8_t DigitalRead(uint8_t pin) {
  uint8_t atmel_pin = AtmelPinFor(pin);
  return is_bit_set(*map[pin]._PIN, atmel_pin);
}
