/*******************************************************************************************************
Описание: Файл для работы с таймером
Разработчик: Бубников Иван
Заметки: 
*******************************************************************************************************/

#include "timer.h"
#include <stm32f10x.h>
#include "led/led.h"

/***************************************************************************************************
Локальные дефайны
***************************************************************************************************/

#define TIMER_IRQ_HANDLER TIM1_UP_IRQHandler


/***************************************************************************************************
Локальные типы данных
***************************************************************************************************/

typedef void ( *RccClockCmd )( uint32_t, FunctionalState );

/***************************************************************************************************
Локальные переменные файла
***************************************************************************************************/
const RccClockCmd timer_rcc_cmd = RCC_APB2PeriphClockCmd;
const uint32_t timer_rcc = RCC_APB2Periph_TIM1;
TIM_TypeDef * const timer = TIM1;

const uint16_t period = 1000;
const uint16_t freq = 1000;

const IRQn_Type timer_irq_type = TIM1_UP_IRQn;
const uint8_t timer_irq_prio = 2;


/***************************************************************************************************
Глобальные функции
***************************************************************************************************/

/**************************************************************************************************
Описание: Инициализация таймера
Аргументы: Нет
Возврат:   Нет
Замечания: 
**************************************************************************************************/
void timer_init( void )
{
    timer_rcc_cmd( timer_rcc, ENABLE );
    
    TIM_TimeBaseInitTypeDef timerInitStructure;
    
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = period - 1;
    timerInitStructure.TIM_Prescaler = SystemCoreClock / freq / period - 1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    
    TIM_TimeBaseInit( timer, &timerInitStructure );
    
    TIM_ITConfig( timer, TIM_IT_Update, ENABLE );
    
    NVIC_SetPriority( timer_irq_type, timer_irq_prio );
    NVIC_EnableIRQ( timer_irq_type );
    
    TIM_Cmd( timer, ENABLE );
}



/**************************************************************************************************
Описание: Прерывание от таймера
Аргументы: Нет
Возврат:   Нет
Замечания: Изменение состояния порта светодиода
**************************************************************************************************/
void TIMER_IRQ_HANDLER()
{
    if( TIM_GetITStatus( timer, TIM_IT_Update ) == true )
    {
        TIM_ClearITPendingBit( timer, TIM_IT_Update );
        
        static bool ledState = false;
        
        ledState = !ledState;
        
        led_setState( ledState );
    }
        
}


