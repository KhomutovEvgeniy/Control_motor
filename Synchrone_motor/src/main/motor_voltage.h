#pragma once

#include <stdint.h>

void initPWM( void ); 

void motor_voltage_setVoltage(int32_t voltageA, int32_t voltageB, int32_t voltageC);
