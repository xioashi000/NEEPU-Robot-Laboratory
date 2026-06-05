#include "MyFunc.h"
#include "stdint.h"
#include "tim.h"
#include "TB67H450.h"
#include "stdbool.h"
#include "gpio.h"


void TB67H450_Init(void)
{
    HAL_TIM_PWM_Start(&timer_A,TIM_CHANNEL_A); //启动定时器
    HAL_TIM_PWM_Start(&timer_B,TIM_CHANNEL_B); //启动定时器
    PWM_INA(0);
    PWM_INB(0);
}

void TB67H450_SetInputA(bool IN1, bool IN2) //AP,AM
{
    HAL_GPIO_WritePin(IN_AM_GPIO_Port, IN_AM_Pin, (GPIO_PinState)IN2);
    HAL_GPIO_WritePin(IN_AP_GPIO_Port, IN_AP_Pin, (GPIO_PinState)IN1);
}

void TB67H450_SetInputB(bool IN1, bool IN2) //BP,BM
{
    HAL_GPIO_WritePin(IN_BM_GPIO_Port, IN_BM_Pin, (GPIO_PinState)IN2);
    HAL_GPIO_WritePin(IN_BP_GPIO_Port, IN_BP_Pin, (GPIO_PinState)IN1);
}

void PWM_INA(uint8_t percent)
{
    if (percent > 100) percent = 100;
    float duty = (float)timer_A.Init.Period * percent / 100.0f;
    __HAL_TIM_SetCompare(&timer_A, TIM_CHANNEL_A, (uint32_t)duty);
}

void PWM_INB(uint8_t percent)
{
    if (percent > 100) percent = 100;
    float duty = (float)timer_B.Init.Period * percent / 100.0f;
    __HAL_TIM_SetCompare(&timer_B, TIM_CHANNEL_B, (uint32_t)duty);
}

void TB67H450_Sleep(void)
{
    PWM_INA(0);
    PWM_INB(0);
    TB67H450_SetInputA(false, false);
    TB67H450_SetInputB(false, false);
}

void TB67H450_Brake(void)
{
    PWM_INA(0);
    PWM_INB(0);
    TB67H450_SetInputA(true, true);
    TB67H450_SetInputB(true, true);
}

void StepCtrl(uint8_t step) // 步数
{
    static uint8_t last_step = 8;
    if (step != last_step) {
        if (step == 0) {
            HAL_GPIO_WritePin(IN_AP_GPIO_Port, IN_AP_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN_AM_GPIO_Port, IN_AM_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_BP_GPIO_Port, IN_BP_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_BM_GPIO_Port, IN_BM_Pin, GPIO_PIN_RESET);
        }
        if (step == 1) {
            HAL_GPIO_WritePin(IN_AP_GPIO_Port, IN_AP_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN_AM_GPIO_Port, IN_AM_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_BP_GPIO_Port, IN_BP_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN_BM_GPIO_Port, IN_BM_Pin, GPIO_PIN_RESET);
        }
        if (step == 2) {
            HAL_GPIO_WritePin(IN_AP_GPIO_Port, IN_AP_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_AM_GPIO_Port, IN_AM_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_BP_GPIO_Port, IN_BP_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN_BM_GPIO_Port, IN_BM_Pin, GPIO_PIN_RESET);
        }
        if (step == 3) {
            HAL_GPIO_WritePin(IN_AP_GPIO_Port, IN_AP_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_AM_GPIO_Port, IN_AM_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN_BP_GPIO_Port, IN_BP_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN_BM_GPIO_Port, IN_BM_Pin, GPIO_PIN_RESET);
        }
        if (step == 4) {
            HAL_GPIO_WritePin(IN_AP_GPIO_Port, IN_AP_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_AM_GPIO_Port, IN_AM_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN_BP_GPIO_Port, IN_BP_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_BM_GPIO_Port, IN_BM_Pin, GPIO_PIN_RESET);
        }
        if (step == 5) {
            HAL_GPIO_WritePin(IN_AP_GPIO_Port, IN_AP_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_AM_GPIO_Port, IN_AM_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN_BP_GPIO_Port, IN_BP_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_BM_GPIO_Port, IN_BM_Pin, GPIO_PIN_SET);
        }
        if (step == 6) {
            HAL_GPIO_WritePin(IN_AP_GPIO_Port, IN_AP_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_AM_GPIO_Port, IN_AM_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_BP_GPIO_Port, IN_BP_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_BM_GPIO_Port, IN_BM_Pin, GPIO_PIN_SET);
        }
        if (step == 7) {
            HAL_GPIO_WritePin(IN_AP_GPIO_Port, IN_AP_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(IN_AM_GPIO_Port, IN_AM_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, IN_BP_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(IN_BM_GPIO_Port, IN_BM_Pin, GPIO_PIN_SET);
        }
        last_step = step;
    }
}

void Set_Output_I(int Ia, int Ib)
{
    if (Ia > 0) {
        TB67H450_SetInputA(true, false);
    } else {
        Ia = -Ia;
        TB67H450_SetInputA(false, true);
    }
    if (Ib > 0) {
        TB67H450_SetInputB(true, false);
    } else {
        Ib = -Ib;
        TB67H450_SetInputB(false, true);
    }
    PWM_INA(Ia);
    PWM_INB(Ib);
}
