/**************************************************************************************************
 MACROS.H

 Макросы
   
**************************************************************************************************/

#pragma once


/**************************************************************************************************
                                           МАКРОСЫ
**************************************************************************************************/

// Обеспечение критической секции кода
#define ENTER_CRITICAL_SECTION()    int wasMasked = __disable_irq()
#define LEAVE_CRITICAL_SECTION()    if ( !wasMasked ) __enable_irq()

// Установить BIT в ADDRESS
#define SETBIT(ADDRESS,BIT) ( (ADDRESS) |= ( 1<<(BIT) ) )

// Сбросить BIT в ADDRESS
#define CLEARBIT(ADDRESS,BIT) ( (ADDRESS) &= ~( 1<<(BIT) ) )

// Проверить BIT в ADDRESS
#define TESTBIT(ADDRESS,BIT) ( (ADDRESS) & ( 1<<(BIT) ) )

// Вернуть количество элементов в массиве
#define NUM_ELEM(x) (sizeof (x) / sizeof (*(x)))

// Зависнуть и ждать перезагрузки по сторожевому таймеру
#define FAIL_ASSERT() do{ __disable_irq(); while(1){continue;} }while(0)
