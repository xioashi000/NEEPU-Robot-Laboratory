# MT6816 磁编码位置检测驱动库

## 1. 简介和功能

本库为 W25Q64 FLASH 模拟 SPI 驱动库，基于 STM32 进行开发。

## 2. 移植步骤与使用说明

1. 将包含的五个文件添加到工程中
2. 在 MySPI.h 和 cube_MX 中修改引脚定义，其中 MISO 应上拉输入
3. 在 main.c 中包含

   ```c
   #include "W25Q64.h"
   ```

4. 调用语句

   ```c
   W25Q64_Init(); //初始化

   //测试读取ID
   uint8_t MID; //定义用于存放MID号的变量
   uint16_t DID; //定义用于存放DID号的变量
   W25Q64_ReadID(&MID, &DID); //获取W25Q64的ID号
   printf("\nW25Q64厂家ID->%d, 设备ID->%d",MID,DID);

   W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count); //按页写指令

   W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count); //读指令

   W25Q64_SectorErase(uint32_t Address); //扇区擦除指令
   ```

5. 示例

   ```c
   void Task_Clear(void)
   {
      W25Q64_SectorErase(0x000000);//扇区擦除
      for (int i = 0; i < 50; i++) {
            TASKS[i][0] = i;
            TASKS[i][1] = TASKS[i][2] = TASKS[i][3] = TASKS[i][4] = 0;
      }
      processing_schdule();
      printf("\n格式化任务表!!!!!");
      Count_TASKS();
      W25Q64_PageProgram(0x000000, (uint8_t *)TASKS, 250);//写新的TASKS到第0页
            for (int i = 0; i < 50; i++) {
         printf("\n");
         for (int j = 0; j < 5; j++) {
         printf("%03d", TASKS[i][j]);
         }
         }
      printf("\n更新任务表成功!");
      Task_Init();

   }
   ```

## 3. 注意事项

1. 引脚较多，注意正确接线
2. SPI 为模式 0
3. 自动写使能，无需再次调用
4. 可以跨页读，不能跨页写
5. 写入前必须页擦除，否则会导致数据错乱
