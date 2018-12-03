#pragma once

#include <stdint.h>
#include <stdbool.h>

void systick_init( void );

bool delay_ms( uint32_t delay);

void systick_start( void );

void systick_stop( void );
