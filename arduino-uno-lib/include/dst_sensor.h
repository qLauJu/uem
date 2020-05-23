#pragma once

#include <stdint.h>

/**
 * @brief Estrutura que armazena os pinos necessários para manipulação do sensor
 * de distância.
 */
typedef struct {
  uint8_t trigger, echo;
} DstSensor;

/**
 * @brief Configura o sensor de distância.
 *
 * Ao chamar esta função, os pinos trigger e echo devem estar definidos.
 *
 * @param dst Sensor de distância a ser configurado.
 */
void DstSensor_init(const DstSensor *dst);

/**
 * @brief Realiza a leitura da distância.
 *
 * @param dst Sensor a ter a distância lida.
 * @return uint32_t Retorna a distância lida em cm.
 */
uint32_t DstSensor_read(const DstSensor *dst);
