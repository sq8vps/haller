#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include <stdint.h>

#define MOTOR_COUNT 5
#define MOTOR_SINGLE_DATA_SIZE 2

#define MOTOR_PAYLOAD_SIZE (MOTOR_COUNT * MOTOR_SINGLE_DATA_SIZE)


/**
 * @brief Send data to STEvals (motor controllers)
 * @param *torque Torque values for each motor
 * @attention *torque array must have a length of MOTOR_COUNT (must be equal to the number of motors)
 */
void Motor_sendData(uint16_t *torque);

#endif /* INC_MOTOR_H_ */
