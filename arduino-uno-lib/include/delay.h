#pragma once

#include <stdint.h>

/**
 * @brief Realiza um atraso com precisão de milisegundos.
 *
 * @param ms Quantidade de milisegundos para realizar o atraso.
 */
void delay_ms(uint32_t ms);

/**
 * @brief Realiza um atraso com precisão de microsegundos.
 *
 * @param us Quantidade de microsegundos para realizar o atraso.
 */
void delay_us(uint32_t us);