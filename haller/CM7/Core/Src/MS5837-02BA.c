/*
 * MS5837-30BA.c
 *
 *  Created on: 31.10.2019
 *      Author: Konto_U�ytkowe
 */

#include <MS5837-02BA.h>

#define I2C_TIMEOUT		10

const float fluidDentisity = 1029;
const float Pa = 100.0f;
const float bar = 0.001f;
const float mbar = 1.0f;

static uint16_t C[8];
static uint32_t D1, D2;
static float TEMP;
static float P;


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
uint8_t MS5837_Init(I2C_HandleTypeDef *hi2c){

	uint8_t reg = MS5837_RESET;

	if( HAL_I2C_Master_Transmit(hi2c, MS5837_ADDRESS, &reg, 1, I2C_TIMEOUT) != HAL_OK ){

		return 0;

	}else{

		HAL_Delay(10); // wait for reset to complete


		MS5837_Read_PROM(hi2c);

		// Verify that data is correct with CRC
		uint8_t crcRead = C[0] >> 12;
		uint8_t crcCalculated = MS5837_CRC4(C);

		if (crcCalculated == crcRead) {
			return INIT_MS5837_02BA_SUCCESFUL; // Initialization success
		}

		return INIT_MS5837_02BA_FAIL; // CRC fail

	}

}

/*****************************************************************************************************
* @fn 					- MS5837_Read_PROM
 *
 * @brief				- Should be executed once after reset by  the user to read the content of the
 * 						calibration PROM and to calculate calibration coefficient.
 *
 * @param[in]			- base address of the peripheral
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 *
 *****************************************************************************************************/
void MS5837_Read_PROM(I2C_HandleTypeDef *hi2c){

	uint8_t reg;
	uint8_t i2c_data[2];


	// Read calibration values and CRC
	for (uint8_t i = 0; i < 8; i++) {

		reg = MS5837_PROM_READ + i * 2;
		HAL_I2C_Master_Transmit(hi2c, MS5837_ADDRESS,  &reg, 1, I2C_TIMEOUT);

		HAL_I2C_Master_Receive(hi2c, MS5837_ADDRESS, i2c_data, 2, I2C_TIMEOUT);

		C[i] = (((int16_t)i2c_data[0]) << 8) | i2c_data[1];

	}

}

/*****************************************************************************************************
* @fn 					- MS5837_D1_Conversion
 *
 * @brief				- Initiate uncompensated pressure conversion (D1), then reading value of uncompensated pressure.
 *
 * @param[in]			- base address of the peripheral
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 *
 *****************************************************************************************************/
void MS5837_D1_Conversion(I2C_HandleTypeDef *hi2c){ // uncompensated pressure

	uint8_t reg;
	uint8_t i2c_data[3];

	// Request D1 conversion
	reg = MS5837_CONVERT_D1_8192;
	HAL_I2C_Master_Transmit(hi2c, MS5837_ADDRESS,  &reg, 1, I2C_TIMEOUT);

	HAL_Delay(20); // Max conversion time per datasheet

	reg = MS5837_ADC_READ;
	HAL_I2C_Master_Transmit(hi2c, MS5837_ADDRESS,  &reg, 1, I2C_TIMEOUT);
	HAL_I2C_Master_Receive(hi2c, MS5837_ADDRESS, i2c_data, 3, I2C_TIMEOUT);

	D1 = 0;
	D1 = i2c_data[0];
	D1 = (D1 << 8) | i2c_data[1];
	D1 = (D1 << 8) | i2c_data[2];

}

/*****************************************************************************************************
* @fn 					- MS5837_D2_Conversion
 *
 * @brief				- Initiate uncompensated temperature conversion (D2), then reading value of uncompensated temperature.
 *
 * @param[in]			- base address of the peripheral
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 *
 *****************************************************************************************************/
void MS5837_D2_Conversion(I2C_HandleTypeDef *hi2c){ // uncompensated temperature

	uint8_t reg;
	uint8_t i2c_data[3];

	// Request D2 conversion
	reg = MS5837_CONVERT_D2_8192;
	HAL_I2C_Master_Transmit(hi2c, MS5837_ADDRESS, &reg, 1, I2C_TIMEOUT);

	HAL_Delay(20); // Max conversion time per datasheet

	reg = MS5837_ADC_READ;
	HAL_I2C_Master_Transmit(hi2c, MS5837_ADDRESS, &reg, 1, I2C_TIMEOUT);
	HAL_I2C_Master_Receive(hi2c, MS5837_ADDRESS, i2c_data, 3, I2C_TIMEOUT);

	D2 = 0;
	D2 = i2c_data[0];
	D2 = (D2 << 8) | i2c_data[1];
	D2 = (D2 << 8) | i2c_data[2];

}

/*****************************************************************************************************
* @fn 					- MS5837_ADC_result
 *
 * @brief				- Calculate temperature and pressure second order temperature compensated.
 *
 * @param[in]			- base address of the peripheral
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 *
 *****************************************************************************************************/
void MS5837_ADC_result(I2C_HandleTypeDef *hi2c){

	// calibration coefficient
	float dT = 0;
	float SENS = 0;
	float OFF = 0;
	float SENSi = 0;
	float OFFi = 0;
	float Ti = 0;
	float OFF2 = 0;
	float SENS2 = 0;

	// read uncompensated temperature value
	MS5837_D2_Conversion(hi2c);
	// read uncompensated pressure value
	MS5837_D1_Conversion(hi2c);

	// calculate temperature
	dT = (float)D2 - ((float)C[5] * 256);
	TEMP = 2000 + (dT * (float)C[6] / 8388608);

	// calculate first order temperature compensated pressure
	OFF = ((float)C[2] * 131072) + (((float)C[4] * dT) / 64);
	SENS = ((float)C[1] * 65536) + ((float)C[3] * dT / 128);
	P = (((float)D1 * SENS/2097152) - OFF) / 32768;

	// second order temperature compensation
	if((TEMP/100)<20){ //Low temp
		Ti = (11*dT*dT)/(34359738368);
		OFFi = (31*(TEMP-2000)*(TEMP-2000))/8;
		SENSi = (63*(TEMP-2000)*(TEMP-2000))/32;
	}

	// calculate temperature and pressure second order temperature compensation
	OFF2 = OFF - OFFi;
	SENS2 = SENS - SENSi;
	TEMP = (TEMP - Ti)/100;
	P = ((((float)D1*SENS2)/2097152-OFF2)/32768)/100;

}

/*****************************************************************************************************
* @fn 					- MS5837_Pressure
 *
 * @brief				- calculate pressure value
 *
 * @param[in]			- base address of the peripheral
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 *
 *****************************************************************************************************/
float MS5837_Pressure(I2C_HandleTypeDef *hi2c){ // mbar (HPa)

	MS5837_ADC_result(hi2c);
	return P;

}

/*****************************************************************************************************
* @fn 					- MS5837_Temperature
 *
 * @brief				- calculate temperature value
 *
 * @param[in]			- base address of the peripheral
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 *
 *****************************************************************************************************/
float MS5837_Temperature(I2C_HandleTypeDef *hi2c){ // C

	MS5837_ADC_result(hi2c);
	return TEMP;

}

/*****************************************************************************************************
* @fn 					- MS5837_Depth
 *
 * @brief				- calculates the depth relative to the water surface
 *
 * @param[in]			- base address of the peripheral
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 *
 *****************************************************************************************************/
float MS5837_Depth(I2C_HandleTypeDef *hi2c){ // m

	return (MS5837_Pressure(hi2c)-1013.25)/(fluidDentisity*9.80665);

}

/*****************************************************************************************************
* @fn 					- MS5837_Altitude
 *
 * @brief				-
 *
 * @param[in]			- base address of the peripheral
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 *
 *****************************************************************************************************/
float MS5837_Altitude(I2C_HandleTypeDef *hi2c){ // m

	return (1-pow((MS5837_Pressure(hi2c)/1013.25),0.190284))*145366.45*0.3048;

}

/*****************************************************************************************************
* @fn 					- MS5837_CRC4
 *
 * @brief				- CRC-4 (cyclic redundancy check) calculation
 *
 * @param[in]			- PROM calibration table
 * @param[in]			-
 *
 * @return				- none
 *
 * @Note				- none
 *
 *****************************************************************************************************/
uint16_t MS5837_CRC4(uint16_t n_prom[]){

	uint16_t n_rem = 0;

	n_prom[0] = ((n_prom[0]) & 0x0FFF);
	n_prom[7] = 0;

	for (uint8_t i = 0; i < 16; i++) {

		if (i % 2 == 1) {
			n_rem ^= (uint16_t) ((n_prom[i >> 1]) & 0x00FF);
		} else {
			n_rem ^= (uint16_t) (n_prom[i >> 1] >> 8);
		}

		for (uint8_t n_bit = 8; n_bit > 0; n_bit--) {
			if (n_rem & 0x8000) {
				n_rem = (n_rem << 1) ^ 0x3000;
			} else {
				n_rem = (n_rem << 1);
			}
		}

	}

	n_rem = ((n_rem >> 12) & 0x000F);

	return (n_rem ^ 0x00);

}

