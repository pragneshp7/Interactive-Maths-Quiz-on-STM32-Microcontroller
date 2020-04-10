/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  *
  * @note    modified by ARM
  *          The modifications allow to use this file as User Code Template
  *          within the Device Family Pack.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017-2018 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif
#ifdef RTE_CMSIS_RTOS2                  // when RTE component CMSIS RTOS2 is used
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#endif


#ifdef RTE_CMSIS_RTOS2_RTX5
/**
  * Override default HAL_GetTick function
  */
uint32_t HAL_GetTick (void) {
  static uint32_t ticks = 0U;
         uint32_t i;

  if (osKernelGetState () == osKernelRunning) {
    return ((uint32_t)osKernelGetTickCount ());
  }

  /* If Kernel is not running wait approximately 1 ms then increment 
     and return auxiliary tick counter value */
  for (i = (SystemCoreClock >> 14U); i > 0U; i--) {
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
  }
  return ++ticks;
}
#endif

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MemoryBus_Config(void);

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  SystemCoreClockUpdate();

  /* Add your application code here
     */
	
	//Initialise GPIO
	MX_GPIO_Init();
	MemoryBus_Config();

#ifdef RTE_CMSIS_RTOS2
  /* Initialize CMSIS-RTOS2 */
  osKernelInitialize ();

  /* Create application main thread */
  osThreadNew(app_main, NULL, &app_main_attr);

  /* Start thread execution */
  osKernelStart();
#endif

  /* Infinite loop */
  while (1)
  {
  }
}

static void MemoryBus_Config (void) {
  FSMC_NAND_PCC_TimingTypeDef ComSpaceTiming;
  FSMC_NAND_PCC_TimingTypeDef AttSpaceTiming;
  FSMC_NORSRAM_TimingTypeDef  NorTiming;
  FSMC_NORSRAM_InitTypeDef    NorInit;
  FSMC_NAND_InitTypeDef       NandInit;

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOE_CLK_ENABLE();
  __GPIOD_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOG_CLK_ENABLE();
  __GPIOH_CLK_ENABLE();
  __GPIOF_CLK_ENABLE();

  /* Peripheral clock enable */
  __FSMC_CLK_ENABLE();
  
  /* FSMC GPIO Configuration */
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;

  GPIO_InitStruct.Pin = GPIO_PIN_7  | GPIO_PIN_6  | GPIO_PIN_0  | GPIO_PIN_5 |
                        GPIO_PIN_1  | GPIO_PIN_4  | GPIO_PIN_15 | GPIO_PIN_14 |
                        GPIO_PIN_13 | GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_10 |
                        GPIO_PIN_9  | GPIO_PIN_8;

  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_3  | GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_13 |
                        GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_11 | GPIO_PIN_14 |
                        GPIO_PIN_7  | GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_15;

  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
                        GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0;

  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_0  | GPIO_PIN_2 | GPIO_PIN_1  | GPIO_PIN_3  |
                        GPIO_PIN_4  | GPIO_PIN_5 | GPIO_PIN_13 | GPIO_PIN_12 |
                        GPIO_PIN_15 | GPIO_PIN_14;

  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /** Perform the NOR1 memory initialization sequence */
  NorInit.NSBank             = FSMC_NORSRAM_BANK1;
  NorInit.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
  NorInit.MemoryType         = FSMC_MEMORY_TYPE_NOR;
  NorInit.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  NorInit.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
  NorInit.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  NorInit.WrapMode           = FSMC_WRAP_MODE_DISABLE;
  NorInit.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
  NorInit.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
  NorInit.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
  NorInit.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
  NorInit.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  NorInit.WriteBurst         = FSMC_WRITE_BURST_DISABLE;
  
  /* Timing */
  NorTiming.AddressSetupTime = 6;
  NorTiming.AddressHoldTime = 15;
  NorTiming.DataSetupTime = 6;
  NorTiming.BusTurnAroundDuration = 15;
  NorTiming.CLKDivision = 2;
  NorTiming.DataLatency = 17;
  NorTiming.AccessMode = FSMC_ACCESS_MODE_B;

  /* Initialize NOR control Interface */
  FMC_NORSRAM_Init(FSMC_NORSRAM_DEVICE, &NorInit);

  /* Initialize NOR timing Interface */
  FMC_NORSRAM_Timing_Init(FSMC_NORSRAM_DEVICE, &NorTiming, NorInit.NSBank); 

  /* Initialize NOR extended mode timing Interface */
  FMC_NORSRAM_Extended_Timing_Init(FSMC_NORSRAM_EXTENDED_DEVICE, &NorTiming, NorInit.NSBank, NorInit.ExtendedMode);

  /* Enable the NORSRAM device */
  __FMC_NORSRAM_ENABLE(FSMC_NORSRAM_DEVICE, NorInit.NSBank);

  /** Perform the NAND2 memory initialization sequence */

  /* NAND Bank Init Structure */
  NandInit.NandBank        = FSMC_NAND_BANK3;
  NandInit.Waitfeature     = FSMC_NAND_PCC_WAIT_FEATURE_DISABLE;
  NandInit.MemoryDataWidth = FSMC_NAND_PCC_MEM_BUS_WIDTH_8;
  NandInit.EccComputation  = FSMC_NAND_ECC_DISABLE;
  NandInit.ECCPageSize     = FSMC_NAND_ECC_PAGE_SIZE_256BYTE;
  NandInit.TCLRSetupTime   = 1;
  NandInit.TARSetupTime    = 1;

  /* ComSpaceTiming */
  ComSpaceTiming.SetupTime     = 1;
  ComSpaceTiming.WaitSetupTime = 5;
  ComSpaceTiming.HoldSetupTime = 5;
  ComSpaceTiming.HiZSetupTime  = 2;
  /* AttSpaceTiming */
  AttSpaceTiming.SetupTime     = 1;
  AttSpaceTiming.WaitSetupTime = 5;
  AttSpaceTiming.HoldSetupTime = 5;
  AttSpaceTiming.HiZSetupTime  = 2;

  /* Initialize NAND control Interface */
  FMC_NAND_Init(FSMC_NAND_DEVICE, &(NandInit));
  
  /* Initialize NAND common space timing Interface */  
  FMC_NAND_CommonSpace_Timing_Init(FSMC_NAND_DEVICE, &ComSpaceTiming, NandInit.NandBank);
  
  /* Initialize NAND attribute space timing Interface */  
  FMC_NAND_AttributeSpace_Timing_Init(FSMC_NAND_DEVICE, &AttSpaceTiming, NandInit.NandBank);
  
  /* Enable the NAND device */
  __FMC_NAND_ENABLE(FSMC_NAND_DEVICE, NandInit.NandBank);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
