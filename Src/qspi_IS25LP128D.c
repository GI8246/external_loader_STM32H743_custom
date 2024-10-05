#include "main.h"
//#include "cmsis_os.h"
#include "stdio.h"

static QSPI_HandleTypeDef  hqspi;
static QSPI_CommandTypeDef cmd;

static uint32_t qspi_mode = 0;

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* hqspi)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  if(hqspi->Instance==QUADSPI)
  {
#if 0
    extern RCC_PLL2InitTypeDef PLL2_conf;

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
    PeriphClkInitStruct.PLL2                 = PLL2_conf;
    PeriphClkInitStruct.QspiClockSelection   = RCC_QSPICLKSOURCE_PLL2;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
#endif

    /* Peripheral clock enable */
    __HAL_RCC_QSPI_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    /**QUADSPI GPIO Configuration
    PB6     ------> QUADSPI_BK1_NCS
    PF7     ------> QUADSPI_BK1_IO2
    PF6     ------> QUADSPI_BK1_IO3
    PF9     ------> QUADSPI_BK1_IO1
    PF8     ------> QUADSPI_BK1_IO0
    PB2     ------> QUADSPI_CLK
    */

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

#if 0
    HAL_NVIC_SetPriority(QUADSPI_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(QUADSPI_IRQn);
#endif
  }
}

void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* hqspi)
{
  if(hqspi->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

  /* USER CODE END QUADSPI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();

    /**QUADSPI GPIO Configuration
    PB6     ------> QUADSPI_BK1_NCS
    PF7     ------> QUADSPI_BK1_IO2
    PF6     ------> QUADSPI_BK1_IO3
    PF9     ------> QUADSPI_BK1_IO1
    PF8     ------> QUADSPI_BK1_IO0
    PB2     ------> QUADSPI_CLK
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_2);

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_9|GPIO_PIN_8);

  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

  /* USER CODE END QUADSPI_MspDeInit 1 */
  }

}

static void write_enable()
{
  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = 0x06; // write enable
  cmd.AddressMode     = QSPI_ADDRESS_NONE;
  cmd.DataMode        = QSPI_DATA_NONE;

  if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    printf("qspi write enable fail\r\n");
}

static void write_disable()
{
  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = 0x04; // write enable
  cmd.AddressMode     = QSPI_ADDRESS_NONE;
  cmd.DataMode        = QSPI_DATA_NONE;

  if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    printf("qspi write disable fail\r\n");
}

static uint8_t read_status_register()
{
  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = 0x05;
  cmd.AddressMode     = QSPI_ADDRESS_NONE;
  cmd.DataMode        = qspi_mode ? QSPI_DATA_4_LINES : QSPI_DATA_1_LINE;
  cmd.NbData          = 1;

  if (HAL_OK != HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE))
  {
    printf("fail: qspi read status register: HAL_QSPI_Command\r\n");
    Error_Handler();
  }

  uint8_t data;
  if (HAL_OK != HAL_QSPI_Receive(&hqspi, &data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE))
  {
    printf("fail: qspi read status register: HAL_QSPI_Receive\r\n");
    Error_Handler();
  }

  return data;
}

static uint8_t write_status_register(uint8_t data)
{
  write_enable();

  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = 0x01;
  cmd.AddressMode     = QSPI_ADDRESS_NONE;
  cmd.DataMode        = qspi_mode ? QSPI_DATA_4_LINES : QSPI_DATA_1_LINE;
  cmd.NbData          = 1;

  if (   HAL_OK != HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
      || HAL_OK != HAL_QSPI_Transmit(&hqspi, &data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) )
  {
    printf("fail: qspi write register\r\n");
    Error_Handler();
  }

  return data;
}

static uint8_t read_function_register()
{
  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = 0x48;
  cmd.AddressMode     = QSPI_ADDRESS_NONE;
  cmd.DataMode        = qspi_mode ? QSPI_DATA_4_LINES : QSPI_DATA_1_LINE;
  cmd.NbData          = 1;

  uint8_t data;

  if (   HAL_OK != HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
      || HAL_OK != HAL_QSPI_Receive(&hqspi, &data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) )
  {
    printf("fail: qspi read function register\r\n");
    Error_Handler();
  }

  return data;
}

static uint8_t read_bank_address_register()
{
  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = 0x16;
  cmd.AddressMode     = QSPI_ADDRESS_NONE;
  cmd.DataMode        = qspi_mode ? QSPI_DATA_4_LINES : QSPI_DATA_1_LINE;
  cmd.NbData          = 1;

  uint8_t data;

  if (   HAL_OK != HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
      || HAL_OK != HAL_QSPI_Receive(&hqspi, &data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) )
  {
    printf("fail: qspi read function register\r\n");
    Error_Handler();
  }

  return data;
}

static uint8_t write_bank_address_register(uint8_t data)
{
  write_enable();

  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = 0x18;
  cmd.AddressMode     = QSPI_ADDRESS_NONE;
  cmd.DataMode        = qspi_mode ? QSPI_DATA_4_LINES : QSPI_DATA_1_LINE;
  cmd.NbData          = 1;

  if (   HAL_OK != HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
      || HAL_OK != HAL_QSPI_Transmit(&hqspi, &data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) )
  {
    printf("fail: qspi write register\r\n");
    Error_Handler();
  }

  return data;
}

static void check_WIP()
{
  while (1)
  {
    if (read_status_register() & 0x01)
      HAL_Delay(2);
    else
      break;
  }
}

uint32_t IS25LP128D_erase_sector(uint32_t address)
{
  if (address > 0x00FFFFFF)
  {
    printf("address > 0x01FFFFFF !!!!");
    return 1;
  }

  if (HAL_QSPI_Abort(&hqspi) != HAL_OK)
      return 1;

  write_enable();

  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = 0xD7;
  cmd.AddressMode     = qspi_mode ? QSPI_ADDRESS_4_LINES : QSPI_ADDRESS_1_LINE;
  cmd.Address         = address;
  cmd.DataMode        = QSPI_DATA_NONE;

  if ( HAL_OK != HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE))
  {
    printf("fail: qspi erase sector \r\n");
    return 1;
  }

  check_WIP();

  return 0;
}

uint32_t IS25LP128D_program_page(uint32_t address, uint8_t *data, uint32_t length)
{
  if (address > 0x00FFFF00 && length > 0xFF)
  {
    printf("address or length error\r\n");
    return 1;
  }

  if (HAL_QSPI_Abort(&hqspi) != HAL_OK)
      return 1;

  write_enable();

  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = 0x02;
  cmd.AddressMode     = qspi_mode ? QSPI_ADDRESS_4_LINES : QSPI_ADDRESS_1_LINE;
  cmd.Address         = address;
  cmd.DataMode        = qspi_mode ? QSPI_DATA_4_LINES : QSPI_DATA_1_LINE;
  cmd.NbData          = length;

  if (   HAL_OK != HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
      || HAL_OK != HAL_QSPI_Transmit(&hqspi, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) )
  {
    printf("fail: qspi erase sector \r\n");
    return 1;
  }

  check_WIP();

  return 0;
}

uint32_t IS25LP128D_read(uint32_t address, uint8_t *data, uint32_t length)
{
  if (address > 0x00FFFFFF || data == NULL)
  {
    printf("fail: qspi read\r\n");
    return 1;
  }

  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = qspi_mode ? 0xEB : 0x03;
  cmd.AddressMode     = qspi_mode ? QSPI_ADDRESS_4_LINES : QSPI_ADDRESS_1_LINE;
  cmd.Address         = address;
  cmd.DummyCycles     = 6;
  cmd.DataMode        = qspi_mode ? QSPI_DATA_4_LINES : QSPI_DATA_1_LINE;
  cmd.NbData          = length;

  if (   HAL_OK != HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
      || HAL_OK != HAL_QSPI_Receive(&hqspi, data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) )
  {
    printf("fail: qspi read\r\n");
  }

  cmd.DummyCycles = 6;

  return 0;
}

static void reset()
{
  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.AddressMode     = QSPI_ADDRESS_NONE;
  cmd.DataMode        = QSPI_DATA_NONE;

  cmd.Instruction = 0x66; // reset enable
  if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    printf("fail: qspi reset enable\r\n");

  cmd.Instruction = 0x99; // reset
  if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    printf("fail: qspi reset\r\n");
}

static uint32_t read_JEDEC_ID()
{
  cmd.InstructionMode = qspi_mode ? QSPI_INSTRUCTION_4_LINES : QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = qspi_mode ? 0xAF : 0x9F;
  cmd.AddressMode     = QSPI_ADDRESS_NONE;
  cmd.DataMode        = qspi_mode ? QSPI_DATA_4_LINES : QSPI_DATA_1_LINE;
  cmd.NbData          = 3;

  uint32_t ID = 0;
  if (   HAL_OK != HAL_QSPI_Command(&hqspi, &cmd,               HAL_QPSI_TIMEOUT_DEFAULT_VALUE)
      || HAL_OK != HAL_QSPI_Receive(&hqspi, (uint8_t *)&ID, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) )
  {
    printf("fail: qspi read JEDEC ID\r\n");
    Error_Handler();
  }

  return ID;
}

static uint32_t enter_qspi_mode()
{
  if (qspi_mode)
  {
    printf("already in qspi mode\r\n");
    return 1;
  }

  cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
  cmd.Instruction     = 0x35;
  cmd.AddressMode     = QSPI_ADDRESS_NONE;
  cmd.DataMode        = QSPI_DATA_NONE;

  if (HAL_OK != HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE))
  {
    printf("fail: enter_qspi_mode\r\n");
    return 1;
  }

  qspi_mode = 1;

  return 0;
}

static void qspi_task(void *argument)
{
  printf("starting qspi task\r\n");

  for(;;)
  {
    //osDelay(1000);
  }
}

void MX_QSPI_Init(void)
{
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;

  hqspi.Init.ClockPrescaler     = 3;
  hqspi.Init.FifoThreshold      = 16;
  hqspi.Init.SampleShifting     = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize          = 24; // 16 MBytes
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode          = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID            = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash          = QSPI_DUALFLASH_DISABLE;

  // peripheral init
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }

  // command init
  cmd.AddressSize       = QSPI_ADDRESS_24_BITS;
  cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  cmd.DummyCycles       = 0;
  cmd.DdrMode           = QSPI_DDR_MODE_DISABLE;
  cmd.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  cmd.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  // memory init
  reset();
  printf("JEDEC ID in SPI mode  = 0x%06X\r\n", (unsigned int)read_JEDEC_ID());
  enter_qspi_mode();
  printf("JEDEC ID in QSPI mode = 0x%06X\r\n", (unsigned int)read_JEDEC_ID());

  printf("IS25LP256D status register value = 0x%02X\r\n", read_status_register());
  printf("IS25LP256D function register value = 0x%02X\r\n", read_function_register());
  printf("IS25LP256D bank address register value = 0x%02X\r\n", read_bank_address_register());

#if 0
  uint8_t in[] = "1234567890\r\n", out[32] = {0, };
  IS25LP128D_erase_sector(0x00FFF000);
  IS25LP128D_program_page(0x00FFFF00, in, 13);
  IS25LP128D_read(0x00FFFF00, out, 13);
  printf("qspi input  = %s\r\n", in);
  printf("qspi output = %s\r\n", out);
#else
  uint8_t buf[32] = {0, };
  IS25LP128D_read(0x00FFFF00, buf, 13);
#endif

  QSPI_MemoryMappedTypeDef cfg;
  cfg.TimeOutPeriod     = 0;
  cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
  if (HAL_OK != HAL_QSPI_MemoryMapped(&hqspi, &cmd, &cfg))
    printf("QSPI init err\r\n");

  // regieter thread
  //qspi_task_handle = osThreadNew(qspi_task, NULL, &qspiTask_attributes);
}
