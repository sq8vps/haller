#ifndef INC_PRESSURE_H_
#define INC_PRESSURE_H_

#include <stdint.h>

/**
 * @brief Get and send pressure data
 */
void Pressure_getAndSend(void);

/**
 * @brief Set periodic pressure measurement interval
 * @param interval Interval in milliseconds
 */
void Pressure_setInterval(uint16_t interval);

/**
 * @brief Initialize pressure module
 */
void Pressure_init(void);

/**
 * @brief Refresh pressure timers
 */
void Pressure_refresh(void);

#endif /* INC_PRESSURE_H_ */
