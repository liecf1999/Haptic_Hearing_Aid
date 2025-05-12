/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Driver_Driver.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
FlagStatus TimCollapsed = RESET;
FlagStatus DMA_ready = RESET;
#define NUM_ACTUATORS 16

double t_sample = 0.0003; // 300us
uint16_t freq = 180; // 180 Hz (new actuator)
double sinus_sampletime = 0.0001; //100us
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void generate_ramp(double *ramp, uint8_t maxvalue, uint8_t numValues);
void ramp_set(double *ramp, double *amplitudes, uint8_t counter, uint8_t numvalues);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM7_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */


	init_drivers();

	uint8_t numValues = 80;
	double ramp[numValues];
	uint8_t maxValue = 0x5B; // 2.3V RMS

	generate_ramp(ramp, maxValue, numValues);
	double amplitudes[NUM_ACTUATORS] = {0};
	uint8_t counter = 0;

	uint16_t numSamples = 288;
	uint8_t rawValues[numSamples];
	uint8_t audioData[numSamples];

	uint8_t drivers = 0;
	uint8_t firstround = 0;

	uint8_t ready = 0;

	for(int j=0; j<NUM_ACTUATORS; j++){
		amplitudes[j] = 0;
	}


	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	HAL_TIM_Base_Start_IT(&htim7);

	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, RESET);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(TimCollapsed){
		TimCollapsed = RESET;
		counter++;
		int i = counter/10;

		if(i>=NUM_ACTUATORS){
			counter = 0;
		}
		for(int j=0; j<NUM_ACTUATORS; j++){
			amplitudes[j] = 0;
		}
		amplitudes[i] = maxValue;

		/*
		if(counter > NUM_ACTUATORS*numValues){
			counter = 0;
		}*/
		// ramp_set(ramp,amplitudes,counter,numValues);
		set_amplitude(amplitudes);
		ready = 1;
	  }

	  if(ready){
		  ready = 0;
		  // clear actual waveform with rising edge
		  HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, RESET);
		  HAL_Delay(1);
		  HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, SET);
		  HAL_Delay(5);
		  // set new waveform with rising edge
		  HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, RESET);
		  HAL_Delay(1);
		  HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, SET);
	  }

	  if(DMA_ready){
		  DMA_ready = RESET;
		  for(int i=0; i<numSamples; i++){
			  audioData[i] = rawValues[i];
		  }
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin==INT_Pin){
		dataArrived = SET;
	}
	if(GPIO_Pin == NEW_MEAS_Pin){
		Restart = SET;
	}
}*/

/**
 * @brief		HAL_TIM_PeriodElapsedCallback
 *
 * @param       htim
 *
 * @return		void
 *
 * @details		callback routine for timer interrupt.
 *
 *
 * @author		Francis Liechti (FL)
 * @date		12.03.2025	FL	Created
 ****************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM7){
		TimCollapsed = SET;
	}
}

/**
 * @brief		HAL_GPIO_EXTI_Callback
 *
 * @param       GPIO_Pin
 *
 * @return		void
 *
 * @details		callback routine for GPIO interrupt.
 *
 *
 * @author		Francis Liechti (FL)
 * @date		27.03.2025	FL	Created
 ****************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//	if(B1_INT_Pin == GPIO_Pin){
//		Button = SET;
//	}
}

/**
 * @brief		HAL_ADC_ConvCpltCallback
 *
 * @param       hadc
 *
 * @return		void
 *
 * @details		callback routine for DMA (ADC).
 *
 *
 * @author		Francis Liechti (FL)
 * @date		12.03.2025	FL	Created
 ****************************************************************************/
/*void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	DMA_ready = SET;
}*/


/**
 * @brief		ramp
 *
 * @param       double *ramp
 *
 * @param		double maxvalue
 *
 * @param		uint8_t numValues
 *
 * @return		void
 *
 * @details		function to calculate ramp
 *
 *
 * @author		Francis Liechti (FL)
 * @date		12.03.2025	FL	Created
 ****************************************************************************/
void generate_ramp(double *ramp, uint8_t maxvalue, uint8_t numValues){
	// numValues only for whole triangle
	double difference = 2*((double) maxvalue/ (double)numValues);
	for(int i=0; i<numValues; i++){
		if(i<=numValues/2){
			ramp[i] = i*difference;
		} else {
			ramp[i] = ((double)maxvalue-(i-numValues/2)*difference);
		}
	}
}

/**
 * @brief		ramp
 *
 * @param		double *ramp
 *
 * @param       double *amplitudes
 *
 * @param		uint8_t counter
 *
 * @param		uint8_t numvalues
 *
 * @return		void
 *
 * @details		function to calculate next value of a ramp function
 *
 *
 * @author		Francis Liechti (FL)
 * @date		12.03.2025	FL	Created
 ****************************************************************************/
void ramp_set(double *ramp, double *amplitudes, uint8_t counter, uint8_t numvalues){
	uint8_t countval = counter%(numvalues/2);
	uint8_t countval2 = countval + (numvalues/2);
	uint8_t number = counter/(numvalues/2);
	uint8_t number2 = (counter >= numvalues / 2) ? (number - 1) : 0;

	for(int i=0; i<NUM_ACTUATORS; i++){
		amplitudes[i] = 0;
	}

	amplitudes[number] = ramp[countval];
	if(counter >= numvalues/2){
		amplitudes[number2] = ramp[countval2];
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
