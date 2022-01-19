#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#include <stdint.h>

void Servo_init(void);
void Servo_set(uint8_t no, uint8_t val);

#endif /* INC_SERVO_H_ */
