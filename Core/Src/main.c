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
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "stdio.h"
#include "math.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define START_BUTTON GPIO_PIN_3
//#define START_BUTTON_GPIO_PIN_PORT GPIOB
//
//#define TS_ACTIVE GPIO_PIN_1
//#define TS_ACTIVE_GPIO_PIN_PORT GPIOB
//
////#define GSS GPIO_PIN_3
////#define GSS_GPIO_PIN_PORT GPIOB
//
////#define BRAKE_LIGHT GPIO_PIN_6
////#define BRAKE_LIGHT_GPIO_PIN_PORT GPIOB
//
//#define BUZZER GPIO_PIN_7
//#define BUZZER_GPIO_PIN_PORT GPIOB
//
//#define TSAL_RED GPIO_PIN_5
//#define TSAL_RED_GPIO_PIN_PORT GPIOA

#define MC_ENABLE GPIO_PIN_5
#define MC_ENABLE_GPIO_PIN_PORT GPIOB

#define R2D_OUT GPIO_PIN_4
#define R2D_OUT_GPIO_PIN_PORT GPIOB
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//uint16_t readValue1;
//uint16_t readValue2;
//uint16_t ch1, ch4;
uint8_t tps1_raw = 0;
uint8_t tps2_raw = 0;
float percent1;
float percent2;
uint32_t tps_values[2];
uint32_t adc_buf[2];
float tps1 = 0, tps2 = 0, diff = 0;
float voltage=0, pressure=0;
int r2d=GPIO_PIN_RESET;
int mc=GPIO_PIN_RESET;
//int tsal_red=GPIO_PIN_RESET;
//int ts= GPIO_PIN_RESET;
//int start= GPIO_PIN_RESET;
//int gss= GPIO_PIN_RESET;

//MC_ENABLE  = GPIO_PIN_RESET;
//R2D_OUT =GPIO_PIN_RESET;
//TSAL_RED =GPIO_PIN_RESET;


int counter = 1;
unsigned long previous_millis = 0;
unsigned long current_millis=0;
char msg[100];
char msg1[100];
//uint16_t adc_buffer[2];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int write (int file,char *ptr ,int len ){
	HAL_UART_Transmit(&huart2,(uint8_t*)ptr,len,HAL_MAX_DELAY);
	return len ;
}
//void safe_state(void){
//	if(((ts == GPIO_PIN_SET)&(tsal_red == GPIO_PIN_SET)) || ((ts == GPIO_PIN_RESET)&(tsal_red == GPIO_PIN_RESET))){
//
//		HAL_GPIO_WritePin(GPIOB, GSS, GPIO_PIN_SET);
//	}
//	else{
//
//		HAL_GPIO_WritePin(GPIOB, GSS, GPIO_PIN_RESET);
//	}
//}
//void R2D_status(void){
//	if (counter == 0){
//		if ((start == GPIO_PIN_SET) & (ts == GPIO_PIN_SET) & (voltage > 0.5 )){
//			HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
//			HAL_Delay(2500);
//			HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);
//			r2d = GPIO_PIN_SET;
//			counter ++;
//		}
//
//	}
//	if (counter != 0){
//		if (ts == GPIO_PIN_RESET){
//			counter = 0 ;
////			r2d = GPIO_PIN_RESET;
//		}
//	}
//
//}
void apps(void){
	 diff = fabs(tps1 - tps2);
    tps1 = ((float)tps1_raw * 3.3f) / 4095.0f;
	       tps2=((float)tps2_raw*3.3f)/4095.0f;

           percent1=(tps1/3.3)*100;
           percent2=(tps2/2.5)*100;

	 diff=fabsf(percent1-percent2);

	  if ((tps1 == tps2) || diff>10 || tps1 ==0.0|| tps2 ==0.0) {
			if ((current_millis - previous_millis) >= 100) {
			if(r2d == GPIO_PIN_RESET || r2d==GPIO_PIN_SET){
			HAL_GPIO_WritePin(GPIOB,MC_ENABLE, GPIO_PIN_RESET);
			previous_millis = current_millis;
		//    		            		HAL_Delay(500);
			}
		}
			}
			else {
				if ((r2d == GPIO_PIN_SET)){
					HAL_GPIO_WritePin(GPIOB, MC_ENABLE, GPIO_PIN_SET);
					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);

					}
			}

}
//void apps(void){
//	 diff = fabs(tps1 - tps2);
//	 current_millis = HAL_GetTick();
//
//		if (counter>= 0){
//			if ((tps1 == tps2) || diff >= 0.2f || tps1 ==0.0|| tps2 ==0.0) {
//						if ((current_millis - previous_millis) >= 100) {
//								if(r2d == GPIO_PIN_RESET || r2d==GPIO_PIN_SET){
//									HAL_GPIO_WritePin(GPIOB,MC_ENABLE, GPIO_PIN_RESET);
//										previous_millis = current_millis;
//		//    		            		HAL_Delay(500);
//										counter = 0 ;
//								}
//						}
//			}
//			else {
//				if ((r2d == GPIO_PIN_SET)){
//					HAL_GPIO_WritePin(GPIOB, MC_ENABLE, GPIO_PIN_SET);
//					HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
//					HAL_Delay(500);
//					counter++;
//				}
//			}
//		}
//}
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
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
//  R2D_status();
  //  HAL_GPIO_WritePin(GPIOB,MC_ENABLE, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, MC_ENABLE, GPIO_PIN_RESET);
//  HAL_GPIO_WritePin(GPIOB, R2D_OUT, GPIO_PIN_RESET);
  //HAL_GPIO_WritePin(GPIOA, TSAL_RED, GPIO_PIN_RESET);
  //HAL_GPIO_WritePin(GPIOB, GSS, GPIO_PIN_RESET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /* USER CODE END WHILE */
	  		  //	  HAL_ADC_Start(&hadc1);
	  current_millis = HAL_GetTick();
		sprintf(msg1,"millis %ld", current_millis);
			  		        HAL_UART_Transmit(&huart2,(uint8_t*)msg1,strlen(msg1),HAL_MAX_DELAY);


			  		             HAL_ADC_Start(&hadc1);
			  		        	 HAL_ADC_PollForConversion(&hadc1,HAL_MAX_DELAY);
			  		    	 	 tps1_raw = HAL_ADC_GetValue(&hadc1);
			  		      	     HAL_ADC_Stop(&hadc1);

			  		      	     HAL_ADC_Start(&hadc2);
			  		             HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
			  		      	     tps2_raw = HAL_ADC_GetValue(&hadc2);
			  		      	     HAL_ADC_Stop(&hadc2);
			  		      	 	 tps1 = ((float)tps1_raw * 3.3f) / 4095.0f;
			  		      	 	 tps2=((float)tps2_raw*3.3f)/4095.0f;

			  		      	 	 percent1=(tps1/3.3)*100;
			  		      	 	 percent2=(tps2/2.5)*100;


			  		      		      apps();
			  		      		      r2d = HAL_GPIO_ReadPin(GPIOB, R2D_OUT);
			  		      		      mc = HAL_GPIO_ReadPin(GPIOB, MC_ENABLE);

			  		      		      sprintf(msg,"RAW: %d, %d\r\n", tps_values[0], tps_values[1]);
			  		      		      HAL_UART_Transmit(&huart2,(uint8_t*)msg,strlen(msg),HAL_MAX_DELAY);
			  		      	          sprintf(msg1,"TPS1= %.2f V,TPS2= %.2f V ,MC_Enable: %ld, R2D: %ld\r\n", tps1,tps2,mc,r2d);
			  		      	   	      HAL_UART_Transmit(&huart2,(uint8_t*)msg1,strlen(msg1),HAL_MAX_DELAY);


	  		  	  //	  // Wait for both conversions
	  		  	  //	  for (int i = 0; i < 2; i++) {
	  		  	  //	      if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
	  		  	  //	          adc_buffer[i] = HAL_ADC_GetValue(&hadc1);
	  		  	  //	      }
	  		  	  //	  }
	  		  	  //
	  		  	  //	  HAL_ADC_Stop(&hadc1);
	  		  	  //
	  		  	  //	  tps1_raw = adc_buffer[0]; // First channel
	  		  	  //	  tps2_raw = adc_buffer[1]; // Second channel
	  		  	  //	  HAL_ADC_Start(&hadc2);  // Start ADC2 (independent)  // Dual mode
	  		  	  ////
	  		  	  ////	  HAL_ADCEx_MultiModeStart_DMA(&hadc1,(uint32_t*)tps_values, 2);
	  		  	  //	  HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)adc_buf, 2);
	  		  	  //	  uint16_t tps1_raw = adc_buf[0] & 0xFFFF;         // ADC1 result
	  		  	  //	  uint16_t tps2_raw = (adc_buf[0] >> 16) & 0xFFFF; // ADC2 result

	  		  	  	  // Convert to voltage
	  		  	  //	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
	  		  	  //HAL_Delay(500);
//	  		  	  HAL_ADC_Start(&hadc1);
//	  		  	  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
//	  		  	  tps1_raw = HAL_ADC_GetValue(&hadc1);
//
//	  		  	  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
//	  		  	  tps2_raw = HAL_ADC_GetValue(&hadc1);
//	  		   	  HAL_ADC_Stop(&hadc1);
//	  		        tps1 = (tps1_raw / 4096.0f) * 3.3f;
//	  		        tps2 = (tps2_raw / 4096.0f) * 3.3f;

	  		        //	      tps1 = (float)tps1_raw / 4095.0f * 3.3f;
	  		        //	      tps2 = (float)tps2_raw / 4095.0f * 3.3f; // temporarily set equal
	  		        //	      voltage = (bps_raw / 4095.0f) * 3.3f;
	  		        //	      voltage -= 0.5f;
	  		        //	      pressure = (voltage / 0.825f) * 100.0f;
	  		        //BPS
//	  		          HAL_ADC_Start(&hadc2);
//	  		  	      HAL_ADC_PollForConversion(&hadc2,HAL_MAX_DELAY);
//	  		  	      bps_raw = HAL_ADC_GetValue(&hadc2);
//	  		  	      HAL_ADC_Stop(&hadc2);
//	  		  	      //voltage = ADC_to_Voltage(adc_value);
//	  		  	      voltage = (bps_raw / 4096.0f) * 3.3f;
//	  		  	      voltage-=0.5;
//	  		  	      pressure = (voltage / (0.825f)) * 100.0f;
//	  		  	      sprintf(msg, "ADC_Value = %d, V: %.2f V, P: %.2f bar \r\n", bps_raw, voltage, pressure);
//	  		  	      HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
//
//	  		          ts = HAL_GPIO_ReadPin(GPIOB, TS_ACTIVE);
//	  		          start = HAL_GPIO_ReadPin(GPIOB, START_BUTTON);
//	  		          //tsal_red = HAL_GPIO_ReadPin(GPIOA, TSAL_RED);
//
//	  		        	     //  R2D logic
//	  		        	      R2D_status();
	  		        	    //safe_state();

	  	    /* USER CODE BEGIN 3 */

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
#ifdef USE_FULL_ASSERT
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
