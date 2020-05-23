#pragma once

#include "delay.h"
#include "pin.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Inicializa o pino como um botão.
 *
 * @param pin Numeração do pino a ser inicializado.
 */
void Button_init(uint8_t pin);

/**
 * @brief Verifica se o botão está pressionado.
 *
 * @param pin Pino a realizar a verificação.
 * @return true Quando o pino está pressionado
 * @return false Quando o pino não está pressionado
 */
bool Button_ispressed(uint8_t pin);

/**
 * @brief Observa um clique no botão e dispara o callback quando o clique
 * acontecer.
 *
 * @param pin Pino do botão a ser observado.
 * @param callback Ponteiro para uma função que é executado quando o clique
 * acontecer
 * @param args Argumentos para o callback
 * @return void* retorna o valor retornado pelo callback.
 */
void *Button_onclick(uint8_t pin, void *(*callback)(void *), void *args);

/**
 * @brief Observa um botão ser solto e dispara o callback.
 *
 * @param pin Pino do botão a ser observado.
 * @param callback Ponteiro para uma função que é executado quando o botão for
 * solto acontecer
 * @param args Argumentos para o callback
 * @return void* retorna o valor retornado pelo callback.
 */
void *Button_onrelease(uint8_t pin, void *(*callback)(void *), void *args);

/**
 * @brief Macro que realiza uma ação quando o botão é pressionado.
 *
 */
#define BUTTON_ONCLICK(btn, action)                                            \
  if (Button_ispressed(btn)) {                                                 \
    action;                                                                    \
    while (Button_ispressed(btn))                                              \
      ;                                                                        \
    delay_ms(10);                                                              \
  }

/**
 * @brief Macro que realiza uma ação quando o botão é solto.
 *
 */
#define BUTTON_ONRELEASE(btn, action)                                          \
  if (Button_ispressed(btn)) {                                                 \
    while (Button_ispressed(btn))                                              \
      ;                                                                        \
    action;                                                                    \
    delay_ms(10);                                                              \
  }
