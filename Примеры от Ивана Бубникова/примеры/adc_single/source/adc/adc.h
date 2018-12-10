#pragma once


#include <stdint.h>
#include <stdbool.h>

void adc_init( void );
void adc_startConvertion( void );
uint32_t adc_getValue( void );
bool adc_checkConversionFinished( void );

