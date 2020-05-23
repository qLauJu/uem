# DHT11

DHT11 sensor implementation for Arduino.

## Usage

```c
#include "arduino.h"
#include "dht11.h"
#include "uart.h"

int main(void) {
  uart_init();

  DHT11 dht;
  dht11_init(&dht, 10);

  while (true) {
    dht11_read(&dht);
    printf("t: %u | h: %u \n", dht.res.temperature, dht.res.humidity);
    _delay_ms(1000);
  }

  return 0;
}
```
