#ifndef _SPI_BME280_H
#define _SPI_BME280_H

#include "los_dev_st_spi.h"
#include "bme280.h"


#define BME280_CS     GPIO_PIN_0
#define BME280_CSL()  HAL_GPIO_WritePin(GPIOB, BME280_CS, GPIO_PIN_RESET)//GPIOA->BRR = BME280_CS;
#define BME280_CSH()  HAL_GPIO_WritePin(GPIOB, BME280_CS, GPIO_PIN_SET)//GPIOA->BSRR = BME280_CS;

#define SPI1_ReadWrite(dat) los_bus_spi_rwonebyte(LOS_STM32L476_SPI1, dat, 50)
#define bme280CSinit() los_bus_spi_devcs_init(GPIOB, BME280_CS)


//SPI1≥ı ºªØ
int8_t user_spi_read (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);
int8_t user_spi_write (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len);


void 	bme280Init(void);

#endif /* _SPI_BME280_H */
