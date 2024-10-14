#include "pwm.h"
#include "stm32h7xx_hal.h"

//timer run at 500 kHz with 10000 reload value = PWM @ 50 Hz
#define PWM_DELTA 250
#define PWM_NEUTRAL 500

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim17;

void PwmInit(void)
{
	for(uint8_t i = 0; i < 12; i++)
		PwmSet(i, 0.0f, PWM_NEUTRAL, PWM_DELTA);
}

void PwmSet(uint8_t channel, float value, uint32_t neutral, uint32_t delta)
{
	//convert from microseconds to 2 us per tick
	neutral >>= 1;
	delta >>= 1;

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
		case 3:
			chan = TIM_CHANNEL_4;
			htim = &htim3;
			break;
		case 4:
			//chan = TIM_CHANNEL_1;
			chan = TIM_CHANNEL_4;
			htim = &htim4;
			break;
		case 5:
			chan = TIM_CHANNEL_2;
			htim = &htim4;
			break;
		case 6:
			chan = TIM_CHANNEL_3;
			htim = &htim4;
			break;
		case 7:
//			chan = TIM_CHANNEL_4;
//			htim = &htim4;
			break;
		case 8:
			chan = TIM_CHANNEL_1;
			htim = &htim15;
			break;
		case 9:
			chan = TIM_CHANNEL_2;
			htim = &htim15;
			break;
		case 10:
			chan = TIM_CHANNEL_1;
			htim = &htim17;
			break;
		default:
			return;
			break;
	}

	if(value < -1.f)
		value = -1.f;
	else if(value > 1.f)
		value = 1.f;

	if(NULL != htim)
		__HAL_TIM_SET_COMPARE(htim, chan, (int32_t)(value * (float)delta) + (int32_t)neutral);
}
