#include "proto.h"

#include "motor.h"

void proto_dummy_handler(uint8_t id, uint8_t *buf, uint8_t len);
void proto_motor_wrapper(uint8_t id, uint8_t *buf, uint8_t len);

#define NULL (void*)0

#define PROTO_PAYLOAD_MAX_LENGTH 255

struct proto_module
{
	Module_id id;
	void (*callback)(uint8_t, uint8_t*, uint8_t);
};

#define MODULES_COUNT 1

struct proto_module proto_module_list[MODULES_COUNT] = {
		{MODULE_NONE, &proto_dummy_handler},
		{MODULE_STEVAL, &proto_motor_wrapper},
};


void Proto_parse(uint8_t *buf, uint16_t *len)
{
	if(*len < 2)
		return;

	if((*len + 2) > PROTO_PAYLOAD_MAX_LENGTH)
		return;

	if(buf[1] != (*len - 2))
	{
		//TODO: respond with error?
		return;
	}

	uint8_t moduleNo = 255;

	for(uint8_t i = 0; i < MODULES_COUNT; i++) //match module
	{
		if((buf[0] & 0xFE) == proto_module_list[i].id)
		{
			moduleNo = i;
		}
	}

	if(moduleNo == 255) //no matching module
	{
		//TODO: respond with error?
		return;
	}

	if(proto_module_list[moduleNo].callback != NULL)
		proto_module_list[moduleNo].callback(buf[0], buf + 2, buf[1]); //call callback function

}


void proto_dummy_handler(uint8_t id, uint8_t *buf, uint8_t len)
{

}

void proto_motor_wrapper(uint8_t id, uint8_t *buf, uint8_t len)
{
	uint16_t torque[MOTOR_COUNT];
	memcpy(torque, buf, 2 * MOTOR_COUNT);
	Motor_send(torque);
}
