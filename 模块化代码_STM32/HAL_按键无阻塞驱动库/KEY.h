#ifndef __KEY_H
#define __KEY_H

/*按键的数量*/
#define KEY_COUNT				4 
/*按键对应的键码*/
#define KEY_1					0 
#define KEY_2					1
#define KEY_3					2
#define KEY_4					3
/*对应的位操作*/
#define KEYHOLD 0X01
#define KEYDOWN 0X02
#define KEYUP 0X04
#define KEYSINGLE 0x08
#define KEYDOUBLE 0x10
#define KEYLONG 0x20
#define KEYREPEAT 0x40

uint8_t GetKey_State(uint8_t n);
uint8_t Key_Check(uint8_t n, uint8_t Flag_key);
void Key_Tick(void);

#endif


