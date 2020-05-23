#pragma once

#include <stdint.h>

#define INPUT 0x0
#define OUTPUT 0x1
#define PULL_UP 0x2

#define LOW 0x0
#define HIGH 0x1

/**
 * @brief Define um pino como entrada, saída ou pull up.
 *
 * @param pin Número do pino a ser configurado.
 * @param mode Modo de configuração do pino, podendo ser INPUT, OUTPUT ou
 * PULL_UP
 */
void PinMode(uint8_t pin, uint8_t mode);

/**
 * @brief Escreve o valor lógico 1 ou 0 no pino especificado.
 *
 * @param pin Número do pino a ser escrito.
 * @param value Valor lógico a ser escrito no pino, podendo ser HIGH ou LOW.
 */
void DigitalWrite(uint8_t pin, uint8_t value);

/**
 * @brief Lê o valor lógico do pino.
 *
 * @param pin Número do pino a ser lido.
 * @return uint8_t Retorna o valor lógico lido, podendo ser HIGH ou LOW.
 */
uint8_t DigitalRead(uint8_t pin);