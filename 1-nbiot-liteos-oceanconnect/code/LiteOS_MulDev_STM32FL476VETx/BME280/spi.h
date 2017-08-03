#ifndef __SPI_H__
#define __SPI_H__

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_spi.h"
#include "stm32l4xx_hal_gpio.h"
#include "BME280.h"

#define         SPI_PORT                SPI1

#define         SPI_SET_NSS_LOW         GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define         SPI_SET_NSS_HIGH        GPIO_SetBits(GPIOB, GPIO_Pin_0)

void drv_spi_init(void);
int8_t user_spi_read (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t user_spi_write (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);

uint8_t SPIx_ReadWriteByte(uint8_t TxData);

#endif