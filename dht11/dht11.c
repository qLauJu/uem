#include "dht11.h"

void dht11_init(DHT11 *dht11, uint8_t pin) {
  dht11->pin = pin;

  pin_mode(dht11->pin, OUTPUT);
  digital_write(dht11->pin, HIGH);
  _delay_ms(200);
}

void dht11_read(DHT11 *dht11) {
  pin_mode(dht11->pin, OUTPUT);
  digital_write(dht11->pin, HIGH);
  _delay_ms(20);

  digital_write(dht11->pin, LOW);
  _delay_ms(20);

  digital_write(dht11->pin, HIGH);
  _delay_us(40);

  pin_mode(dht11->pin, INPUT_PULLUP);
  _delay_us(10);

  _delay_us(80);

  _delay_us(80);

  uint8_t data[5];
  uint8_t bits[40];

  for (uint8_t i = 0; i < 40; ++i) {
    while (digital_read(dht11->pin) == LOW)
      ;

    bits[i] = pulse_in(dht11->pin, HIGH) > 30;
  }

  for (uint8_t i = 0; i < 5; ++i) {
    uint8_t byte = 0;
    for (uint8_t j = 0; j < 8; ++j) {
      byte |= bits[i * 8 + j] << (7 - j);
    }
    data[i] = byte;
  }

  dht11->res.humidity = data[0];
  dht11->res.temperature = data[2];
}