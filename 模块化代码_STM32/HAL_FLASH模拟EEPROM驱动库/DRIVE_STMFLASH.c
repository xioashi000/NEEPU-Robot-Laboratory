#include "DRIVE_STMFLASH.h"
#include "stm32f1xx_hal.h"

uint16_t Flash_Buffer[BUFFER_SIZE] = {1,2,3,4};

uint16_t stmflash_read_halfword(uint32_t faddr)
{
    return *(volatile uint16_t *)faddr;
}

/**
 * @brief     	从指定地址开始读出指定长度的数据
 * @param     	raddr : 起始地址
 * @param     	pbuf  : 数据指针
 * @param      	length: 要读取的半字(16位)数,即2个字节的整数倍
 * @retval      	无
 */
void stmflash_read(uint32_t raddr, uint16_t *pbuf, uint16_t length)
{
uint16_t i;

    for (i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_halfword(raddr); 	/* 读取2个字节 */
        raddr += 2; 									/* 偏移2个字节 */
    }
}

void stmflash_write_nocheck(uint32_t waddr, uint16_t *pbuf, uint16_t length)
{
uint16_t i;

    for (i = 0; i < length; i++)
    {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, waddr, pbuf[i]);
        waddr += 2; 		/* 指向下一个半字 */
    }
}

uint16_t g_flashbuf[STM32_SECTOR_SIZE / 2]; /* 最多是2K字节 */
void stmflash_write(uint32_t waddr, uint16_t *pbuf, uint16_t length)
{
    uint32_t secpos;    		/* 扇区地址 */
    uint16_t secoff;    		/* 扇区内偏移地址(16位字计算) */
    uint16_t secremain; 		/* 扇区内剩余地址(16位字计算) */
    uint16_t i;
    uint32_t offaddr;   		/* 去掉0X08000000后的地址 */
    FLASH_EraseInitTypeDef flash_eraseop;
uint32_t erase_addr;   	/* 擦除错误，这个值为发生错误的扇区地址 */

   if(waddr<STM32_FLASH_BASE||(waddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))
    {
        return; 				/* 非法地址 */
}

HAL_FLASH_Unlock();		/* FLASH解锁 */

    offaddr = waddr - STM32_FLASH_BASE;     		/* 实际偏移地址 */
    secpos = offaddr / STM32_SECTOR_SIZE;		/* 得到扇区编号 */
    secoff = (offaddr % STM32_SECTOR_SIZE) / 2;	/* 在扇区内的偏移(2B为基本单位) */
secremain = STM32_SECTOR_SIZE / 2 - secoff;	/* 扇区剩余空间大小 */

    if (length <= secremain)
    {
        secremain = length;	/* 不大于该扇区范围 */
}

    while (1)
    {
        stmflash_read(secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE,
 g_flashbuf, STM32_SECTOR_SIZE / 2);	/* 读出整个扇区的内容 */

        for (i = 0; i < secremain; i++)		/* 校验数据 */
        {
            if (g_flashbuf[secoff + i] != 0XFFFF)
            {
                break; 						/* 需要擦除 */
            }
        }

        if (i < secremain) 					/* 需要擦除 */
        { 
            flash_eraseop.TypeErase = FLASH_TYPEERASE_PAGES;	/* 选择页擦除 */
            flash_eraseop.NbPages = 1;						/* 要擦除的页数 */
            flash_eraseop.PageAddress = secpos * STM32_SECTOR_SIZE +  
STM32_FLASH_BASE;	/* 要擦除的起始地址 */
            HAL_FLASHEx_Erase(&flash_eraseop, &erase_addr);

            for (i = 0; i < secremain; i++)         		     /* 复制 */
            {
                g_flashbuf[i + secoff] = pbuf[i];
            }
            stmflash_write_nocheck(secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE, 
   g_flashbuf, STM32_SECTOR_SIZE / 2); /* 写入整个扇区 */
        }
        else
        {	   /* 写已经擦除了的,直接写入扇区剩余区间 */
            stmflash_write_nocheck(waddr, pbuf, secremain); 
        }

        if (length == secremain)
        {
            break; 					/* 写入结束了 */
        }
        else       					/* 写入未结束 */
        {
            secpos++;             		/* 扇区地址增1 */
            secoff = 0;             	/* 偏移位置为0 */
            pbuf += secremain;      	/* 指针偏移 */
            waddr += secremain * 2; 	/* 写地址偏移(16位数据地址,需要*2) */
            length -= secremain;    	/* 字节(16位)数递减 */

            if (length > (STM32_SECTOR_SIZE / 2))
            {
                secremain = STM32_SECTOR_SIZE / 2;	/* 下一个扇区还是写不完 */
            }
            else
            {
                secremain = length; 	/* 下一个扇区可以写完了 */
            }
        }
}
    HAL_FLASH_Lock(); 				/* 上锁 */
}
