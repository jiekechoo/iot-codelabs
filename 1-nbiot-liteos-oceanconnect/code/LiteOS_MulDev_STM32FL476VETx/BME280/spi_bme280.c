#include "spi_bme280.h"

int8_t user_spi_read (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    BME280_CSL();
    
    SPI1_ReadWrite(reg_addr | 0x80);        //发送地址	
  
    uint16_t i;
    for(i = 0; i < len; i++)
    {
        data[i] = SPI1_ReadWrite(0xFF);     //读取数据
    }
    
    BME280_CSH();
  return BME280_OK;
}

int8_t user_spi_write (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    BME280_CSL();

    SPI1_ReadWrite(reg_addr & ~0x80);      //发送地址	

    uint16_t i;
    for(i = 0; i < len; i++)
    {
        SPI1_ReadWrite(data[i]);            //发送数据
    }
    
    BME280_CSH();

  return BME280_OK;
}


void bme280Init(void)
{
    // initialize I/O
    bme280CSinit();
    BME280_CSH();

}
