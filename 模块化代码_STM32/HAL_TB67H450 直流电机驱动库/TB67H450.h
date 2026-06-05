#ifndef _TB67H450_H_
#define _TB67H450_H_
#include "stdbool.h"
#include "stdint.h"
//定义IO口
//A相
#define IN_AM_GPIO_Port GPIOA
#define IN_AM_Pin GPIO_PIN_1
#define IN_AP_GPIO_Port GPIOA
#define IN_AP_Pin GPIO_PIN_3

//B相
#define IN_BP_GPIO_Port GPIOB
#define IN_BP_Pin GPIO_PIN_1
#define IN_BM_GPIO_Port GPIOB
#define IN_BM_Pin GPIO_PIN_10

#define timer_A htim2 //A相定时器
#define TIM_CHANNEL_A TIM_CHANNEL_3 //A相PWM通道
#define timer_B htim3 //B相定时器
#define TIM_CHANNEL_B TIM_CHANNEL_3 //B相PWM通道

void PWM_INA(uint8_t compare);
void PWM_INB(uint8_t compare);
void TB67H450_Init(void);
void TB67H450_SetInputA(bool IN1, bool IN2); //AP,AM
void TB67H450_Sleep(void);
void TB67H450_Brake(void);
void StepCtrl(uint8_t step); // 步数
void Set_Output_I(int Ia, int Ib);
#endif

