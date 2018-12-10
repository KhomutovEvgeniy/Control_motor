/**************************************************************************************************
 CONFIG.H

 Файл конфигурации устройства
   
**************************************************************************************************/

#pragma once



/**************************************************************************************************
                                     ГЛОБАЛЬНЫЕ ОПРЕДЕЛЕНИЯ
**************************************************************************************************/
// Пины вывода
#define M1_PORTA GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7
#define M2_PORTA GPIO_Pin_0|GPIO_Pin_1
#define M2_PORTC GPIO_Pin_14|GPIO_Pin_15
#define M3_PORTA GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12
#define M3_PORTB GPIO_Pin_5
#define M4_PORTB GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9

#define M1_P1 GPIO_Pin_4
#define M1_P2 GPIO_Pin_5
#define M1_P3 GPIO_Pin_6
#define M1_P4 GPIO_Pin_7
#define M2_P1 GPIO_Pin_14
#define M2_P2 GPIO_Pin_15
#define M2_P3 GPIO_Pin_0
#define M2_P4 GPIO_Pin_1
#define M3_P1 GPIO_Pin_5
#define M3_P2 GPIO_Pin_12
#define M3_P3 GPIO_Pin_11
#define M3_P4 GPIO_Pin_8
#define M4_P1 GPIO_Pin_9
#define M4_P2 GPIO_Pin_8
#define M4_P3 GPIO_Pin_7
#define M4_P4 GPIO_Pin_6

#define Switch_PORTB GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14

#define LVL_TRANS_EN_PORTB GPIO_Pin_0|GPIO_Pin_15
#define LVL_TRANS_EN_PORTC GPIO_Pin_13


// Пины RS-485

#define PIN_RS485_DE GPIO_Pin_1
#define PORT_RS485_DE GPIOA
