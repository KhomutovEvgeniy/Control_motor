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
Локальные типы данных
***************************************************************************************************/

typedef void ( *RccClockCmd )( uint32_t, FunctionalState );

/***************************************************************************************************
Прототипы локальных функций
***************************************************************************************************/
static void initDma( void );


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

const uint8_t adc_channels = 1;


const RccClockCmd dma_rcc_cmd = RCC_AHBPeriphClockCmd;
const uint32_t dma_rcc = RCC_AHBPeriph_DMA1;
DMA_TypeDef * const dma = DMA1;
DMA_Channel_TypeDef * const dma_channel = DMA1_Channel1;

static uint16_t adcConvertedValue;

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
    initDma();
    
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
    adcInitStruct.ADC_ContinuousConvMode = ENABLE;
    adcInitStruct.ADC_DataAlign = ADC_DataAlign_Right;
    adcInitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    adcInitStruct.ADC_Mode = ADC_Mode_Independent;
    adcInitStruct.ADC_NbrOfChannel = adc_channels;
    adcInitStruct.ADC_ScanConvMode = ENABLE;
    ADC_Init( adc, &adcInitStruct );
    
    //настройка канала преобразования АЦП
    ADC_RegularChannelConfig( adc, adc_channel, 1, adc_sample_time );
    
    //настройка АЦП на работу с прямым доступом к памяти
    ADC_DMACmd(ADC1, ENABLE);
    
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
    return adcConvertedValue;
}

/***************************************************************************************************
Локальные функции
***************************************************************************************************/
void initDma( void )
{
    //тактирование модуля прямого доступа к памяти
    dma_rcc_cmd( dma_rcc, ENABLE );
    
    //инициализация
    DMA_InitTypeDef dmaInitStructure;
    dmaInitStructure.DMA_BufferSize = adc_channels;
    dmaInitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    dmaInitStructure.DMA_M2M = DMA_M2M_Disable;
    dmaInitStructure.DMA_MemoryBaseAddr = (uint32_t)&adcConvertedValue;
    dmaInitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dmaInitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dmaInitStructure.DMA_Mode = DMA_Mode_Circular;
    dmaInitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(adc->DR);
    dmaInitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dmaInitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dmaInitStructure.DMA_Priority = DMA_Priority_High;

    DMA_Init( dma_channel, &dmaInitStructure);

    //запуск модуля прямого доступа к памяти
    DMA_Cmd( dma_channel, ENABLE );

}

