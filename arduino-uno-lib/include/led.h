#pragma once

#include "pin.h"
#include <stdint.h>

/// Constante que define o estado desligado do LED
#define OFF 0x0

/// Constante que define o estado ligado do LED
#define ON 0x1

/**
 * @brief Inicializa o pino como um LED
 *
 * @param led Pino a ser utilizado como LED
 */
void Led_init(uint8_t led);

/**
 * @brief Atribui o estado ON ou OFF ao LED.
 *
 * @param led Pino a ser atribuído o estado.
 * @param state Estado ON ou OFF para atribuir ao pino.
 */
void Led_set(uint8_t led, uint8_t state);

/**
 * @brief Troca o estado do LED.
 *
 * @param led Pino a ter o estado trocado.
 */
void Led_swap(uint8_t led);

/**
 * @brief Pisca o LED com uma duração.
 *
 * @param led Pino do LED
 * @param duration Duração em milisegundos.
 */
void Led_blink(uint8_t led, uint32_t duration);