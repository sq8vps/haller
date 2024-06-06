#include "gripper.h"
#include "proto.h"
#include "pwm.h"

#define GRIPPER_PACKET_ID 0x04
#define GRIPPER_PACKET_LENGTH 1

#define GRIPPER_CHANNEL 9

#define GRIPPER_NEUTRAL 1500
#define GRIPPER_DELTA 300

static void GripperParse(void *buffer, uint8_t len)
{
	if(len < GRIPPER_PACKET_LENGTH)
		return;

	uint8_t val = ((uint8_t*)buffer)[0];

	PwmSet(GRIPPER_CHANNEL, val ? -1.f : 1.f, GRIPPER_NEUTRAL, GRIPPER_DELTA);
}

void GripperInit(void)
{
	ProtoRegister(GRIPPER_PACKET_ID, &GripperParse);
}
