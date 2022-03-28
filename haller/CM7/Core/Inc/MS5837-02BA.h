/*
 * MS5837-02BA.h
 *
 *  Created on: 31.10.2019
 *      Author: Konto_U�ytkowe
 */

#include "main.h"
#include "math.h"
#include <string.h>

#ifndef MS5837_30BA_H_
#define MS5837_30BA_H_

#define MS5837_ADDRESS 				0xEC

#define MS5837_RESET 				0x1E
#define MS5837_ADC_READ				0x00
#define MS5837_PROM_READ			0xA0
#define MS5837_CONVERT_D1_8192		0x4A
#define MS5837_CONVERT_D2_8192		0x5A

#define MS5837_CONVERT_D1_4096		0x48
#define MS5837_CONVERT_D2_4096		0x58

enum{
	INIT_MS5837_02BA_FAIL,
	INIT_MS5837_02BA_SUCCESFUL
};

/*****************************************************************************************************
* @fn 					- MS5837_Init
 *
 * @brief				- Sent the reset sequence, to make sure that the calibration PROM gets loaded into the internal register
 * 						or to reset the device from unknown condition. Then check the data validity in memory (CRC).
 *
 * @param[in]			- base address of the peripheral
 * @param[in]			-
 *
 * @return				- initialization status
 *
 * @Note				- none
 *
 *****************************************************************************************************/
uint8_t MS5837_Init(I2C_HandleTypeDef *hi2c);


void MS5837_Read_PROM(I2C_HandleTypeDef *hi2c);

void MS5837_D1_Conversion(I2C_HandleTypeDef *hi2c);

void MS5837_D2_Conversion(I2C_HandleTypeDef *hi2c);

void MS5837_ADC_result(I2C_HandleTypeDef *hi2c);

float MS5837_Pressure(I2C_HandleTypeDef *hi2c);
float MS5837_Temperature(I2C_HandleTypeDef *hi2c);
float MS5837_Depth(I2C_HandleTypeDef *hi2c);
float MS5837_Altitude(I2C_HandleTypeDef *hi2c);


uint16_t MS5837_CRC4(uint16_t n_prom[]);

#endif /* MS5837_30BA_H_ */
