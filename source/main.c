/*******************************************************************************************************
Описание: Основной файл работы с мотором 
(первая лабораторка по дисциплине Программирование микроконтроллеров в робототехнике)
Разработчик: Хомутов Евгений
Заметки:
*******************************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>
#include <string.h>
#include <stdio.h>
#include "adc/adc.h"
#include "debug/debug.h"
#include "motor_speed/motor_speed.h"
#include "motor_voltage/motor_voltage.h"
#include "control/control.h"

/***************************************************************************************************
Глобальные функции
***************************************************************************************************/

int main( void )
{
    initPWM();
    
    adc_init();
    
    adc_startConvertion();
    
    systick_init();
    
    static int16_t sensorSpeed = 0;
    
    static int16_t userSpeed = 0;

    static int16_t mistake = 0;
    
    int16_t signal = 0;
    
    init_link_pc_uart();
    

    while ( 1 )
    {
        enter_task();
         
        sensorSpeed = motor_speed_getSpeed( ); // Значение скорости с датчика мотора
        userSpeed = get_user_speed( ); // Желаемое значение

        mistake = userSpeed - sensorSpeed; // Ошибка управления
        signal = control_run(mistake); // Управляющий сигнал с ПИДа
            
        motor_voltage_setVoltage(signal);
        send_cur_speed(sensorSpeed);
    }
 }


#ifdef  USE_FULL_ASSERT
/*
    * @brief  Reports the name of the source file and the source line number
    *         where the assert_param error has occurred.
    * @param  file: pointer to the source file name
    * @param  line: assert_param error line source number
    * @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
 
  /* Infinite loop */
    while (1)
    {;}
}
#endif
