#ifndef _DRIVE_STMFLASH_H_
#define _DRIVE_STMFLASH_H_
#include "stm32f1xx_hal.h"
#include <stdint.h>



/* FLASH起始地址 */
#define STM32_FLASH_BASE        0x08000000  		/* STM32 FLASH 起始地址 */
#define STM32_FLASH_SIZE        0x10000         	/* STM32 FLASH 总大小 */
/* STM32F103扇区大小 */
#if STM32_FLASH_SIZE < 256 * 1024
#define STM32_SECTOR_SIZE  1024		/* 容量小于256K的F103, 扇区大小为1K字节 */
#else
#define STM32_SECTOR_SIZE  2048		/* 容量大于等于256K的F103, 扇区大小为2K字节 */
#endif


/* 设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小 + 0X08000000) */
#define FLASH_SAVE_ADDR 0X08006000
/* 设置FLASH 保存内容*/
#define BUFFER_SIZE 100 
extern uint16_t Flash_Buffer[];
void stmflash_read(uint32_t raddr, uint16_t *pbuf, uint16_t length);//length应为偶数
void stmflash_write_nocheck(uint32_t waddr, uint16_t *pbuf, uint16_t length);//length应为偶数
uint16_t stmflash_read_halfword(uint32_t faddr);
void stmflash_write(uint32_t waddr, uint16_t *pbuf, uint16_t length);//length应为偶数



#endif

