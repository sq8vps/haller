
#ifndef INC_PWM_H_
#define INC_PWM_H_

#include <stdint.h>

void PwmInit(void);

void PwmSet(uint8_t channel, float value);


#endif /* INC_PWM_H_ */
