#pragma once

#include <stdint.h>

/**
 * @brief Escreve uma onda quadrada no pino.
 *
 * @param pin Numeração do pino. Pinos suportados 3, 5, 6, 9, 10 e 11
 * @param freq Frequência a ser escrita, intervalo [30.51, 8000000]hz
 */
void sqr_wave(uint8_t pin, double freq);