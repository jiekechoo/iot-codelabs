#include "los_bsp_led.h"

#ifdef LOS_STM32L476VE


#endif

void LOS_EvbLedInit(void)
{

#ifdef LOS_STM32L476VE

#endif
    
     GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable the GPIO_LED Clock */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure the GPIO_LED pin */
    GPIO_InitStruct.Pin     = LED2_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(LED2_PORT, &GPIO_InitStruct);

}

/*************************************************************************************************
 *  function£ºcontrol led on off                                                                 *
 *  param (1) index Led's index                                                                  *
 *        (2) cmd   Led on or off                                                                *
 *  return : None                                                                                *
 *  discription:                                                                                 *
**************************************************************************************************/
void LOS_EvbLedControl(int index, int cmd)
{
#ifdef LOS_STM32L476VE

#endif
    return;
}

void LED2_ON()
{
    HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);
}    
void LED2_OFF()
{
    HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
}    

