#include "pressure.h"
#include "MS5837-02BA.h"
#include "stm32h7xx.h"
#include "CommunicationCodes.h"

extern I2C_HandleTypeDef hi2c2;

uint32_t pressure_nextMeasurement = 0;
uint32_t pressure_interval = 0;

void Pressure_getAndSend(void)
{
	uint32_t t = (uint32_t)MS5837_Pressure(&hi2c2) * 100;
	uint8_t buf[4];
	buf[0] = t & 0xFF;
	buf[1] = (t >> 8) & 0xFF;
	buf[2] = (t >> 16) & 0xFF;
	buf[3] = (t >> 24) & 0xFF;
//	Proto_send(RESP_PRESSURE, buf, 2);
}



void Pressure_setInterval(uint16_t interval)
{
	pressure_interval = interval / HAL_GetTickFreq();
}

void Pressure_init(void)
{
	if(HAL_I2C_IsDeviceReady(&hi2c2, MS5837_ADDRESS, 10, 100) != HAL_OK) //check if sensor is ready
		NVIC_SystemReset();
	//TODO: is this OK?

	if(MS5837_Init(&hi2c2) == INIT_MS5837_02BA_FAIL)
		NVIC_SystemReset();

	Pressure_setInterval(PRESSURE_DEFAULT_INTERVAL);
}

void Pressure_refresh(void)
{
	if(pressure_interval == 0)
		return;

	if(HAL_GetTick() >= pressure_nextMeasurement)
	{
		pressure_nextMeasurement = HAL_GetTick() + pressure_interval;
		Pressure_getAndSend();
	}
}
