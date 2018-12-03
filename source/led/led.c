/*******************************************************************************************************
Описание: Файл для работы со светодиодом
Разработчик: Бубников Иван
Заметки: 
*******************************************************************************************************/

#include "led.h"
#include <stm32f10x.h>
#include <stdbool.h>
#include <string.h>

/***************************************************************************************************
Локальные типы данных
***************************************************************************************************/

typedef void ( *RccClockCmd )( uint32_t, FunctionalState );

/***************************************************************************************************
Локальные переменные файла
***************************************************************************************************/
const RccClockCmd led_rcc_cmd = RCC_APB2PeriphClockCmd;
const uint32_t led_rcc = RCC_APB2Periph_GPIOA;
GPIO_TypeDef * const led_port = GPIOA;
const uint16_t led_pin = GPIO_Pin_0;


/***************************************************************************************************
Глобальные функции
***************************************************************************************************/

/**************************************************************************************************
Описание: Инициализация LED 
Аргументы: Нет
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void led_init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //тактирование пина приёма данных
    led_rcc_cmd( led_rcc, ENABLE );
    //инициализация пина приёма данных
    GPIO_InitStructure.GPIO_Pin = led_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( led_port , &GPIO_InitStructure );
}



/**************************************************************************************************
Описание: Управление светодиодом
Аргументы: True - вкл, false - выкл
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void led_setState( bool state )
{
    if( state == true )
    {
        GPIO_SetBits( led_port, led_pin );
    }
    else
    {
        GPIO_ResetBits( led_port, led_pin);
    }
}

