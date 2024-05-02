#include "motor.h"
#include "proto.h"
#include "pwm.h"

#define MOTOR_PACKET_ID 0x01
#define MOTOR_STOP_PACKET_ID 0x02
#define MOTOR_PACKET_LENGTH (6 * 4)

#define MOTOR_FIRST_PWM_CHANNEL 0

static void MotorParse(void *buffer, uint8_t len)
{
	if(len < MOTOR_PACKET_LENGTH)
		return;

	float *val = buffer;

	//val[0] - motor 0, val[1] - motor 1 ...
	for(uint8_t i = 0; i < 6; i++)
		PwmSet(i + MOTOR_FIRST_PWM_CHANNEL, val[i]);
}

static void MotorParseStop(void *buffer, uint8_t len)
{
	//stop all motors
	for(uint8_t i = 0; i < 6; i++)
		PwmSet(i + MOTOR_FIRST_PWM_CHANNEL, 0.0f);
}

void MotorInit(void)
{
	ProtoRegister(MOTOR_PACKET_ID, &MotorParse);
	ProtoRegister(MOTOR_STOP_PACKET_ID, &MotorParseStop);
}
