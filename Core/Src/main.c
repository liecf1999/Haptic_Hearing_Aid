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
#include "dma.h"
#include "i2c.h"
#include "sai.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Driver_Driver.h"
#include "math.h"
#include <stdio.h>
#include "audio.h"
// #include "audio.h"
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
FlagStatus Data_Arrived_First_Half = RESET;
FlagStatus Data_Arrived_Second_Half = RESET;
FlagStatus Data_Arrived = RESET;
#define NUM_ACTUATORS 16

#define NUM_SAMPLES 512 // leads to 32ms
#define DMA_BUFFER_SIZE 2048 // = 4*NUM_SAMPLES (left right, and stops at half)

#define NUM_FREQ_BANDS 8 // Number of frequency bands

uint32_t rawdata[DMA_BUFFER_SIZE] = {0};

double t_sample = 0.0003; // 300us
uint16_t freq = 180; // 180 Hz (new actuator)
double sinus_sampletime = 0.0001; //100us

float fftSignal[NUM_SAMPLES];
float WindowedSignal[NUM_SAMPLES];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void generate_ramp(double *ramp, uint8_t maxvalue, uint8_t numValues);
void ramp_set(double *ramp, double *amplitudes, uint8_t counter, uint8_t numvalues);
//extern void initialise_monitor_handles(void); // for semihosting
void DWT_Init(void);
void delay_us(uint32_t us);

int __io_putchar(int ch) {
    return fputc(ch, stdout); // Semihosting handles this
}
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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM7_Init();
  MX_ADC1_Init();
  MX_SAI1_Init();
  /* USER CODE BEGIN 2 */


 init_drivers();
 DWT_Init();

	uint8_t numValues = 80;
	uint8_t maxValue = 0x5B; // 2.3V RMS

	double amplitudes[NUM_ACTUATORS] = {0};
	uint8_t counter = 0;
	int32_t audioData_left[NUM_SAMPLES];
	int32_t audioData_right[NUM_SAMPLES];

	generateHammingWindow();
	delay_estimator_init();

	uint8_t ready = 0;

	for(int j=0; j<NUM_ACTUATORS; j++){
		amplitudes[j] = 0;
	}

//	// Part for debugging
//	initialise_monitor_handles();
//
//	printf("Semihosting test...\n\r");

	char s[50];
	char *p;
	FILE *fp;
	p = s;


//	HAL_NVIC_EnableIRQ(TIM7_IRQn);
//	HAL_TIM_Base_Start_IT(&htim7);
	HAL_TIM_Base_Start(&htim7);

	HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint32_t *)rawdata, DMA_BUFFER_SIZE);


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
	  }

	  if(ready!=0){
		  // clear actual waveform with falling edge
		  HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, RESET);
		  delay_us(100);
		  // set new waveform
		  HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, SET);
		  ready = 0;
	  }

	  if(Data_Arrived_First_Half && (ready == 0)){
		  Data_Arrived_First_Half = RESET;
		  // Create lock file to have no conflicts with matlab
//		  FILE* lock = fopen("C:\\...\\Data_Left.lock", "w");
//		  if (lock) fclose(lock);  // just create and close

//		  fp=fopen("C:\\Users\\franc\\OneDrive\\Dokumente\\MATLAB\\Master_Thesis\\Data_Left.txt", "w");
		  for(int i=0; i<NUM_SAMPLES; i++){
			  // value is stored in four array parts (LSB left, MSB left, LSB right, MSB right)
			  // Sort it and value is 18-bit (MSB) in 2's complement
			  audioData_left[i] = ((int32_t)(rawdata[2*i] << 0)) >> 14;
			  audioData_right[i] = ((int32_t)(rawdata[2*i+1] << 0)) >> 14;
//			  int n= sprintf (p, "%d", (int) audioData_left[i]);
//			  fprintf(fp,p);
//			  fprintf(fp,"\n");
		  }
//		  fclose(fp);


		  __disable_irq();
		  process_signal(amplitudes, audioData_left, audioData_right, fftSignal, WindowedSignal);
		  set_amplitude(amplitudes);
		  __enable_irq();
//		  fp=fopen("C:\\Users\\franc\\OneDrive\\Dokumente\\MATLAB\\Master_Thesis\\Data_FFT.txt", "w");
//		  for(int i=0; i<NUM_SAMPLES; i++){
//			  int n= sprintf (p, "%d", (int) fftSignal[i]);
//			  fprintf(fp,p);
//			  fprintf(fp,"\n");
//		  }
//		  fclose(fp);

//		  fp=fopen("C:\\Users\\franc\\OneDrive\\Dokumente\\MATLAB\\Master_Thesis\\Data_Windowed.txt", "w");
//		  for(int i=0; i<NUM_SAMPLES; i++){
//			  int n= sprintf (p, "%d", (int) WindowedSignal[i]);
//			  fprintf(fp,p);
//			  fprintf(fp,"\n");
//		  }
//		  fclose(fp);
		  ready = 1;

		  // Delete lock file as signal for "done writing"
//		   remove("C:\\...\\Data_Left.lock");
	  }

	  if(Data_Arrived_Second_Half && (ready==0)){
		  Data_Arrived_Second_Half = RESET;
		  // Create lock file to have no conflicts with matlab
//		  FILE* lock = fopen("C:\\...\\Data_Left.lock", "w");
//		  if (lock) fclose(lock);  // just create and close
		  int offset = DMA_BUFFER_SIZE/2;
//
//		  fp=fopen("C:\\Users\\franc\\OneDrive\\Dokumente\\MATLAB\\Master_Thesis\\Data_Left.txt", "w");
		  for(int i=0; i<NUM_SAMPLES; i++){
//			  // value is stored in four array parts (LSB left, MSB left, LSB right, MSB right)
//			  // Sort it and value is 18-bit (MSB) in 2's complement
			  audioData_left[i] = ((int32_t)(rawdata[2*i + offset] << 0)) >> 14;
			  audioData_right[i] = ((int32_t)(rawdata[2*i+1 + offset] << 0)) >> 14;
//			  int n= sprintf (p, "%d", (int) audioData_left[i]);
//			  fprintf(fp,p);
//			  fprintf(fp,"\n");
		  }
//		  fclose(fp);

		  process_signal(amplitudes, audioData_left, audioData_right, fftSignal, WindowedSignal);
		  __disable_irq();
		  set_amplitude(amplitudes);
		  __enable_irq();
		  ready = 2;

		  // Delete lock file as signal for "done writing"
//		  remove("C:\\...\\Data_Left.lock");
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
 * @brief		DWT_Init
 *
 * @param       void
 *
 * @return		void
 *
 * @details		function to init delay in us
 *
 *
 * @author		Francis Liechti (FL)
 * @date		28.05.2025	FL	Created
 ****************************************************************************/
void DWT_Init(void) {
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    }
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/*
 * @brief		delay_us
 *
 * @param       uint32_t us
 *
 * @return		void
 *
 * @details		function to delay in us
 *
 *
 * @author		Francis Liechti (FL)
 * @date		28.05.2025	FL	Created
 ****************************************************************************/
void delay_us(uint32_t us) {
    uint32_t cycles = (SystemCoreClock / 1000000L) * us;
    uint32_t start = DWT->CYCCNT;
    while ((DWT->CYCCNT - start) < cycles);
}


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

}

/**
 * @brief		HAL_ADC_ConvCpltCallback
 *
 * @param       hadc
 *
 * @return		void
 *
 * @details		callback routine for ADC (DMA).
 *
 *
 * @author		Francis Liechti (FL)
 * @date		12.03.2025	FL	Created
 ****************************************************************************/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
}

/**
 * @brief		HAL_SAI_RxCpltCallback
 *
 * @param       hsai
 *
 * @return		void
 *
 * @details		callback routine for SAI (I2S DMA). Gets called when buffer is full
 *
 *
 * @author		Francis Liechti (FL)
 * @date		05.06.2025	FL	Created
 ****************************************************************************/
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai){
	if(hsai == &hsai_BlockA1){
		Data_Arrived_Second_Half = SET;
//		HAL_GPIO_WritePin(TIM_MEAS_GPIO_Port, TIM_MEAS_Pin, SET);
	}
}

/**
 * @brief		HAL_SAI_RxHalfCpltCallback
 *
 * @param       hsai
 *
 * @return		void
 *
 * @details		callback routine for SAI (I2S DMA). Gets called when buffer is half full
 *
 *
 * @author		Francis Liechti (FL)
 * @date		05.06.2025	FL	Created
 ****************************************************************************/
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai){
	if(hsai == &hsai_BlockA1){
		Data_Arrived_First_Half = SET;
//		HAL_GPIO_WritePin(TIM_MEAS_GPIO_Port, TIM_MEAS_Pin, RESET);
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
