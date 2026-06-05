# 有对应外设时，可直接配置使用

## 使用说明

仅使用方法，不包含原理

## HAL 库外设

1. 串口通讯

   - 点击 USATRx
   - 设置 MODE 为异步通信(Asynchronous)
   - 基础参数：波特率为 115200 Bits/s。传输数据长度为 8 Bit。奇偶检验无，停止位 1
   - 如果要使用中断，使能接收中断 NVIC,或者使能接收 DMA 中断
   - 串口重定向 （记得开启 micro_lib）

   ```c
   #include "stdio.h"
   int fputc(int ch, FILE *f)
   {
       /*重定向print ，huart是一个usart的结构体，如果需要换串口，改这个就行*/
       HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1000);
       return (ch);
   }

   int fgetc(FILE *f)
   {
       /*重定向scanf，huart是一个usart的结构体，如果需要换串口，改这个就行*/
       int ch;
       HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 1000);
       return (ch);
   }
   ```

   - 串口发送/接收函数

   ```c
   HAL_UART_Transmit(); //串口发送数据，使用超时管理机制
   HAL_UART_Receive(); //串口接收数据，使用超时管理机制
   HAL_UART_Transmit_IT(); //串口中断模式发送
   HAL_UART_Receive_IT(); //串口中断模式接收
   HAL_UART_Transmit_DMA(); //串口DMA模式发送
   HAL_UART_Transmit_DMA(); //串口DMA模式接收
   ```

   - 中断接收数据

   ```c
   HAL_UART_IRQHandler(UART_HandleTypeDef *huart);  //串口中断处理函数
   HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);  //串口发送中断回调函数
   HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart);  //串口发送一半中断回调函数（用的较少）
   HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);  //串口接收中断回调函数
   HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart);//串口接收一半回调函数（用的较少）
   HAL_UART_ErrorCallback(); //串口接收错误函数
   ```

   - 示例

   ```c
   HAL_UART_Transmit(&huartx,(uint8_t*)”Hello”,5,0xffff);
   HAL_UART_Receive(&huartx,buf,3,0xffff);

   ```

2. PWM 输出

   - cube_mx 中 PWM 配置

   - 在 PWM 输出模式下，除了 CNT（计数器当前值）、ARR（自动重装载值）之外，还多了一个值 CCRx（捕获/比较寄存器值）。
   - 当 CNT 小于 CCRx 时，TIMx_CHx 通道输出低电平；
     当 CNT 等于或大于 CCRx 时，TIMx_CHx 通道输出高电平。

   - PWM 频率：

     $$
     Fpwm =Tclk / ((arr+1)*(psc+1))(单位：Hz)
     $$

     arr 是计数器值
     psc 是预分频值

   - PWM 占空比：

     $$
     duty circle = TIM2->CCR2 / arr %
     $$

   - PWM 语句

   ```c
   HAL_TIM_PWM_Start(&htimx,TIM_CHANNEL_x); //启动定时器
   __HAL_TIM_SetCompare(&htimx,TIM_CHANNEL_1,cont); //设置占空比
   ```

3. 定时器

   - cube_mx 中定时器配置
   - 定时器中断

   ```c
   HAL_TIM_Base_Start_IT(&htimx); //启动定时器中断
   HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim); //定时器中断回调函数
   ```

4. 外部中断

   - cube_mx 中外部中断配置 NVIC
   - 外部中断回调函数

   ```c
   HAL_GPIO_EXTI_Callback(); //外部中断回调函数
   ```

   - 中断检测按键并且消抖的方法（不推荐）

   ```c
   #define DEBOUNCE_DELAY 50  // 消抖延时（毫秒）

   void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
   {
       HAL_Delay(DEBOUNCE_DELAY);  // 延时消抖

       // 再次检查引脚状态，确认是否为有效触发
       if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET)
       {
           if (GPIO_Pin == GPIO_PIN_0)
           {
               // 处理引脚0的中断
           }
           else if (GPIO_Pin == GPIO_PIN_1)
           {
               // 处理引脚1的中断
           }
           // 其他引脚...
        }
    }
   ```

5. USB 模拟串口(CDC 模式)

   - cube_mx 中 USB 选择 CDC ， 其他保持默认
   - 发送

   ```c
   #include "usbd_cdc_if.h" //  包含头文件
    ...
    main()
    {
    ...
      CDC_Transmit_FS(&i, 1); //发送1个u8的数据
    ...
    }

   ```

   - 发送重定向

   ```c
   #include <stdarg.h>
   #include <stdio.h>

   unsigned char usbtemp[64];

   void USBVcom_printf(const char *format,...)
   {
      unsigned short len;

      va_list args;
      va_start(args, format);
      len = vsnprintf((char*)usbtemp, sizeof(usbtemp)+1, (char*)format,args);
      va_end(args);

      //HAL_UART_Transmit_IT(&huart1, printf_temp, len);
      CDC_Transmit_FS(usbtemp, len);
   }
   ```

6. 传感器

   - 传感器驱动库
   - 传感器初始化
   - 传感器数据读取

7. 传感器

   - 传感器驱动库
   - 传感器初始化
   - 传感器数据读取

8. 传感器
