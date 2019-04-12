#pragma once
/*******************************************************************************************************
Описание: Основной файл работы с синхронным двигателем
(Вторая лабораторка по дисциплине Программирование микроконтроллеров в робототехнике)
Разработчик: Хомутов Евгений
Заметки:
*******************************************************************************************************/
/*******************************************************************************************************
ПАРАМЕТРЫ ПОДКЛЮЧЕНИЯ ДВИГАТЕЛЯ
Фаза двигателя А -> TIM1 channel1 
Фаза двигателя B -> TIM1 channel2 
Фаза двигателя C -> TIM1 channel3

Сопротивление принимается равным 1 Ом
Датчик тока фазы А -> ADC1 input0 
Датчик тока фазы B -> ADC1 input1 Смещение 1.65В!

Датчик положения TIM2->CNT

Разрешение датчика угла поворота 1024
Ноль датчика совпадает с фазой А двигателя.
******************************************************************************************************/

/* Программа должна обеспечивать следующий функционал:

•	Выдачу напряжения на обмотки двигателя с обеспечением верного порядка коммутации

•	Чтение данных с датчиков тока

•	Чтение данных с датчика положения ротора

•	Управление током двигателя на основе ПИ-регулятора

•	Управление скоростью двигателя на основе ПИД-регулятора
*/
#include "project_config.h"
#include "motor_voltage.h"
//#include "motor_speed.h"
#include "motor_regulator.h"
#include "motor_revert.h"

#include "adc.h"

#define MAX_CURRENT 2000
#define MAX_SPEED 10000

static int32_t sensorSpeed = 0; // Скорость, вычисленная с датчика положения

static int32_t sensorCurrentA = 0;
static int32_t sensorCurrentB = 0;
static int32_t sensorCurrentC = 0;

static int32_t currentErrD = 0;
static int32_t currentErrQ = 0;
static int32_t sensorCurrentD = 0; 
static int32_t sensorCurrentQ = 0;
static int32_t signalVoltageA = 0;
static int32_t signalVoltageB = 0;
static int32_t signalVoltageC = 0;

// Скорость задается в диапазоне -100...100 об/сек
volatile int32_t userSpeed = 70; // Желаемое значение
static int32_t speedErr = 0;    // Ошибка по управлению

static int32_t signalVoltageD = 0;
static int32_t signalVoltageQ = 0;

volatile int32_t currentTask = 0;

static int32_t position = 0;

#define SYSTICK_PRESCALER 1000 
#define DELAY 1

/***************************************************************************************************
Локальные типы данных
***************************************************************************************************/
 
/***************************************************************************************************
Прототипы локальных функций
***************************************************************************************************/

/***************************************************************************************************
Локальные переменные файла
***************************************************************************************************/
volatile uint32_t timeStampMs = 0;

/***************************************************************************************************
Прототипы локальных функций
***************************************************************************************************/

void systickStart( void );
void systickStop( void );
bool delayMs(uint32_t delay);

/***************************************************************************************************
Описание: Временная задержка по SysTick
Аргументы: delay - время задержки в мс
Возврат: true - прошло заданное время, false - нет
Замечания: 
***************************************************************************************************/
bool delayMs(uint32_t delay)
{                                                                                  
    
    if ( timeStampMs < delay )
    {
        return false;
    }
    else
    {
        timeStampMs = 0;
        return true;
    }
}

  /*************************************************************************************************
Описание: Остановка таймера SysTick
Аргументы: Нет
Возврат:   Нет
Замечания: 
***************************************************************************************************/
void systickStop( void )
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

/***************************************************************************************************
Описание: Запуск таймера SysTick
Аргументы: Нет
Возврат:   Нет
Замечания: 
***************************************************************************************************/
void systickStart( void )
{
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
}

/***************************************************************************************************
Описание: Чтение значения с датчика положения ротора двигателя
Аргументы: Нет
Возврат:   Значение от 0 до 1024
Замечания: Значение с датчика положения считывается в TIM2->CNT
***************************************************************************************************/
uint16_t motor_position_getPosition( void )
{
    return TIM2->CNT;
}

/***************************************************************************************************
Глобальные функции
***************************************************************************************************/

/***************************************************************************************************
Описание: Получение значения скорости двигателя
Аргументы: Нет
Возврат:   Значение скорости двигателя
Замечания: 
***************************************************************************************************/
int32_t motor_speed_getSpeed( int32_t tempPosition )
{ 
    static int32_t prevPosition = 0;
    float motorSpeed = 0;

    // Если ротор двигателя прошел через начальное положение
    if((tempPosition - prevPosition > 600) || (tempPosition - prevPosition < -600))
    {
        // Если ротор двигателя прошел через начальное положение в положительном вращении
        if(tempPosition - prevPosition < -600)
        {
            motorSpeed = (float)(SYSTICK_PRESCALER / DELAY) * (1024 - prevPosition + tempPosition); 
        }
        // Если ротор двигателя прошел через начальное положение в отрицательном вращении
        else
        {
            motorSpeed = (float)(SYSTICK_PRESCALER / DELAY) * (-1024 - prevPosition + tempPosition);
        }
    }
    // Если ротор двигателя вращается себе спокойно
    else
    {
        motorSpeed = (float)(SYSTICK_PRESCALER / DELAY) * (tempPosition - prevPosition);
    }
    prevPosition = tempPosition;
    //timeStampMs = 0;
    
    motorSpeed /= 1024;  // Нормируем к об/сек

    return (int32_t)motorSpeed;
}

/***************************************************************************************************
Описание: Инициализация SysTick
Аргументы: Нет
Возврат:   Нет
Замечания: Смотреть описание SYSTICK_PRESCALER
***************************************************************************************************/
void systick_init ( void )
{     
    SysTick_Config( SystemCoreClock/SYSTICK_PRESCALER ); 
}

/***************************************************************************************************
Описание: Прерывания по Systick
Аргументы: Нет
Возврат:   Нет
Замечания: 
***************************************************************************************************/
void SysTick_Handler()
{
    timeStampMs++;
}

int main(void)
{   
    initPWM();
    adc_init();
    adc_startConvertion();
    systick_init();
    
    while ( 1 )
    {
        if( delayMs(DELAY) == true)
        {
            position = motor_position_getPosition(); // Получение угла положения ротора в градусах
            
            // Обратная связь по скорости
            
            sensorSpeed = motor_speed_getSpeed(position);     // Значение скорости с датчика мотора

            speedErr = userSpeed - sensorSpeed;       // Ошибка управления по скорости

            currentTask = pid_regulator(speedErr);  // Величина задающего входного тока сигнал с ПИДа (-2000...2000)

            // Обратная связь по току
            sensorCurrentA = motor_current_getCurrent(0);       // Значение тока с датчика тока фазы А (диапазон -2048...2047)
            sensorCurrentB = motor_current_getCurrent(1);       // Значение тока с датчика тока фазы B
            sensorCurrentC = - sensorCurrentA - sensorCurrentB; // Значение тока с фазы C (Ia + Ib + Ic = 0)

            // Перевод из трехфазной СК статора в подвижную двуфазную СК ротора dq
            motor_revert_ABC_to_DQ(sensorCurrentA, sensorCurrentB, sensorCurrentC, position); 
            
            // Получение проекций вектора тока в осях d'q'
            sensorCurrentD = motor_current_getCurrentD();
            sensorCurrentQ = motor_current_getCurrentQ();
          
            currentErrQ = currentTask - sensorCurrentQ;
            currentErrD = - sensorCurrentD;

            signalVoltageD = pi_regulator_axis_d(currentErrD);
            signalVoltageQ = pi_regulator_axis_q(currentErrQ);

            // Формирование задающего воздействия, сначала перевод из dq в alpha/betta, затем в трехфазную СК статора 
            motor_revert_DQ_to_ABC(signalVoltageD, signalVoltageQ, position);

            signalVoltageA = motor_current_getCurrentA();
            signalVoltageB = motor_current_getCurrentB();
            signalVoltageC = motor_current_getCurrentC();
            
            int32_t tempVoltageA = signalVoltageA;
            int32_t tempVoltageB = signalVoltageB;
            int32_t tempVoltageC = signalVoltageC;

            // Перевод отрицательных составляющих, главное шоб фазная разность осталась прежней
            if( tempVoltageA < 0)
            {
                tempVoltageB -= tempVoltageA;
                tempVoltageC -= tempVoltageA;
                tempVoltageA = 0;
            }
            
            if( tempVoltageB < 0)
            {
                tempVoltageA -= tempVoltageB;
                tempVoltageC -= tempVoltageB;
                tempVoltageB = 0;
            }
            
            if( tempVoltageC < 0)
            {
                tempVoltageA -= tempVoltageC;
                tempVoltageB -= tempVoltageC;
                tempVoltageC = 0;
            }

            motor_voltage_setVoltage(tempVoltageA, tempVoltageB, tempVoltageC);
        }
    }
}

// классический ассерт для STM32
#ifdef USE_FULL_ASSERT
    void assert_failed(uint8_t * file, uint32_t line)
    { 
        /* User can add his own implementation to report the file name and line number,
         ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
         
        (void)file;
        (void)line;

        while(1){};
    }
#endif
