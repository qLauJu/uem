#pragma once

/**
 * @brief Ativa o bit N no registrador REG
 *
 */
#define sbi(REG, N) ((REG) |= (1 << (N)))

/**
 * @brief Desativa o bit N no registrador REG
 *
 */
#define cbi(REG, N) ((REG) &= ~(1 << (N)))

/**
 * @brief Verifica se o bit N está ativo no registrador REG
 *
 */
#define is_bit_set(REG, N) (((REG) >> (N)) & 1)
