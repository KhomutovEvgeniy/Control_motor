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
#include "systick/systick.h"
#include "motor_speed/motor_speed.h"
#include "motor_voltage/motor_voltage.h"
#include "control/control.h"

/***************************************************************************************************
Локальные переменные файла
***************************************************************************************************/
const char speedRequest[] = "\nEnter speed:'+xxxx' or '-xxxx':";

// char message[] = "5";

/***************************************************************************************************
Глобальные функции
***************************************************************************************************/

int main( void )
{
    initPWM();
    
    adc_init();
    
    adc_startConvertion();
    
    systick_init();
    
    pid_init( );

    static int16_t sensorSpeed = 0;
    
    static int16_t userSpeed = 0;

    static int16_t mistake = 0;
    
    int16_t signal = 0;
    
    init_link_pc_uart();
    

    while ( 1 )
    {
        if ( check_rx_flag() == 1 )
        {
            reset_rx_flag();
            SysTick->CTRL &= ~SysTick_CTRL_TICKINT; //Выключить прерывание от SysTick
            send_to_pc( (char*)speedRequest, sizeof(speedRequest) - 1 );
            clear_rx_buffer();
            // Ожидание получения задания
            while ( check_rx_flag() == 0)
            {;}
            
            reset_rx_flag();
            check_task_speed();
            
            systick_init();
        }
         
        sensorSpeed = motor_speed_getSpeed( ); // Значение скорости с датчика мотора
        userSpeed = get_user_speed( ); // Желаемое значение

        mistake = userSpeed - sensorSpeed; // Ошибка управления
        signal = control_run(mistake);
            
        motor_voltage_setVoltage(signal);
        if ( delay_ms(1000) == true)
        {
            send_cur_speed( sensorSpeed );
        }
        clear_rx_buffer();
    }
    
    // int16_t speed = get_cur_speed( );
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
