#pragma once

#include "arduino.h"

typedef struct {
  uint8_t pin;
  struct {
    uint8_t temperature, humidity;
  } res;
} DHT11;

void dht11_init(DHT11 *dht11, uint8_t pin);
void dht11_read(DHT11 *dht11);