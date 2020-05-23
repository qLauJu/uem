# Biblioteca Arduino UNO
Trabalho da disciplina de Sistemas Digitais do curso de Ciência da Computação da Universidade Estadual de Maringá.

## Documentação

### Pinos

Os pinos são mapeados de maneira que possam ser utilizados a partir da numeração do Arduino. A manipulação dos pinos podem ser realizadas por meio das funções abaixo localizadas no arquivo `pin.h`:

```c
void PinMode(uint8_t pin, uint8_t mode);
void DigitalWrite(uint8_t pin, uint8_t value);
uint8_t DigitalRead(uint8_t pin);
```

Onde:
- Constantes para o modo do pino `INPUT`, `OUTPUT` ou `PULL_UP`.
- Constantes para o valor lógico do pino `HIGH` ou `LOW`.

#### Exemplo

```c
#include "pin.h"

#define LED 11

int main(void) {

  PinMode(LED, OUTPUT);
  DigitalWrite(LED, HIGH);

  while (1) {}

  return 0;
}
```

### LED

Há um componente de LED localizado no arquivo `led.h`. A manipulação do LED pode ser feita através das seguintes funções:

```c
void Led_init(uint8_t led);
void Led_set(uint8_t led, uint8_t state);
void Led_swap(uint8_t led);
void Led_blink(uint8_t led, uint32_t delay);
```

- A função `Led_init` é utilizada para configurar o pino como um LED.
- A função `Led_set` define o estado do LED, `ON` ou `OFF`.
- A função `Led_swap` troca o estado do LED, se está `ON` troca para `OFF` e se está `OFF` troca para `ON`.
- A função `Led_blink` recebe um parâmetro `delay` e pisca o LED por `delay` ms.

#### Exemplo

```c
#include "led.h"

#define LED 11

int main(void) {
  Led_init(LED);

  while(1)  
    Led_blink(LED, 500);

  return 0;
}
```

### Atraso
Há duas funções de atraso, uma com presição de milisegundos e outra com precisão de microsegundos. Ambas rotinas são implementadas em assembly no arquivo `_delay.s` e executadas a partir das funções definidas em `delay.h`.

```c
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
```

### Sensor de distância
O componente de sensor de distância é definido no arquivo `dst_sensor.h`. Ele consiste da definição de 2 pinos. Tal componente é manipulado pelas funções:

```c
void DstSensor_init(DstSensor *dst);
uint32_t DstSensor_read(DstSensor *dst);
```

- A função `DstSensor_init` configura o componente.
- A função `DstSensor_read` realiza a leitura de distância e retorna o valor em cm.

A estrutura `DstSensor` é uma estrutura contendo os campos `trigger` e `echo`, que são pinos do componente.

#### Exemplo

```c
#include "dst_sensor.h"

#define TRIGGER 9
#define ECHO 10

int main(void) {

  DstSensor dst = { .trigger = TRIGGER, .echo = ECHO };
  DstSensor_init(&dst);

  while (1) {
    uint32_t distance = DstSensor_read(&dst);
    // Use a distância lida.
  }

  return 0;
}

```

### Display 7 segmentos
O componente display de 7 segmentos está definido no arquivo `seg7.h`. O componente é definido como uma estrutura contendo campos específicos para os pinos, A, B, C, D, E, F, G, H e DP do display.

Para manipulação do componente, utiliza-se as seguintes funções:

```c
void Seg7_init(const Seg7 *display);
void Seg7_putdigit(const Seg7 *display, uint8_t digit);
```

- A função `Seg7_init` é utilizada para inicializar o display.
- A função `Seg7_putdigit` é utilizada para escrever um dígito (0-9) no display.

#### Exemplo

```c
#include "seg7.h"
#include "delay.h"

int main(void) {
  uint8_t i = 0;
  Seg7 display = {
    .a = 3,
    .b = 4,
    .c = 5,
    .d = 6,
    .e = 7,
    .f = 8,
    .g = 9,
    .dp = 10,
  }

  Seg7_init(&display);

  while (1) {
    Seg7_putdigit(&display, i);
    i = (i + 1) % 10;
    delay(500);
  }

  return 0;
}
```

### Botão
O componente de botão é definido no arquivo `button.h`. Há dois modos de interagir com o componente, através da definição de uma fução que é chamada quando o botão é pressionado ou solto, ou atráves da passagem de ações pelos MACROS.

```c
void Button_init(uint8_t pin);
bool Button_ispressed(uint8_t pin);
void *Button_onclick(uint8_t pin, void *(*callback)(void *), void *args);
void *Button_onrelease(uint8_t pin, void *(*callback)(void *), void *args);
```

- A função `Button_init` realiza a inicialização do pino como um botão.
- A função `Button_ispressed` verifica se o botão está pressionado.
- A função `Button_onclick` executa o callback quando o botão é pressionado.
- A função `Button_onrelease` executa o callback quando o botão é solto.

Além das funções acima, há dois MACROS que simplificam a utilização do botão, vide exemplo abaixo.

#### Exemplo

```c
#include "button.h"
#include "delay.h"
#include "led.h"
#include <stdbool.h>
#include <stddef.h>

void *on_click(void *args) {
  uint8_t led = *(uint8_t *)args;
  Led_swap(led);
  return NULL;
}

int main(void) {
  const uint8_t led = 2;
  const uint8_t btn = 3;

  Button_init(btn);
  Led_init(led);

  while (true) {
    Button_onclick(btn, on_click, (void *)&led);
    // ou
    BUTTON_ONCLICK(btn, Led_swap(led));
  }

  return 0;
}
```

### Onda quadrada e PWM
No arquivo `analog.h` e `sqr_wave.h` são definidas funções para geração de saída PWM e escrita de onda quadrada respectivamente.

**Saída PWM**

A saída PWM é feita utilizando a função:
```c
void AnalogWrite(uint8_t pin, uint16_t dc);
```
Onde o parâmetro `dc` é o **Duty cycle** a ser aplicado ao pino, este valor varia de 0 a 255. Apenas alguns pinos suporta saída PWM, são eles, 3, 5, 6, 9, 10 e 11.

**Onda quadrada**

A onda quadrada é feita utilizando a função:
```c
void sqr_wave(uint8_t pin, double freq);
```
Onde o parâmetro `freq` indica a frequência que deve ser aplicada ao pino. São suportadas frequências entre o intervalo 30.51Hz até 8MHz.

Os pinos que suportam a escrita de onda quadrada são 3, 5, 6, 9, 10 e 11.