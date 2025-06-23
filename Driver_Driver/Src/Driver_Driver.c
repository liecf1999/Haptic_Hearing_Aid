/****************************************************************************/
/*  Source    : Driver_Driver.c												*/
/****************************************************************************/
/*                                                                          */
/*  @file     : driver_driver.c												*/
/*                                                                          */
/*  @brief	  : File to handle LRA Driver									*/
/*                                                                          */
/*  @author   : Francis Liechti (FL)                                        */
/*                                                                          */
/*  @date	  : 11.03.2025  				                        		*/
/*                                                                          */
/****************************************************************************/

/*----- Header-Files -----------------------------------------------------*/
#include "Driver_Driver.h"
#include "DRV2506.h"
#include "math.h"

/*----- Defines  ---------------------------------------------------------*/
#define TIMEOUT 100

#define MAX_BUFFER_SIZE 4
#define NUM_ACTUATORS 16

#define SWITCH_ADDRESS_1 0x70 << 1
#define SWITCH_ADDRESS_2 0x71 << 1

#define PI 3.14159265358979323846


/*----- Data ---------------------------------------------------------*/

uint8_t Buf[MAX_BUFFER_SIZE] = {0}; // Buffer for I2C-data


// Init data
// Library Mode

//uint8_t set_mode[2] = {REG_MODE, MODE_EDGE};
uint8_t set_mode[2] = {REG_MODE, MODE_LEVEL};
uint8_t set_LRA[2] = {REG_FEEDBACK_CTRL, MODE_LRA | FB_BRAKE_FACTOR | LOOP_GAIN | BACK_EMF_GAIN};
uint8_t set_Control3[2] = {REG_CONTROL3, NG_THRESHOLD | AUTO_CALIBRATION};
uint8_t start_auto_calibration[2] = {REG_MODE,  MODE_AUTO_CALIBRATION};
uint8_t set_timing[2] = {REG_CONTROL2, BIDIR_INPUT | BRAKE_STABILIZER | SAMPLE_TIME_300 | BLANKING_TIME_75 | IDISS_TIME_75};
uint8_t set_LRA_settings[2] = {REG_CONTROL1, DRIVE_TIME};
uint8_t set_Clamp[2] = {REG_OVERDRIVE_CLAMP_VOLTAGE, CLAMP};
uint8_t dev_RESET[2] = {REG_MODE, 0x00};
uint8_t set_library[2] = {REG_LIBRARY, 0x06};
uint8_t set_waveform[2] = {REG_WF_SEQ_1, 0x0F};
uint8_t set_waveform2[2] = {REG_WF_SEQ_2, 0x00};
uint8_t set_auto_OL[2] = {REG_CONTROL5, LRA_AUTO_OPEN_LOOP};
uint8_t set_period[2] = {REG_LRA_OPEN_LOOP_PERIOD, LRA_PERIOD};
uint8_t set_rated_voltage[2] = {REG_RATED_VOLTAGE, RATED_VOLTAGE};
uint8_t GO[2] = {REG_GO, 0x01};
uint8_t clear_GO[2] = {REG_GO, 0x00};

uint8_t A_CAL_COMP = 0;
uint8_t A_CAL_BEMF = 0;
uint8_t status = 0;
uint8_t calib_complete = 0;
uint8_t LRA_period = 0;

/*----- Private variables ---------------------------------------------------------*/


/*----- Functions  ----------------------------------------------------------------*/

/**
 * @brief		init_drivers
 *
 * @param       void
 *
 * @return		void
 *
 * @details		Function to initialize the drivers.
 *
 *
 * @author		Francis Liechti (FL)
 * @date		11.03.2025	FL	Created
 ****************************************************************************/
void init_drivers(void){
	// First enable the drivers and the MUX
	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, RESET);
	HAL_GPIO_WritePin(N_RES_GPIO_Port, N_RES_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DRIVE_EN_GPIO_Port, DRIVE_EN_Pin , GPIO_PIN_SET);

	// Set HW-addresses for the MUX
	HAL_GPIO_WritePin(A0_1_GPIO_Port, A0_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A1_1_GPIO_Port, A1_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A2_1_GPIO_Port, A2_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A0_2_GPIO_Port, A0_2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(A1_2_GPIO_Port, A1_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(A2_2_GPIO_Port, A2_2_Pin, GPIO_PIN_RESET);

	// Now init the drivers
	// Have to do it for every actuator
	// So first chose the actuator (GPIOs A0, A1, A2)
	for(uint8_t i=0; i<NUM_ACTUATORS; i++){
		select_driver(i);
		calib_complete = 0x02;
		HAL_Delay(10);
		int a = 0;

		// Send the initializations
		// Library Mode

		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, dev_RESET, sizeof(dev_RESET), TIMEOUT) == HAL_OK){
			a++;
		}
		HAL_Delay(100);
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_rated_voltage, sizeof(set_rated_voltage), TIMEOUT) == HAL_OK){
			a++;
		}
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_Clamp, sizeof(set_Clamp), TIMEOUT) == HAL_OK){
			a++;
		}
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_LRA, sizeof(set_LRA), TIMEOUT) == HAL_OK){
			a++;
		}
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, start_auto_calibration, sizeof(start_auto_calibration), TIMEOUT) == HAL_OK){
			a++;
		}
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, GO, sizeof(GO), TIMEOUT) == HAL_OK){
			a++;
		}

		uint8_t reg = REG_GO;
		// If every actuator working, this could be the way to go
		// else you have here an endless loop
		/*
		while(calib_complete != 0){
			HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, &reg, 1, TIMEOUT);
			if(HAL_I2C_Master_Receive(&hi2c1, DEV_ADDRESS, Buf, 1, TIMEOUT) == HAL_OK){
				calib_complete = Buf[0];
			}

		}*/
		HAL_Delay(2000);
		HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, &reg, 1, TIMEOUT);
		if(HAL_I2C_Master_Receive(&hi2c1, DEV_ADDRESS, Buf, 1, TIMEOUT) == HAL_OK){
			calib_complete = Buf[0];
		}

		reg = REG_STATUS;
		HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, &reg, 1, TIMEOUT);
		if(HAL_I2C_Master_Receive(&hi2c1, DEV_ADDRESS, Buf, 1, TIMEOUT) == HAL_OK){
			status = Buf[0];
		}

		reg = REG_AUTO_CALIBRATION_RESULT;
		HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, &reg, 1, TIMEOUT);
		if(HAL_I2C_Master_Receive(&hi2c1, DEV_ADDRESS, Buf, 1, TIMEOUT) == HAL_OK){
			A_CAL_COMP = Buf[0];
		}
		reg = REG_AUTO_CALIBRATION_BACK_EMF_RESULT;
		HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, &reg, 1, TIMEOUT);
		if(HAL_I2C_Master_Receive(&hi2c1, DEV_ADDRESS, Buf, 1, TIMEOUT) == HAL_OK){
			A_CAL_BEMF = Buf[0];
		}

		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_LRA_settings, sizeof(set_LRA_settings), TIMEOUT) == HAL_OK){
			a++;
		}
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_timing, sizeof(set_timing), TIMEOUT) == HAL_OK){
			a++;
		}

		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_Control3, sizeof(set_Control3), TIMEOUT) == HAL_OK){
			a++;
		}
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_library, sizeof(set_library), TIMEOUT) == HAL_OK){
			a++;
		}
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_mode, sizeof(set_mode), TIMEOUT) == HAL_OK){
			a++;
		}

		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_period, sizeof(set_period), TIMEOUT) == HAL_OK){
			a++;
		}
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_waveform, sizeof(set_waveform), TIMEOUT) == HAL_OK){
			a++;
		}
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_waveform2, sizeof(set_waveform), TIMEOUT) == HAL_OK){
			a++;
		}

		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, set_auto_OL, sizeof(set_auto_OL), TIMEOUT) == HAL_OK){
			a++;
		}

		// Send a trigger to have waveform while measuring resonance period
		// clear actual waveform with falling edge
		HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, RESET);
		HAL_Delay(1);
		// Set new waveform with rising edge
		HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, SET);
		HAL_Delay(10);
		reg = REG_LRA_RESONANCE_PERIOD;
		HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, &reg, 1, TIMEOUT);
		if(HAL_I2C_Master_Receive(&hi2c1, DEV_ADDRESS, Buf, 1, TIMEOUT) == HAL_OK){
			LRA_period = Buf[0];
		}

		// Set all actuators to 0V
		uint8_t send[2] = {REG_RATED_VOLTAGE, 0};
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, send, sizeof(send), TIMEOUT) == HAL_OK){
			int a = 1;
		}
	}
}

/**
 * @brief		set_amplitude
 *
 * @param       double amplitudes
 *
 * @return		void
 *
 * @details		Function to set amplitude for all drivers.
 *
 *
 * @author		Francis Liechti (FL)
 * @date		12.03.2025	FL	Created
 ****************************************************************************/
void set_amplitude(double *amplitudes){
	uint8_t value = amplitudes[0];

	for(int i=0; i<NUM_ACTUATORS; i++){
		select_driver(i);
		value = (uint8_t)(amplitudes[i]); // calculation: datasheet p.21
		uint8_t send[2] = {REG_RATED_VOLTAGE, value};
		if(HAL_I2C_Master_Transmit(&hi2c1, DEV_ADDRESS, send, sizeof(send), TIMEOUT) == HAL_OK){
			int a = 1;
		}
	}
}

/**
 * @brief		select_driver
 *
 * @param       number of the driver
 *
 * @return		void
 *
 * @details		Function to select driver.
 *
 *
 * @author		Francis Liechti (FL)
 * @date		12.03.2025	FL	Created
 ****************************************************************************/
void select_driver(uint8_t number){
	int a = 0;
	uint8_t value = 0;
	// Disable other MUX if necessary
	if(number == 0){
		if(HAL_I2C_Master_Transmit(&hi2c1, SWITCH_ADDRESS_2, &value, sizeof(value), TIMEOUT) == HAL_OK){
			a = 1;
		}
	} else if(number == 8){
		if(HAL_I2C_Master_Transmit(&hi2c1, SWITCH_ADDRESS_1, &value, sizeof(value), TIMEOUT) == HAL_OK){
			a = 1;
		}
	}

	if(number<8){
		value = 1<<number;
		if(HAL_I2C_Master_Transmit(&hi2c1, SWITCH_ADDRESS_1, &value, sizeof(value), TIMEOUT) == HAL_OK){
			a = 1;
		}
	} else{
		value = 1<<(number-8);
		if(HAL_I2C_Master_Transmit(&hi2c1, SWITCH_ADDRESS_2, &value, sizeof(value), TIMEOUT) == HAL_OK){
			a = 1;
		}
	}
}
