#include "button.h"
#include <stddef.h>

void Button_init(uint8_t pin) { PinMode(pin, PULL_UP); }
bool Button_ispressed(uint8_t pin) { return DigitalRead(pin) == LOW; }

void *Button_onclick(uint8_t pin, void *(*callback)(void *), void *args) {
  void *r = NULL;

  if (Button_ispressed(pin)) {
    r = (*callback)(args);
    while (Button_ispressed(pin))
      ;
    delay_ms(10);
  }

  return r;
}

void *Button_onrelease(uint8_t pin, void *(*callback)(void *), void *args) {
  void *r = NULL;
  if (Button_ispressed(pin)) {
    while (Button_ispressed(pin))
      ;

    r = (*callback)(args);
    delay_ms(10);
  }

  return r;
}
