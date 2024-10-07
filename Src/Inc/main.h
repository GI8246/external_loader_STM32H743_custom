#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

void Error_Handler(void);

#define LED_STATUS_Pin GPIO_PIN_13
#define LED_STATUS_GPIO_Port GPIOD

int Init(void);
void MX_USART1_Init(void);
void MX_QSPI_Init(void);

uint32_t IS25LP128D_erase_sector(uint32_t address);
uint32_t IS25LP128D_program_page(uint32_t address, uint8_t *data, uint32_t length);
uint32_t IS25LP128D_read(uint32_t address, uint8_t *data, uint32_t length);
uint32_t IS25LP128D_erase_chip();

#ifdef __cplusplus
}
#endif

#endif
