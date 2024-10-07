/**
  ******************************************************************************
  * @file    Dev_Inf.c
  * @author  MCD Application Team
  * @brief   This file defines the structure containing informations about the
  *          external flash memory MT25TL01 used by STM32CubeProgramer in
  *          programming/erasing the device.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */


#include "Dev_Inf.h"

struct StorageInfo const StorageInfo  =  {
  "SMU_DISP_H743_IS25LP128D",  // Device Name + EVAL Board name
  NOR_FLASH,                   // Device Type
  0x90000000,                  // Device Start Address
  0x01000000,                  // Device Size in 16 MBytes
  0x100,                       // Programming Page Size 256 Bytes
  0xFF,                        // Initial Content of Erased Memory

  // Specify Size and Address of Sectors (view example below)
  {
    0x00001000, 0x00001000,   // Sector Num : 4096, Sector Size: 4 kBytes
  }
};

