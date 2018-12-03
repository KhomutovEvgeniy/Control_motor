#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void init_link_pc_uart( void );

void systick_init( void );

void send_to_pc( char* buf, uint32_t len );

void send_cur_speed ( int16_t speed );

int16_t get_user_speed( void );

void enter_task( void );
