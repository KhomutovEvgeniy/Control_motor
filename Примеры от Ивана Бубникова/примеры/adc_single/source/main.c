/*******************************************************************************************************
Описание: Основной файл примера АЦП
Разработчик: Бубников Иван
Заметки:
*******************************************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>
#include <string.h>
#include <stdio.h>
#include "adc/adc.h"

/***************************************************************************************************
Глобальные функции
***************************************************************************************************/

int main( void )
{
    adc_init();
    
    adc_startConvertion();
    
    while(1)
    {
        if( adc_checkConversionFinished() )
        {
            char str[10];
            sprintf( str, "%d \n\r", adc_getValue() );
            
            adc_startConvertion();
        }
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
  {
  }
}
#endif
