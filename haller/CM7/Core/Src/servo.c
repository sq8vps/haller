#include "servo.h"
#include "proto.h"
#include "stm32h7xx_hal.h"

#define SERVO_PACKET_ID 0x03
#define SERVO_PACKET_LENGTH 5

#define SERVO_DEFAULT_POSITION		5000
#define SERVO_MAX_DIFF				3333

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim15;

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
	if(p->number > 3) //TODO: insert correct value
		return;

	//TODO: convert radians to PWM value and apply
}

void ServoInit(void)
{
	 TIM3->CCR1 = SERVO_DEFAULT_POSITION;
	 TIM3->CCR2 = SERVO_DEFAULT_POSITION;
	 TIM3->CCR3 = SERVO_DEFAULT_POSITION;
	 TIM3->CCR4 = SERVO_DEFAULT_POSITION;
	 HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	 HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	 HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	 HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	 HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
	 TIM15->CCR1 = SERVO_DEFAULT_POSITION;
	 HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
	 TIM15->CCR2 = SERVO_DEFAULT_POSITION;

	 ProtoRegister(SERVO_PACKET_ID, &ServoParse);
}


static void ServoSet(uint8_t no, uint8_t val)
{
	val %= 101;
	int32_t duty;
	duty = ((int32_t)val - 50) * 2 * SERVO_MAX_DIFF;
	duty /= 100;
	duty += SERVO_DEFAULT_POSITION;

	switch(no)
	{
		case 0:
			TIM15->CCR1 = (uint16_t)duty;
			break;
		case 1:
			TIM15->CCR2 = (uint16_t)duty;
			break;
		case 2:
			TIM3->CCR1 = (uint16_t)duty;
			break;
		case 3:
			TIM3->CCR2 = (uint16_t)duty;
			break;
		case 4:
			TIM3->CCR3 = (uint16_t)duty;
			break;
		case 5:
			TIM3->CCR4 = (uint16_t)duty;
			break;
		default:
			break;
	}
}
