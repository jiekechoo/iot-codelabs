#include "spi.h"

void drv_spi_init(void)  
{  
    SPI_InitTypeDef  SPI_InitStructure;  
    GPIO_InitTypeDef GPIO_InitStructure;  

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);  

    /* SCK */  
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_5;  
    GPIO_InitStructure.GPIO_Speed       = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_AF_PP;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    /* MOS */  
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_7;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    /* MISO */  
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_6;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

    /* CS  */  
    GPIO_InitStructure.GPIO_Pin         = GPIO_Pin_0;  
    GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_Out_PP;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
    SPI_SET_NSS_HIGH; 

    SPI_InitStructure.SPI_Direction             = SPI_Direction_2Lines_FullDuplex;  
    SPI_InitStructure.SPI_Mode                  = SPI_Mode_Master;  
    SPI_InitStructure.SPI_DataSize              = SPI_DataSize_8b;  
    SPI_InitStructure.SPI_CPOL                  = SPI_CPOL_High;  
    SPI_InitStructure.SPI_CPHA                  = SPI_CPHA_2Edge;  
    SPI_InitStructure.SPI_NSS                   = SPI_NSS_Soft;  
    SPI_InitStructure.SPI_BaudRatePrescaler     = SPI_BaudRatePrescaler_256;  
    SPI_InitStructure.SPI_FirstBit              = SPI_FirstBit_MSB;  
    SPI_InitStructure.SPI_CRCPolynomial         = 7;  
    SPI_Init(SPI1, &SPI_InitStructure);  

    SPI_Cmd(SPI1, ENABLE);  
    
    SPIx_ReadWriteByte(0xFF);
}  

uint8_t aa = 0;

int8_t user_spi_read (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    SPI_SET_NSS_LOW;
    
    SPIx_ReadWriteByte(reg_addr | 0x80);        //发送地址	
  
    uint16_t i;
    for(i = 0; i < len; i++)
    {
        data[i] = SPIx_ReadWriteByte(0xFF);     //读取数据
    }
    
    SPI_SET_NSS_HIGH;
  return BME280_OK;
}

int8_t user_spi_write (uint8_t dev_id, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    SPI_SET_NSS_LOW;

    SPIx_ReadWriteByte(reg_addr & ~0x80);      //发送地址	

    uint16_t i;
    for(i = 0; i < len; i++)
    {
        SPIx_ReadWriteByte(data[i]);            //发送数据
    }
    
    SPI_SET_NSS_HIGH;

  return BME280_OK;
}

uint8_t SPIx_ReadWriteByte(uint8_t TxData)
{
    uint8_t retry=0;				 	
    while (SPI_I2S_GetFlagStatus(SPI_PORT, SPI_I2S_FLAG_TXE) == RESET)             //检查指定的SPI标志位设置与否:发送缓存空标志位
        {
        retry++;
        if(retry>200) return 0;
        }			  

    SPI_I2S_SendData(SPI_PORT, TxData);                                             //通过外设SPIx发送一个数据
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET)            //检查指定的SPI标志位设置与否:接受缓存非空标志位
        {
        retry++;
        if(retry>200) return 0;
        }	  						    
    return SPI_I2S_ReceiveData(SPI_PORT);                                           //返回通过SPIx最近接收的数据					    
}