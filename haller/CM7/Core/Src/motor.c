#include "motor.h"
#include "proto.h"

#define MOTOR_PACKET_ID 0x01
#define MOTOR_STOP_PACKET_ID 0x02
#define MOTOR_PACKET_LENGTH (6 * 4)

static void MotorParse(void *buffer, uint8_t len)
{
	if(len < MOTOR_PACKET_LENGTH)
		return;

	float *val = buffer;

	//val[0] - motor 0, val[1] - motor 1 ...
}

static void MotorParseStop(void *buffer, uint8_t len)
{
	//stop all motors
}

void MotorInit(void)
{
	ProtoRegister(MOTOR_PACKET_ID, &MotorParse);
	ProtoRegister(MOTOR_STOP_PACKET_ID, &MotorParseStop);
}
