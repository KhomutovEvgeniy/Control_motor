/*******************************************************************************************************
Описание: Файл для работы с АЦП
Разработчик: Бубников Иван
Заметки: 
*******************************************************************************************************/
#include "adc.h"
#include <stm32f10x.h>
#include <stdbool.h>
#include <string.h>

/***************************************************************************************************
Локальные дефайны
***************************************************************************************************/

#define ADC_IRQ_HANDLER ADC1_2_IRQHandler

/***************************************************************************************************
Локальные типы данных
***************************************************************************************************/

typedef void ( *RccClockCmd )( uint32_t, FunctionalState );

/***************************************************************************************************
Локальные переменные файла
***************************************************************************************************/
const RccClockCmd adc_pin_rcc_cmd = RCC_APB2PeriphClockCmd;
const uint32_t adc_pin_rcc = RCC_APB2Periph_GPIOB;
GPIO_TypeDef * const adc_port = GPIOB;
const uint16_t adc_pin = GPIO_Pin_0;

const RccClockCmd adc_rcc_cmd = RCC_APB2PeriphClockCmd;
const uint32_t adc_rcc = RCC_APB2Periph_ADC1;
ADC_TypeDef * const adc = ADC1;
const uint8_t adc_channel = ADC_Channel_8;
const uint8_t adc_sample_time = ADC_SampleTime_239Cycles5;

const IRQn_Type adc_irq_type = ADC1_2_IRQn;
const uint8_t adc_irq_prio = 2;


/***************************************************************************************************
Глобальные функции
***************************************************************************************************/

/**************************************************************************************************
Описание: Инициализация ADC 
Аргументы: Нет
Возврат:   Нет
Замечания: Для чтения значений с потенциометра
**************************************************************************************************/
void adc_init( void )
{
    //тактирование пина АЦП
    adc_pin_rcc_cmd( adc_pin_rcc, ENABLE );

    //инициализация пина АЦП
    GPIO_InitTypeDef gpioInitStructure;
    gpioInitStructure.GPIO_Pin = adc_pin;
    gpioInitStructure.GPIO_Mode = GPIO_Mode_AIN;
    gpioInitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init( adc_port, &gpioInitStructure );
    
    //тактирование модуля АЦП
    adc_rcc_cmd( adc_rcc, ENABLE );
    
    //инициализация модуля АЦП
    ADC_InitTypeDef adcInitStruct;
    adcInitStruct.ADC_ContinuousConvMode = DISABLE;
    adcInitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    adcInitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    adcInitStruct.ADC_Mode = ADC_Mode_Independent;
    adcInitStruct.ADC_NbrOfChannel = 1;
    adcInitStruct.ADC_ScanConvMode = ENABLE;
    ADC_Init( adc, &adcInitStruct );
    
    //настройка канала преобразования АЦП
    ADC_RegularChannelConfig( adc, adc_channel, 1, adc_sample_time );
    
    //запуск модуля АЦП
    ADC_Cmd( adc, ENABLE );
}


/**************************************************************************************************
Описание: Запуск преобразования
Аргументы: Нет
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void adc_startConvertion( void )
{    
    ADC_SoftwareStartConvCmd( adc, ENABLE );
}

/**************************************************************************************************
Описание: Чтение результата преобразования
Аргументы: Нет
Возврат:   Данные с АЦП
Замечания: 
**************************************************************************************************/
uint32_t adc_getValue( void )
{
    return ADC_GetConversionValue( adc );
}

/**************************************************************************************************
Описание: Проверка, закончилось ли преобразование
Аргументы: Нет
Возврат:   ФЛаг окончания работы АЦП
Замечания: Сбрасывает флаг
**************************************************************************************************/
bool adc_checkConversionFinished( void )
{
    if( ADC_GetFlagStatus( adc, ADC_FLAG_EOC ) )
    {
        ADC_ClearFlag( adc, ADC_FLAG_EOC );
        return true;
    }
    
    return false;
}
