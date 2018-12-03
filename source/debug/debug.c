/***************************************************************************************************
Описание: Прием и передача данных по USART по SysTick
Разработчик: Хомутов Евгений
Заметки:
***************************************************************************************************/
#include <stm32f10x_usart.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <misc.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "debug/debug.h"

/***************************************************************************************************
Локальные дефайны
***************************************************************************************************/
#define PC_LINK_UART USART1
#define PC_LINK_UART_PORT GPIOA
#define PC_LINK_UART_TX_PIN GPIO_Pin_9
#define PC_LINK_UART_RX_PIN GPIO_Pin_10

#define PC_LINK_UART_BR 115200

#define PC_LINK_UART_RCC_CMD(x) RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, x)
#define PC_LINK_UART_PORT_RCC_CMD(x) RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, x)

#define RX_BUF_SIZE 10

#define MAX_SPEED 2047
#define MIN_SPEED -2048

/**************************************************************************************************
Систик совершает прерывания с частотой 72МГц/SYSTICK_PRESCALER, для того чтоб прерывания выполнялись 
каждую 1мс SYSTICK_PRESCALER = 72МГц/72кГц = 1000.
Для модуляции в протеусе: необходимо домножить SYSTICK_PRESCALER на 72МГц/(частота МК в протеусе), 
для того, чтобы в протеусе прерывания также соверщались каждую 1мс. 
В данном случае частота в протеусе 1МГц
***************************************************************************************************/
#define SYSTICK_PRESCALER 1000

/***************************************************************************************************
Локальные типы данных
***************************************************************************************************/
 
/***************************************************************************************************
Прототипы локальных функций
***************************************************************************************************/
void initUartPins( void );
void clearRxBuf(void);

/***************************************************************************************************
Локальные переменные файла
***************************************************************************************************/
static bool rxFlagEndLine = 0;
static bool startSetTask = 0;
static char RXi;
static char RXc;
static char rxBuf[RX_BUF_SIZE] = {'\0'};
const char speedRequest[] = "Enter speed:'+xxxx' or '-xxxx':";

static int16_t userSpeed = 0;
static char curSpeedStr[] = "+0000";

static volatile uint32_t timeStampMs = 0;

/***************************************************************************************************
Прототипы локальных функций
***************************************************************************************************/
void initUartPins( void );

void systickStart( void );
void systickStop( void );
bool delayMs(uint32_t delay);

void clearRxBuf( void ) ;
bool checkRxFlag ( void );
void resetRxFlag ( void );

void checkTaskSpeed ( void );

void intToStr( int16_t value, char buf[]);
int32_t strToInt( char *s );

/***************************************************************************************************
Описание: Инициализация выводов в режим TX и RX
Аргументы: Нет
Возврат:   Нет
Замечания: 
***************************************************************************************************/
void initUartPins( void )
{
    PC_LINK_UART_PORT_RCC_CMD(ENABLE);

    GPIO_InitTypeDef gpioInitStruct;
    gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInitStruct.GPIO_Pin = PC_LINK_UART_TX_PIN;
    GPIO_Init( PC_LINK_UART_PORT, &gpioInitStruct );

    gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpioInitStruct.GPIO_Pin = PC_LINK_UART_RX_PIN;
    GPIO_Init( PC_LINK_UART_PORT, &gpioInitStruct );
}

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
Описание: Освобождение буфера UART
Аргументы: Нет
Возврат:   Нет
Замечания: 
***************************************************************************************************/
void clearRxBuf( void ) 
{
    for (RXi=0; RXi<RX_BUF_SIZE; RXi++)
        rxBuf[RXi] = '\0';
    RXi = 0;
}

/***************************************************************************************************
Описание: Чтение флага завершения приема сообщения
Аргументы: Нет
Возврат:   true - по UART пришел байт, false - буфер приемника пуст
Замечания: 
***************************************************************************************************/
bool checkRxFlag ( void )
{
    return rxFlagEndLine;
}

/***************************************************************************************************
Описание: Сброс флага завершения приема сообщения
Аргументы: Нет
Возврат:   Нет
Замечания: 
***************************************************************************************************/
void resetRxFlag ( void )
{
    rxFlagEndLine = false;
}

 /**************************************************************************************************
Описание: Преобразование строки в число.
Аргументы: s - строка, которую преобразуем
Возврат:   Нет
Замечания: 
***************************************************************************************************/
int32_t strToInt( char *s )
{
  int32_t temp = 0; // число
  uint8_t i = 0;
  int8_t sign = 0; // знак числа 0 - положительное, 1 — отрицательное
  if ( s[i] == '-' )
  {
    sign = 1;
  }
  else if ( s[i] != '+')
  {
      return userSpeed;  
  }
  i++;
  
  while ( s[i] >= 0x30 && s[i] <= 0x39 )
  {
    temp = temp + (s[i] & 0x0F);
    temp = temp * 10;
    i++;
  }
  temp = temp / 10;
  if ( sign == 1 )
    temp *= -1;
  return ( temp );
}

/***************************************************************************************************
Описание: Преобразование числа в строку
Аргументы: value - число, которое преобразуем, buf - куда преобразуем
Возврат:   Нет
Замечания: Преобразуются только числа в формате "+xxxx" / "-xxxx"
***************************************************************************************************/
void intToStr( int16_t value, char buf[])
{
    if (value < 0) 
    { 
        buf[0] = '-';
        value = -value;
    } 
    else 
    { 
        buf[0] = '+'; 
    }                              
    for (uint8_t i = 1; i < 5; i++) 
    { 
        buf[i] = value % (int)pow(10, (5 - i)) / (int)pow(10, (5 - i - 1)) + '0'; 
    }
}

/***************************************************************************************************
Описание: Проверка правильности задания скорости на годность.
Аргументы: Нет
Возврат:   Нет
Замечания: 
***************************************************************************************************/
void checkTaskSpeed ( void )
{
    int32_t task;
    task = strToInt( (char*)rxBuf );
    
    if ( task >= MIN_SPEED && task <= MAX_SPEED)
    {
        userSpeed = task;
    }
    systick_init( );
}

/***************************************************************************************************
Глобальные функции
***************************************************************************************************/

/***************************************************************************************************
Описание: Инициализация SysTick
Аргументы: Нет
Возврат:   Нет
Замечания: Смотреть описание SYSTICK_PRESCALER
***************************************************************************************************/
void systick_init ( void )
{
    // настройка приоритета прерываний SysTick
    NVIC_InitTypeDef nvicInitStruct;
    nvicInitStruct.NVIC_IRQChannel = SysTick_IRQn;
    nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
    nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init( &nvicInitStruct );

    SysTick_Config( SystemCoreClock/SYSTICK_PRESCALER ); 
}

/***************************************************************************************************
Описание: Инициализация USART1
Аргументы: Нет
Возврат:   Нет
Замечания: 
***************************************************************************************************/
void init_link_pc_uart( void )
{
    PC_LINK_UART_RCC_CMD(ENABLE);
    
    initUartPins();
    
    // конфигурирование UART
    USART_InitTypeDef uartInitStruct;
    USART_StructInit( &uartInitStruct );
    uartInitStruct.USART_BaudRate = PC_LINK_UART_BR;
    uartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init( PC_LINK_UART, &uartInitStruct );
    
    // настройка приоритета прерываний UART
    NVIC_InitTypeDef nvicInitStruct;
    nvicInitStruct.NVIC_IRQChannel = USART1_IRQn;
    nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
    nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    nvicInitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init( &nvicInitStruct );
    
    // запуск USART1
    USART_Cmd( PC_LINK_UART, ENABLE );
    USART_ITConfig( PC_LINK_UART, USART_IT_RXNE, ENABLE);
    
    // подключение DMA к USART1
    USART_DMACmd( PC_LINK_UART, USART_DMAReq_Tx, ENABLE );
    
    // запуск DMA
    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE );
}


/***************************************************************************************************
Описание: Отправка по UART на компьютер
Аргументы: buf - массив символов, то что отправляем, len - длина массива
Возврат:   Нет
Замечания: 
***************************************************************************************************/
void send_to_pc( char* buf, uint32_t len )
{
    DMA_DeInit( DMA1_Channel4 );

    DMA_InitTypeDef dmaInitStruct;
    DMA_StructInit( &dmaInitStruct );
    dmaInitStruct.DMA_BufferSize = len;
    dmaInitStruct.DMA_MemoryBaseAddr = (uint32_t) buf;
    dmaInitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    dmaInitStruct.DMA_M2M = DMA_M2M_Disable;
    dmaInitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dmaInitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dmaInitStruct.DMA_Mode = DMA_Mode_Normal;
    dmaInitStruct.DMA_PeripheralBaseAddr = (uint32_t) &(PC_LINK_UART->DR);
    dmaInitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dmaInitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_Init( DMA1_Channel4, &dmaInitStruct );

    DMA_Cmd( DMA1_Channel4, ENABLE );
}

/***************************************************************************************************
Описание: Отправка текущего значения скорости в терминал каждую секунду
Аргументы: speed - текущее значение скорости с датчика
Возврат:   Нет
Замечания: 
***************************************************************************************************/
void send_cur_speed( int16_t speed )
{
    if ( delayMs(100) == true)
    {
        intToStr(speed, curSpeedStr);
        send_to_pc(curSpeedStr, sizeof(curSpeedStr) - 1);
    }
}


 /**************************************************************************************************
Описание: Ввод значения пользователем
Аргументы: Нет
Возврат:   Нет
Замечания: 
***************************************************************************************************/

void enter_task( void )
{
    if ( startSetTask == true )
    {
        startSetTask = false;
        resetRxFlag();
        systickStop();
        send_to_pc( (char*)speedRequest, sizeof(speedRequest) - 1 );
        
        // ожидани завершения ввода задания
        while ( checkRxFlag() == 0)
        {;}
        
        resetRxFlag();
        checkTaskSpeed();
        
        systickStart();
        clearRxBuf();
    }
}

/***************************************************************************************************
Описание: Получение желаемого значения скорости.
Аргументы: Нет
Возврат:   Значение скорости, введенное пользователем
Замечания: 
***************************************************************************************************/
int16_t get_user_speed( void )
{
    return userSpeed;
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

 /**************************************************************************************************
Описание: Прерывание от UART
Аргументы: Нет
Возврат:   Нет
Замечания: 
***************************************************************************************************/
 void USART1_IRQHandler( void )
{
    // проверка на событие: прилетел байт с компьютера
    if ( USART_GetFlagStatus( PC_LINK_UART, USART_FLAG_RXNE ) )
    {     
        // Сохраняем принятый байт в RX_BUF
        RXc = USART_ReceiveData(USART1);
        rxBuf[RXi] = RXc;
        RXi++;

        // Если не конец строки - нажатие Enter)
        if ( RXc != 13 )
        {
            // Если из терминала в МК пришел символ 'b', то взводится флаг ввода задания
            if (RXc == 'b')
            {
                startSetTask = 1;
                clearRxBuf();
            }
            // очистка буфера приема при переполнении
            if (RXi > RX_BUF_SIZE) 
            {
                clearRxBuf();
            }
        }
        else {
            rxFlagEndLine = 1;
        }
        // Эхо-отклик - вывод в терминал полученного символа
         USART_SendData(USART1, RXc);
    }
} 
