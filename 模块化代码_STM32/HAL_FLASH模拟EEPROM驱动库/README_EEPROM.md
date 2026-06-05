# FLASH 模拟 EEPROM 驱动库

## 1. 简介和功能

本库为 FLASH 模拟 EEPROM 驱动库，基于 STM32 进行开发。模拟 EEPROM 的原理是将数据写入 FLASH 中，然后通过读取 FLASH 中的数据来模拟 EEPROM 的功能。

## 2. 移植步骤与使用说明

1. 将 DRIVE_STMFLASH.c 和 DRIVE_STMFLASH.h 文件添加到工程中
2. 在 main.c 中包含

   ```c
   #include "DRIVE_STMFLASH.h"
   ```

3. 调用语句

   ```c
   stmflash_write(FLASH_SAVE_ADDR, (uint16_t *)Flash_Buffer, BUFFER_SIZE);

   stmflash_read(FLASH_SAVE_ADDR, (uint16_t *)Flash_Buffer, BUFFER_SIZE);
   ```

## 3. 注意事项

1. FLASH_SAVE_ADDR 为 FLASH 保存数据的地址，需要根据实际使用的芯片进行修改，stm32f103c8t6 可以不用更改
2. BUFFER_SIZE 为保存数据的长度，必须是偶数
3. Flash_Buffer 需要是 uint16_t 类型的数组，长度为偶数
