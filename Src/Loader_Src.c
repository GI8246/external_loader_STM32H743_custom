#include "main.h"

#define VERIFY 1

#define LOADER_OK   0x1
#define LOADER_FAIL 0x0

#define StartAddresse 0x90000000

extern void SystemClock_Config(void);


RCC_PLLInitTypeDef PLL1_conf = {
    .PLLState  = RCC_PLL_ON,
    .PLLSource = RCC_PLLSOURCE_HSE,

    .PLLM      = 5,
    .PLLN      = 160,

    .PLLP      = 2,
    .PLLQ      = 4,
    .PLLR      = 8,

    .PLLRGE    = RCC_PLL1VCIRANGE_2,
    .PLLVCOSEL = RCC_PLL1VCOWIDE,
    .PLLFRACN  = 0,
};

RCC_PLL2InitTypeDef PLL2_conf = {
    .PLL2M      = 5,
    .PLL2N      = 80,

    .PLL2P      = 2,
    .PLL2Q      = 2,
    .PLL2R      = 2,

    .PLL2RGE    = RCC_PLL2VCIRANGE_2,
    .PLL2VCOSEL = RCC_PLL2VCOWIDE,
    .PLL2FRACN  = 0,
};

RCC_PLL3InitTypeDef PLL3_conf = {
    .PLL3M      = 5,
    .PLL3N      = 60,

    .PLL3P      = 2,
    .PLL3Q      = 2,
    .PLL3R      = 33,

    .PLL3RGE    = RCC_PLL3VCIRANGE_2,
    .PLL3VCOSEL = RCC_PLL3VCOWIDE,
    .PLL3FRACN  = 0,
};

int __io_putchar(int ch)
{
    ITM_SendChar(ch);

    return ch;
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  return HAL_OK;
}

uint32_t HAL_GetTick(void)
{
  return 1;
}

void HAL_Delay(uint32_t Delay)
{
  int i = 0;

  for (i = 0; i < 0x400; i++);
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0, };
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0, };

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
  RCC_OscInitStruct.PLL            = PLL1_conf;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK   | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1  | RCC_CLOCKTYPE_PCLK2
                              | RCC_CLOCKTYPE_D3PCLK1| RCC_CLOCKTYPE_D1PCLK1;

  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;

  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  /*activate CSI clock mondatory for I/O Compensation Cell*/
  __HAL_RCC_CSI_ENABLE() ;

  /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE() ;

  /* Enables the I/O Compensation Cell */
  HAL_EnableCompensationCell();
}

void Error_Handler(void)
{
    while(1)
    {
        //HAL_GPIO_TogglePin(LED_STATUS_GPIO_Port, LED_STATUS_Pin);
        HAL_Delay(500);
    }
}

int Read(uint32_t Address, uint32_t Size, uint16_t* buffer)
{
  uint32_t ret = LOADER_OK;

  if (IS25LP128D_read(Address & 0x00FFFFFF, (uint8_t *)buffer, Size))
    ret = LOADER_FAIL;

  return ret;
}

int Write(uint32_t Address, uint32_t Size, uint8_t* buffer)
{
  uint32_t ret = LOADER_OK;

  uint8_t *ptr = buffer;
  uint32_t written = 0;
  uint32_t page_size = 0x100;

  //printf("addr = 0x%08X, size = %d\r\n", Address, Size);

  while (Size > written)
  {
    if (IS25LP128D_program_page(Address & 0x00FFFFFF, ptr, page_size))
    {
      printf("fail: Write: address = 0x%80X\r\n", Address);
      ret = LOADER_FAIL;
      break;
    }
    Address += page_size;
    ptr     += page_size;
    written += page_size;
  }

  return ret;
}

int SectorErase(uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
  uint32_t ret     = LOADER_OK;
  uint32_t address = EraseStartAddress;

  while (1)
  {
    if (IS25LP128D_erase_sector(address & 0x00FFFFFF))
    {
      ret = LOADER_FAIL;
      break;
    }

    address += 4096; // 4 kB sector size
    if (address > EraseEndAddress)
      break;
  }

  return ret;
}

#if 0
int MassErase(void)
{
  uint32_t ret = LOADER_OK;

  if (LOADER_FAIL == IS25LP128D_erase_chip())
    ret = LOADER_FAIL;

  return ret;
}
#endif

#if VERIFY
uint32_t CheckSum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal)
{
  uint8_t missalignementAddress = StartAddress % 4;
  uint8_t missalignementSize    = Size ;

  int      cnt;
  uint32_t Val;

  StartAddress -= StartAddress % 4;
  Size += (Size % 4 == 0) ? 0 : 4 - (Size % 4);

  for (cnt = 0; cnt < Size; cnt += 4)
  {
    IS25LP128D_read(StartAddress - StartAddresse, &Val, 4);

    if (missalignementAddress)
    {
      switch (missalignementAddress)
      {
      case 1:
        InitVal += (uint8_t) (Val >> 8 & 0xff);
        InitVal += (uint8_t) (Val >> 16 & 0xff);
        InitVal += (uint8_t) (Val >> 24 & 0xff);
        missalignementAddress -= 1;
        break;
      case 2:
        InitVal += (uint8_t) (Val >> 16 & 0xff);
        InitVal += (uint8_t) (Val >> 24 & 0xff);
        missalignementAddress -= 2;
        break;
      case 3:
        InitVal += (uint8_t) (Val >> 24 & 0xff);
        missalignementAddress -=3 ;
        break;
      }
    }
    else if ((Size-missalignementSize) % 4 && (Size-cnt) <= 4)
    {
      switch (Size-missalignementSize)
      {
      case 1:
        InitVal += (uint8_t)  Val;
        InitVal += (uint8_t) (Val >> 8  & 0xff);
        InitVal += (uint8_t) (Val >> 16 & 0xff);

        missalignementSize -= 1;

        break;
      case 2:
        InitVal += (uint8_t)  Val;
        InitVal += (uint8_t) (Val >> 8 & 0xff);

        missalignementSize -= 2;

        break;
      case 3:
        InitVal += (uint8_t) Val;

        missalignementSize -= 3;

        break;
      }
    }
    else
    {
      InitVal += (uint8_t) Val;
      InitVal += (uint8_t)(Val >> 8  & 0xff);
      InitVal += (uint8_t)(Val >> 16 & 0xff);
      InitVal += (uint8_t)(Val >> 24 & 0xff);
    }
    StartAddress += 4;
  }

  return (InitVal);
}

uint64_t Verify (uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement)
{
  uint32_t InitVal = 0;
  uint32_t InternalAddr = MemoryAddr - StartAddresse;
  uint32_t VerifiedData = 0;
  uint16_t TmpBuffer = 0x0000;
  uint64_t checksum;

  Size *= 4;

  checksum = CheckSum((uint32_t)MemoryAddr + (missalignement & 0xf),
                      Size - ((missalignement >> 16) & 0xF),
                      InitVal);

  if (InternalAddr % 2 != 0)
  {
    IS25LP128D_read((InternalAddr - InternalAddr % 2), &TmpBuffer, 2);

    if ((uint8_t)(TmpBuffer >> 8) != (*(uint8_t*)RAMBufferAddr))
      return ((checksum << 32) + MemoryAddr);

    VerifiedData++;
  }

  while ((Size - VerifiedData) > 1)
  {
    IS25LP128D_read(InternalAddr + VerifiedData, &TmpBuffer, 2);

    if ((TmpBuffer & 0x00FF) != (*((uint8_t*)RAMBufferAddr + VerifiedData)))
      return ((checksum << 32) + MemoryAddr + VerifiedData);

    VerifiedData++;

    if ((uint8_t)(TmpBuffer >> 8) != (*((uint8_t *)RAMBufferAddr+VerifiedData)))
      return ((checksum << 32) + MemoryAddr + VerifiedData);

    VerifiedData++;
  }

  if ((Size - VerifiedData) != 0)
  {
    IS25LP128D_read(InternalAddr + VerifiedData, &TmpBuffer, 2);

    if ((uint8_t)(TmpBuffer & 0x00FF) != (*((uint8_t*)RAMBufferAddr + VerifiedData)))
      return ((checksum << 32) + MemoryAddr + VerifiedData);
  }

  return (checksum << 32);
}
#endif


int Init(void)
{
  *(uint32_t*)0xE000EDF0 = 0xA05F0000; //enable interrupts in debug

  SystemInit();

  /* ADAPTATION TO THE DEVICE
   *
   * change VTOR setting for H7 device
   * SCB->VTOR = 0x24000000 | 0x200;
   *
   * change VTOR setting for other devices
   * SCB->VTOR = 0x20000000 | 0x200;
   *
   * */

  SCB->VTOR = 0x24000000 | 0x200;

  HAL_Init();

  SystemClock_Config();

  MX_USART1_Init();

  __HAL_RCC_QSPI_FORCE_RESET();  //completely reset peripheral
  __HAL_RCC_QSPI_RELEASE_RESET();

  MX_QSPI_Init();

  return LOADER_OK;
}

