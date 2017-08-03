//#include "ocean_sample.h"
#include "los_bsp_led.h"
#include "los_task.h"
#include "cmsis_os.h"
#include "max7219_driver_func.h"
#include "neul_bc95.h"
#include "los_dev_st_uart.h"

#include "ocean_interface.h"
#include "spi_bme280.h"

#define USE_NB_BC95 1
//#define USE_AGENTTINY 1

int get_cmd = 0;
int uset_value = 0;
int scroll = 0;

uint8_t relay = 0x00;

unsigned char g_local_ip[4] = {192, 168, 206, 41};
unsigned char g_local_gw[4] = {192, 168, 206, 1};
unsigned char g_local_msk[4] = {255, 255, 255, 0};
unsigned char g_macaddr[6] = {0x54,0x55,0x58,0x10,0x00,0x24};
unsigned char g_devsn[16] = "test00000001";
    

char max7219_nbget_show_data(int idx)
{
	if (idx < 0  || idx > 38)
	{
		return '0';
	}
	if (0<=idx && 9>= idx)
	{
		return 0x30+idx;
	}
	if (10<=idx || 35>=idx)
	{
		return 0x41 + idx - 10;
	}
	return '0';
}

void ocean_report_cmd_status(int dataflag, int result)
{
    unsigned char data[5] = {0};
    data[0] = 0x45;
    data[1] = 0x4E;
    data[2] = 0x56;//data[0`2] header
    data[3] = 0x03;// data report 

    data[4] = dataflag; //set char index cmd 

    ocean_send_data(data, 5);
}
//deal with the device control command, 
//this func will be called in other files while use agenttiny
int ocean_dev_control(unsigned char *rcvbuf, unsigned short rcvlen)
{
    unsigned short nLen = rcvlen;
    unsigned char *pucVal = rcvbuf;
    if (4 == nLen && pucVal[0] == 0xaa && pucVal[1] == 0xaa && pucVal[2] == 0x00 && pucVal[3] == 0x00)
    {
        //iot platform returned data , should discard.
        return 0;
    }
    if (5 == nLen && pucVal[0] == 0x45 && pucVal[1] == 0x4E && pucVal[2] == 0x56)
    {
        if (pucVal[3] == 0x03) 
        {
            //cmd comes
            switch(pucVal[4])
            {
                case 0x00:  //set relay off 
                    relay = 0x00;
                    LED2_OFF();
                    ocean_report_cmd_status(0, 0);
                    break;
                case 0x01:  //set relay on
                    relay = 0x01;
                    LED2_ON();
                    ocean_report_cmd_status(1, 0);
                    break;
                default:
                    break;
            }
            //need send cmd status
        }
        else
        {
            //not correct cmd
            return 0;
        }
    }
    else
    {
        return 0;
    }
    return 0;
}

static unsigned char test_rbuf[512];
void oceancon_sampletask(void * pvParameters)
{
	unsigned char data[11] = {0};
#ifdef USE_AGENTTINY  
    unsigned char iotaddress[4] = {112,93,129,154};
    char *iotport = "5683";
#endif
	//int leds;
	//int testd = 0;
	//char showd;
    int ret = -1;
    int n = 0;
    int rmsgcnt = 0;
#ifdef USE_AGENTTINY    
    ret = ocean_init_iot_addr(iotaddress, (unsigned char *)iotport);
    if (ret != 0)
    {
        while(1);
    }
#endif
    ret = -1;
    while(ret != 0)
    {
#ifdef USE_AGENTTINY
        ret = ocean_res_init(CON_AGENT, TRAN_ETH, NB_MAX);
#endif
#ifdef USE_NB_BC95
        ret = ocean_res_init(CON_NB, TRAN_MAX, NB_NEUL_BC95);
#endif
    }
    
    
    los_bus_spi_init(LOS_STM32L476_SPI1);
    bme280Init();
    
    struct bme280_dev dev;
    struct bme280_data comp_data;
    
    /* Sensor_0 interface over SPI with native chip select line */
    dev.dev_id          = 0;
    dev.intf            = BME280_SPI_INTF;
    dev.read            = user_spi_read;
    dev.write           = user_spi_write;
    dev.delay_ms        = osDelay;

    bme280_init(&dev);
   
    uint8_t settings_sel;
    
    /* Recommended mode of operation: Indoor navigation */
    dev.settings.osr_h = BME280_OVERSAMPLING_1X;
    dev.settings.osr_p = BME280_OVERSAMPLING_16X;
    dev.settings.osr_t = BME280_OVERSAMPLING_2X;
    dev.settings.filter = BME280_FILTER_COEFF_16;
    dev.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;


    settings_sel = BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    settings_sel |= BME280_OSR_HUM_SEL;
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;
        
    bme280_set_sensor_settings(settings_sel, &dev);
    bme280_set_sensor_mode(BME280_NORMAL_MODE, &dev);
    
    
    uint16_t temperature = 0;
    uint16_t humidity = 0;
    uint16_t pressure = 0;
    
    while(1)
    {
        if(bme280_get_sensor_data(BME280_ALL, &comp_data, &dev) == BME280_OK)
        {
            temperature = comp_data.temperature / 100;
            humidity = comp_data.humidity / 1024;
            pressure = comp_data.pressure / 10000;
        }
        /* get sensor data start { */
		data[0] = 0x45;
		data[1] = 0x4E;
		data[2] = 0x56;//data[0`2] header <ENV>
		data[3] = 0x0d;// data report 
		data[4] = temperature >> 8; //temperature
		data[5] = temperature;      //temperature
        data[6] = humidity >> 8;    //humidity
		data[7] = humidity;         //humidity
        data[8] = pressure >> 8;    //pressure
		data[9] = pressure;         //pressure
        data[10]= relay;            //relay status

        //showd = max7219_get_show_data(data[4]);
        //LOS_TaskLock();
        //LOS_MAX7219_show(showd);
        //LOS_TaskUnlock();
        /* get sensor data end } */
        
        //call interface to send data to iot platform
        ocean_send_data(data, 11);
        
        rmsgcnt = ocean_get_unread_pkg();
        while(rmsgcnt > 0)
        {
            n = ocean_recv_data(test_rbuf, 512);
            if (n > 0)
            {
                ocean_dev_control(test_rbuf, n);
                rmsgcnt--;
            }
        }
        osDelay(60000);
    }

}

void los_oceancon_sample(void)
{
	osThreadDef_t thread_def;
	
	thread_def.name = "OceanSample";
	thread_def.stacksize = 2048;
	thread_def.tpriority = osPriorityLow;
	thread_def.pthread = (os_pthread)oceancon_sampletask;
	osThreadCreate(&thread_def, NULL);
}
