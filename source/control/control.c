/***************************************************************************************************
Описание: Файл для управления скоростью двигателя на основе ПИД-регулятора
Разработчик: Хомутов Евгений
Заметки:
***************************************************************************************************/
#include "control.h"   
#include <stdlib.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>


/***************************************************************************************************
Локальные дефайны
***************************************************************************************************/
#define MAX_SPEED 2047
#define MIN_SPEED -2048

/***************************************************************************************************
Локальные типы данных
***************************************************************************************************/

/***************************************************************************************************
Локальные переменные файла
***************************************************************************************************/
// Коэффициенты усиления ПИД-регулятора
static float kP = 0.5;
static float kI = 0;
static float kD = 0;

static uint16_t prevErr = 0;
static uint16_t intTerm = 0;

/***************************************************************************************************
Глобальные функции
***************************************************************************************************/

/***************************************************************************************************
Описание: ПИД-регулятор
Аргументы: err - ошибка управления
Возврат: Величина управляющего сигнала
Замечания: 
***************************************************************************************************/

int16_t control_run( int16_t err )
{
    int16_t output;
    intTerm += kI*err;
    output = kP * err;
    output += intTerm;
    output += kD * (err - prevErr);

    if ( output > MAX_SPEED )
    {
        output = MAX_SPEED;
    }
    else if ( output < MIN_SPEED )
    {
        output = MIN_SPEED;
    }

  prevErr = err;

  return output;
}
