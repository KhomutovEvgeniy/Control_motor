#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifndef PID_H 
#define PID_H 
 
/***************************PID*************************/ 
 
// PID 
typedef struct PID{ 
    int mistake; 
    float P; //Proportional 
    float I; //Integral
    float D; //Derivative   
    int LastError; //ошибка[-1]  
    int PrevError; //ошибка[-2]  
    int SumError; //сумма ошибок 
}PID;  
 
 
void pid_init(void); 
int16_t control_run(int16_t mistake); 

 
#endif 
