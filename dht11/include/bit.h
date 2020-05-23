#pragma once

/**
 * @brief Set nth bit on register REG.
 *
 */
#define sbi(REG, N) ((REG) |= (1 << (N)))

/**
 * @brief Unset nth bit on register REG.
 *
 */
#define cbi(REG, N) ((REG) &= ~(1 << (N)))

/**
 * @brief Verify if nth bit on register REG is set.
 *
 */
#define is_bit_set(REG, N) ((1 << (N) & (REG)) ? 1 : 0)
