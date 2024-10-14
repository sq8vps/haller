#include "servo.h"
#include "proto.h"
#include "pwm.h"

#define SERVO_PACKET_ID 0x03
#define SERVO_PACKET_LENGTH 5

#define SERVO_FIRST_PWM_CHANNEL 6
#define SERVO_COUNT 3 //TODO: unknown for now

#define SERVO_COEFF (1.f / (3.14f / 2.f)) //-1...1 for -3.14/2...3.14/2

#define SERVO_NEUTRAL 1500
#define SERVO_DELTA 1000

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

	PwmSet(p->number + SERVO_FIRST_PWM_CHANNEL, p->value * SERVO_COEFF, SERVO_NEUTRAL, SERVO_DELTA);
}

void ServoInit(void)
{
	 ProtoRegister(SERVO_PACKET_ID, &ServoParse);
	 for(uint8_t i = 0; i < SERVO_COUNT; i++)
		 PwmSet(i + SERVO_FIRST_PWM_CHANNEL, 0.f, SERVO_NEUTRAL, SERVO_DELTA);
}
