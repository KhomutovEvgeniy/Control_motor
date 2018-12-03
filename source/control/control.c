#include "control.h"   
#include "stdlib.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"


PID stmPID;                  
  
void pid_init(void)   
{   
	stmPID.P = 20;             //Proportional  
	stmPID.I = 1;              //Integral 
	stmPID.D = 0.5;             //Derivative  
	stmPID.LastError = 0;       //ошибка[K-1]   
	stmPID.PrevError = 0;       //ошибка[K-2]   
	stmPID.SumError = 0;   
}   
   
int16_t control_run( int16_t mistake)   
{   
    int16_t pError; 
    int16_t iError; 
    int16_t dError;   
	int16_t temp;   
	
	pError = mistake-stmPID.LastError;//ошибка[K]-ошибка[K-1]   
	
	iError = mistake;    //ошибка[K]   
	
	dError = mistake - 2 * (stmPID.LastError) + stmPID.PrevError; //ошибка[K]-2*ошибка[K-1]+ошибка[K-2]   
	
	temp = stmPID.P * pError + stmPID.I * iError + stmPID.D * dError;  
	
	stmPID.PrevError = stmPID.LastError;//ошибка[K-2]   
	
	stmPID.LastError = mistake; //ошибка[K-1]   
	
	return temp;   
}
