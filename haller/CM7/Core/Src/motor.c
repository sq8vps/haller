#include "motor.h"

#include "stm32h7xx.h"
#include <string.h>



extern I2C_HandleTypeDef hi2c1;


void Motor_sendData(uint16_t *torque)
{
	uint8_t data[MOTOR_PAYLOAD_SIZE];
	memcpy(data, torque, 2 * MOTOR_COUNT);

	HAL_I2C_Master_Transmit(&hi2c1, 0x00, data, MOTOR_PAYLOAD_SIZE, 10);
}
