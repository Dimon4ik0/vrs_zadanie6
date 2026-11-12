/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "hts221.h"
#include "lps25hb.h"


// I2C slave device useful information
#define 	LSM6DS0_DEVICE_ADDRESS		0xD6U
#define 	LSM6DS0_WHO_AM_I_VALUE		0x68U
#define 	LSM6DS0_WHO_AM_I_ADDRES		0x0FU


void SystemClock_Config(void);


uint8_t hts_who_am_i = 0;
uint8_t lps_who_am_i = 0;
uint8_t hts_calib_bytes[4] = {0};
uint8_t ctrl_readback = 0;

int32_t test1_hts_id_status = -1;
int32_t test2_lps_id_status = -1;
int32_t test3_multibyte_status = -1;
int32_t test4_write_status = -1;
int32_t test5_readback_status = -1;


int main(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();


  //hts221_init(I2C1_Read, I2C1_Write);
  //lps25hb_init(I2C1_Read, I2C1_Write);

  // --- TEST 1: Single-byte read WHO_AM_I (HTS221 at address 0x5F) ---
    // Expected ID: 0xBC (188 decimal)
    test1_hts_id_status = I2C1_Read(0x5F, 0x0F, &hts_who_am_i, 1);

    // --- TEST 2: Single-byte read WHO_AM_I (LPS25HB at address 0x5D) ---
    // Expected ID: 0xBD (189 decimal)
    test2_lps_id_status = I2C1_Read(0x5D, 0x0F, &lps_who_am_i, 1);

    // --- TEST 3: Multi-byte read (4 calibration bytes from HTS221) ---
    // Bit 7 (0x80) enables automatic register address increment
    test3_multibyte_status = I2C1_Read(0x5F, 0x30 | 0x80, hts_calib_bytes, 4);

    // --- TEST 4 & 5: Single-byte Write & Verification (LPS25HB CTRL_REG1) ---
    uint8_t ctrl1_config = 0x90; // Active mode (1Hz ODR)
    test4_write_status = I2C1_Write(0x5D, 0x20, &ctrl1_config, 1);
    test5_readback_status = I2C1_Read(0x5D, 0x20, &ctrl_readback, 1);


  while (1)
  {
	  LL_mDelay(500);
  }
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
