#include "servo.h"
#include "proto.h"
#include "pwm.h"

#define SERVO_PACKET_ID 0x03
#define SERVO_PACKET_LENGTH 5

#define SERVO_FIRST_PWM_CHANNEL 6
#define SERVO_COUNT 3 //TODO: unknown for now

#define SERVO_COEFF 0.1f //TODO: unknown for now

struct ServoPacket
{
	uint8_t number;
	float value;
} __attribute__ ((packed));

static void ServoParse(void *buffer, uint8_t len)
{
	if(len < SERVO_PACKET_LENGTH)
		return;

	struct ServoPacket *p = buffer;
	if(p->number >  SERVO_COUNT)
		return;

	PwmSet(p->number + SERVO_FIRST_PWM_CHANNEL, p->value * SERVO_COEFF);
}

void ServoInit(void)
{
	 ProtoRegister(SERVO_PACKET_ID, &ServoParse);
}
