#pragma once

#include <stdint.h>

/// Display de 7 segmentos, contem todo os 7 pinos mais o ponto.
typedef struct {
  uint8_t a, b, c, d, e, f, g, dp;
} Seg7;

/**
 * @brief Inicializa o display com os pinos.
 *
 * @param display Componente do display a ser configurado.
 */
void Seg7_init(const Seg7 *display);

/**
 * @brief Escreve um dígito no display
 *
 * @param display Componente de display a ser escrito
 * @param digit Digito que deve ser escrito
 */
void Seg7_putdigit(const Seg7 *display, uint8_t digit);
