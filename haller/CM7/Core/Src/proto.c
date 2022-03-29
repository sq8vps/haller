#include "proto.h"

#include "motor.h"
#include "servo.h"
#include "pressure.h"
#include <string.h>
#include "lan.h"

void proto_dummy_handler(uint8_t id, uint8_t *buf, uint8_t len);
void proto_motor_wrapper(uint8_t id, uint8_t *buf, uint8_t len);
void proto_servo_wrapper(uint8_t id, uint8_t *buf, uint8_t len);
void proto_pressure_wrapper(uint8_t id, uint8_t *buf, uint8_t len);

#define PROTO_PAYLOAD_WORD_LENGTH 2
#define PROTO_PAYLOAD_MAX_LENGTH (256 / PROTO_PAYLOAD_WORD_LENGTH)

struct proto_module
{
	enum Command id;
	void (*callback)(uint8_t, uint8_t*, uint8_t);
};

#define MODULES_COUNT 6

struct proto_module proto_module_list[MODULES_COUNT] = {
		{255, &proto_dummy_handler},

		{RESPREQ_SET_RATE_OF_PRESSURE_SENSOR_REPORT, &proto_pressure_wrapper},
		{RESPREQ_GET_PRESSURE_SENSOR_VALUE_ONCE, &proto_pressure_wrapper},
		{NORESPREQ_SET_AZIMUTHAL_SERVOS, &proto_servo_wrapper},
		{NORESPREQ_SET_THRUSTERS, &proto_motor_wrapper},
		{NORESPREQ_SET_SERVOS, &proto_servo_wrapper},
};

void Proto_send(enum Command id, uint8_t *data, uint16_t len)
{
	if(len > (PROTO_PAYLOAD_MAX_LENGTH))
		return;

	uint8_t buf[PROTO_PAYLOAD_MAX_LENGTH * 2 + 2];
	buf[0] = (uint8_t)id;
	buf[1] = len;
	for(uint16_t i = 0; i < (len * 2); i++)
	{
		buf[2 + i] = data[i];
	}
	Lan_sendUdp(buf, len * 2 + 2);
}

void Proto_parse(uint8_t *buf, uint16_t *len)
{
	if(*len < 2)
		return;

	if((*len + 2) > PROTO_PAYLOAD_MAX_LENGTH)
		return;

	if((buf[1] * PROTO_PAYLOAD_WORD_LENGTH) != (*len - 2))
	{
		//TODO: respond with error?
		return;
	}

	uint8_t moduleNo = 255;

	for(uint8_t i = 0; i < MODULES_COUNT; i++) //match module
	{
		if(buf[0] == proto_module_list[i].id)
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
	Motor_sendData(torque);
}

void proto_servo_wrapper(uint8_t id, uint8_t *buf, uint8_t len)
{
	if(id == NORESPREQ_SET_SERVOS)
		Servo_set(buf[0], buf[2]);
	else if(id == NORESPREQ_SET_AZIMUTHAL_SERVOS)
	{
		Servo_set(0, buf[0]);
		Servo_set(1, buf[2]);
	}
}

void proto_pressure_wrapper(uint8_t id, uint8_t *buf, uint8_t len)
{
	if(id == RESPREQ_GET_PRESSURE_SENSOR_VALUE_ONCE)
	{
		Pressure_getAndSend();
	}
	else if(id == RESPREQ_SET_RATE_OF_PRESSURE_SENSOR_REPORT)
	{
		Pressure_setInterval((uint16_t)buf[0] | (uint16_t)(buf[1] << 8));
		Proto_send(RESP_RATE_OF_PRESSURE_SENSOR_REPORT_SET, buf, len);
	}
}
