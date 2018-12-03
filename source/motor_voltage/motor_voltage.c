/*******************************************************************************************************
Описание: Файл для установки напряжения на мотор
Разработчик: Хомутов Евгений
Заметки:
*******************************************************************************************************/
#include "motor_voltage.h"

#include "stdlib.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

/***************************************************************************************************
Локальные дефайны
***************************************************************************************************/
#define PERIOD 1000

/**************************************************************************************************
Описание: Инициализация пинов под ШИМ
Аргументы: Нет
Возврат:   Нет
Замечания: Нет
**************************************************************************************************/
void initPWM( void ) 
{   
    GPIO_InitTypeDef port;
    TIM_TimeBaseInitTypeDef timer;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // Тактирование порта A
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   // Тактирование TIM1
     
    //Настраиваем PA8 на генерацию ШИМ
    GPIO_StructInit(&port);
    port.GPIO_Mode = GPIO_Mode_AF_PP;
    port.GPIO_Pin = GPIO_Pin_8;
    port.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &port);        
    
    TIM_TimeBaseStructInit(&timer);  
    
    //Настройка TIM1 на частоту 200Гц и направлением счета на возрастание
    timer.TIM_Prescaler = 720 - 1;
    timer.TIM_Period = PERIOD - 1;
    timer.TIM_ClockDivision = 0;
    timer.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &timer);  
    
    //Структура для первого канала TIM1
    TIM_OCInitTypeDef timerPWM;       
    
    TIM_OCStructInit(&timerPWM);
    
    timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
    timerPWM.TIM_OutputState = TIM_OutputState_Enable;
    timerPWM.TIM_Pulse = 10;
    //timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM1, &timerPWM);    //Настройка таймерного канала
    
    GPIOA->CRH&=~(0xF000);  
    GPIOA->CRH|=0x3000; //PA11 на выход
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);           //Запуск TIM1

}

/**************************************************************************************************
Описание: Преобразование значения из одного диапазона в другой  
Аргументы:
x - значение, которое преобразуем
in_min/in_max  -диапазон, из которого преобразуем
out_min/out_max  -диапазон, в который преобразуем
Возврат:   Нет
Замечания: для генерации ШИМа на двигателе в диапазоне от -1000 до 1000 
**************************************************************************************************/
int16_t map(int16_t x, int16_t in_min, int16_t in_max, int16_t out_min, int16_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


/**************************************************************************************************
Описание: Выдача напряжений обоих полярностей на обмотки двигателя  
Аргументы: pulseWidths - коэффициент заполнения ШИМа 
Возврат:   Нет
Замечания: для генерации ШИМа на двигателе в диапазоне от -1000 до 1000 
**************************************************************************************************/

void motor_voltage_setVoltage(int16_t speed)
{    
    int16_t pulseWidths = map(speed, -2048, 2047, -1000, 1000);
    
    if ( pulseWidths > 0 )  
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_11);
        TIM_SetCompare1(TIM1, pulseWidths);     
    }
    else
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_11);       
        TIM1->CCR1 = 1000 + pulseWidths;
    }
}
