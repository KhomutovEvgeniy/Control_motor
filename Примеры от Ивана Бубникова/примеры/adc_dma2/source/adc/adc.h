#pragma once

#include <stdint.h>

void adc_init( void );
void adc_startConvertion( void );
uint32_t adc_getValue( uint8_t channel );

