#include "main.h"
#include "KEY.h"
#include "stdint.h"
/*按键对应返回值*/
#define KEYPRESSED 1
#define KEYUNPRESSED 0
/*长按、双击、间隔操作对应的时间，单位1ms*/
#define TIMEKEYDOUBLE 200
#define TIMEKEYLONG 3000
#define TIMEKEYREPEAT 100

uint8_t Key_Flag[KEY_COUNT];

uint8_t GetKey_State(uint8_t n)//n表示要读取按键的键码
{
	if(n== KEY_1)
	{
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) == 0)//要读取按键的键码引脚自行修改
		 {
			return KEYPRESSED;
		 }	
	}
	else if(n==KEY_2)
	{
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) == 0)//要读取按键的键码引脚自行修改
		 {
			return KEYPRESSED;
		 }	
	}
		return KEYUNPRESSED;
}
uint8_t Key_Check(uint8_t n, uint8_t Flag_key)
{
	if(Key_Flag[n] & Flag_key)//若传入0x02，判断down位有效
	{
		if(Flag_key != KEYHOLD)
		{
			Key_Flag[n] &= ~Flag_key; //给检查位清0
		}
		return 1;
	}
	return 0;
}
void Key_Tick(void)
{
	static uint8_t count, i;
	static uint8_t CurrState[KEY_COUNT],PrevState[KEY_COUNT];
	static uint8_t State[KEY_COUNT];
	static uint8_t TimeKey[KEY_COUNT];
	count++;
		/*定时时间清零*/
		for (i = 0; i < KEY_COUNT; i ++)
	{
		if (TimeKey[i] > 0)
		{
			TimeKey[i] --;
		}
	}
	/*二十倍频*/
	if(count>=20)
	{
		count= 0;
		for (i = 0; i < KEY_COUNT; i ++)//循环每个按键
	{
			PrevState[i] = CurrState[i];
			CurrState[i] = GetKey_State(i);
			/*判断按键按住是否不放*/	
		if(CurrState[i] == KEYPRESSED) 
		{
			Key_Flag[i] = Key_Flag[i] | KEYHOLD;
		}
		else
		{
			Key_Flag[i] = Key_Flag[i] & ~KEYHOLD;
		}
/*判断按键按下时刻*/		
		if(CurrState[i] == KEYPRESSED && PrevState[i] == KEYUNPRESSED) 
		{
			Key_Flag[i] = Key_Flag[i] | KEYDOWN;
		}
/*判断按键松开时刻*/				
		if(CurrState[i] == KEYUNPRESSED && PrevState[i] == KEYPRESSED) 
		{
			Key_Flag[i] = Key_Flag[i] | KEYUP;
		}
		/*状态机*/
		/*状态0按键空闲*/
		if(State[i] == 0)
		{
			if(CurrState[i] == KEYPRESSED)
			{
			  TimeKey[i] = TIMEKEYLONG;//设定长按定时时间
				State[i] = 1; //跳转状态1
			}
		}
		/*状态1按键按下*/
		else if(State[i] == 1)
		{
			if(CurrState[i] == KEYUNPRESSED) 
			{
			  TimeKey[i] = TIMEKEYDOUBLE;
				State[i] = 2;
			}
			else if(TimeKey[i] == 0) //若
			{
				TimeKey[i] = TIMEKEYREPEAT;
				Key_Flag[i] = Key_Flag[i] | KEYLONG;
				State[i] = 4;
			}
		}
		else if(State[i] == 2)
		{
			if(TimeKey[i] == 0)
			{
				Key_Flag[i] = Key_Flag[i] | KEYSINGLE;
				State[i] = 0;
			}
			else if(CurrState[i] == KEYPRESSED)
			{
				Key_Flag[i] = Key_Flag[i] | KEYDOUBLE;
				State[i] = 3;
			}
		}
		else if(State[i] == 3)
		{
			if(CurrState[i] == KEYUNPRESSED)
			{
				State[i] = 0;
			}
		}
		else if(State[i] == 4)
		{
			if(CurrState[i] == KEYUNPRESSED)
			{
				State[i] = 0;
			}
			
			if(TimeKey[i] == 0)
			{
			 TimeKey[i] = TIMEKEYREPEAT;
			 Key_Flag[i] = Key_Flag[i] | KEYREPEAT;
				State[i] = 4;
			}
		}
	}
	}
	
}
