
#ifndef INC_PWM_H_
#define INC_PWM_H_

#include <stdint.h>

void PwmInit(void);

void PwmSet(uint8_t channel, float value, uint32_t neutral, uint32_t delta);


#endif /* INC_PWM_H_ */
