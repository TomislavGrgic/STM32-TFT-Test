/**
  ******************************************************************************
  * @file    system_stm32f0xx.h
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M0 Device System Source File for STM32F0xx devices.  
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f0xx_system
  * @{
  */  
  
/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_STM32F0XX_H
#define __SYSTEM_STM32F0XX_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup STM32F0xx_System_Includes
  * @{
  */

/**
  * @}
  */


/** @addtogroup STM32F0xx_System_Exported_types
  * @{
  */
  /* This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      3) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) by calling HAL API function HAL_RCC_ClockConfig()
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */
extern const uint8_t AHBPrescTable[16];   /*!< AHB prescalers table values */
extern const uint8_t APBPrescTable[8];    /*!< APB prescalers table values */

/**
  * @}
  */

/** @addtogroup STM32F0xx_System_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F0xx_System_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F0xx_System_Exported_Functions
  * @{
  */
  
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
extern void _init(void); 
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_STM32F0XX_H */

/**
  * @}
  */
  
/**
  * @}
  */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
