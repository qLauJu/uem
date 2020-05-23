#pragma once

#include <stdint.h>

/**
 * @brief Escreve uma onda PWM no pino com o duty cycle definido.
 *
 * @param pin Pino a escrever a onda, apenas os pinos 3, 5, 6, 9, 10 e 11 são
 * suportados.
 * @param dc Valor de duty cycle, o intervalo aceito é [0, 255]
 */
void AnalogWrite(uint8_t pin, uint16_t dc);
