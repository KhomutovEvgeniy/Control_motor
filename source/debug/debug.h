#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

void init_link_pc_uart( void );

void send_to_pc( char* buf, uint32_t len );

bool check_rx_flag( void );

void reset_rx_flag( void );

void check_task_speed ( void );

void send_cur_speed ( int16_t speed );

void int_to_str(int16_t value, char buf[]);

void clear_rx_buffer(void);

int16_t get_user_speed( void );

