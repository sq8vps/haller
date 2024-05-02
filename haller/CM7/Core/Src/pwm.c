#include "pwm.h"
#include "stm32h7xx_hal.h"

#define PWM_MAX_VALUE 9999

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim17;

void PwmInit(void)
{
	for(uint8_t i = 0; i < 12; i++)
		PwmSet(i, 0.0f);
}

void PwmSet(uint8_t channel, float value)
{
	TIM_HandleTypeDef *htim = NULL;
	uint32_t chan = 0;

	switch(channel)
	{
		case 0:
			chan = TIM_CHANNEL_1;
			htim = &htim3;
			break;
		case 1:
			chan = TIM_CHANNEL_2;
			htim = &htim3;
			break;
		case 2:
			chan = TIM_CHANNEL_3;
			htim = &htim3;
			break;
		case 4:
			chan = TIM_CHANNEL_4;
			htim = &htim3;
			break;
		case 5:
			chan = TIM_CHANNEL_1;
			htim = &htim4;
			break;
		case 6:
			chan = TIM_CHANNEL_2;
			htim = &htim4;
			break;
		case 7:
			chan = TIM_CHANNEL_3;
			htim = &htim4;
			break;
		case 8:
			chan = TIM_CHANNEL_4;
			htim = &htim4;
			break;
		case 9:
			chan = TIM_CHANNEL_1;
			htim = &htim15;
			break;
		case 10:
			chan = TIM_CHANNEL_2;
			htim = &htim15;
			break;
		case 11:
			chan = TIM_CHANNEL_1;
			htim = &htim17;
			break;
		default:
			return;
			break;
	}

	value += 1.f;
	value *= 0.5f;

	if(value < 0.f)
		value = 0.f;
	else if(value > 1.f)
		value = 1.f;

	__HAL_TIM_SET_COMPARE(htim, chan, (uint32_t)(value * (float)PWM_MAX_VALUE));
}
