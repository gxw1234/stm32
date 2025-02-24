#include "tasks/spi_task.h"
#include "main.h"
#include <stdio.h>

/* CS引脚控制宏定义 */
#define SPI_CS_LOW()       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define SPI_CS_HIGH()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)


/* 读取DRDY引脚状态 */
#define READ_DRDY()        HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_5)

/* ADS1220参考电压 */
#define VREF              2.048f    // 参考电压2.048V
#define ADC_FSR           8388608.0f // 2^23, ADC满量程范围

// #define ADC_FSR           16777216.0f // 2^23, ADC满量程范围

/* 函数声明 */
static uint8_t Is_ADC_Data_Valid(uint32_t adc_value);
static float Convert_ADC_To_Voltage(uint32_t adc_value);
static void DRDY_GPIO_Init(void);
static uint8_t SPI_TransmitReceive(uint8_t data);
void SPI_Transmit(uint8_t data);

/* 将24位ADC数据转换为电压值 */
static float Convert_ADC_To_Voltage(uint32_t adc_value)
{
    int32_t signed_value;
    float voltage;
    
    // 如果是负数（最高位为1）
    if(adc_value & 0x800000) {
        signed_value = (int32_t)(adc_value | 0xFF000000);
    } else {
        signed_value = (int32_t)adc_value;
    }
    
    // 将补码转换为电压值
    voltage = ((float)signed_value * VREF) / ADC_FSR;
    
    return voltage;
}

/* 检查数据是否有效 */
static uint8_t Is_ADC_Data_Valid(uint32_t adc_value)
{
    // 检查是否有异常的高位（24位以上应该都是0或1）
    uint32_t high_bits = (adc_value >> 23) & 0x1FF;  // 检查高9位
    return (high_bits == 0x000 || high_bits == 0x1FF);  // 应该全0或全1
}

/* SPI发送接收函数 */
static uint8_t SPI_TransmitReceive(uint8_t data) {
    uint8_t txData[] = {data};  // 发送数据缓冲区
    uint8_t rxData[1];          // 接收数据缓冲区

    /* 同时发送和接收数据 */
    if(HAL_SPI_TransmitReceive(&hspi1, txData, rxData, sizeof(txData), 100) != HAL_OK)
    {
        Error_Handler();
    }

    return rxData[0];
}

/* 配置DRDY引脚为普通输入模式 */
static void DRDY_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 使能GPIOD时钟 */
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* 配置DRDY引脚(PD5)为输入模式 */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;    /* 普通输入模式 */
    GPIO_InitStruct.Pull = GPIO_PULLUP;        /* 上拉，空闲时为高电平 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void SPI_Transmit(uint8_t data) {
    uint8_t txData[] = {data};  // 发送数据缓冲区
    uint8_t rxData[1];          // 接收数据缓冲区

    /* 拉低CS开始传输 */
    // SPI_CS_LOW();
    
    /* 同时发送和接收数据 */
    if(HAL_SPI_TransmitReceive(&hspi1, txData, rxData, sizeof(txData), 100) != HAL_OK)
    {
        Error_Handler();
    }
    /* 拉高CS结束传输 */
    // SPI_CS_HIGH();

    /* 打印发送和接收的数据 */
    printf("SPI: TX=0x%02X, RX=0x%02X\n", txData[0], rxData[0]);
}

void SPI_Task(void *argument)
{
    /* 初始化DRDY引脚为普通输入 */
    DRDY_GPIO_Init();

    /* 确保电源稳定，等待至少50us */
    vTaskDelay(pdMS_TO_TICKS(1));  // 用1ms确保足够

    /* 拉低CS开始通信 */
    SPI_CS_LOW();
    vTaskDelay(pdMS_TO_TICKS(1));  // 确保td(CSSC)

    /* 发送复位命令 */
    SPI_Transmit(0x06);  // RESET命令
    vTaskDelay(pdMS_TO_TICKS(1));  // 等待至少50us + 32·t(CLK)

    /* 写配置寄存器 */
    SPI_Transmit(0x43);  // WREG命令，写寄存器3
    SPI_Transmit(0x00);  // 配置数据 - 寄存器0：PGA=1, AIN0/AIN1
    SPI_Transmit(0xD4);  // 配置数据 - 寄存器1：DR=20SPS, 连续转换模式
    SPI_Transmit(0x10);  // 配置数据 - 寄存器2：IDAC关闭
    SPI_Transmit(0x00);  // 配置数据 - 寄存器3：默认设置
    vTaskDelay(pdMS_TO_TICKS(1));

    /* 读回寄存器验证配置 */
    // SPI_Transmit(0x23);  // RREG命令，读所有寄存器
    // SPI_Transmit(0xFF);  // 读寄存器值
    // SPI_Transmit(0xFF);
    // SPI_Transmit(0xFF);
    // SPI_Transmit(0xFF);
    // vTaskDelay(pdMS_TO_TICKS(1));
    SPI_Transmit(0x23);

    vTaskDelay(pdMS_TO_TICKS(1));
    /* 发送启动命令，开始连续转换 */
    SPI_Transmit(0x08);  // START/SYNC命令
    vTaskDelay(pdMS_TO_TICKS(1));

    SPI_CS_HIGH();


    /* 主循环 */
    while(1)
    {
        
        static GPIO_PinState last_state = GPIO_PIN_SET;  // 上一次的引脚状态
        GPIO_PinState current_state = READ_DRDY();       // 当前引脚状态
        
        /* 检测下降沿（从高电平变为低电平） */
        if(last_state == GPIO_PIN_SET && current_state == GPIO_PIN_RESET)
        {
            printf("Bottom triggered!\n");
            
            /* 拉低CS开始读数据 */
            SPI_CS_LOW();
            vTaskDelay(pdMS_TO_TICKS(1));  // 等待td(CSSC)


            // SPI_Transmit(0x23);


            /* 读取24位ADC数据 */
            uint32_t adc_value = 0;
            uint8_t msb = SPI_TransmitReceive(0xFF);  // 读取高8位
            uint8_t mid = SPI_TransmitReceive(0xFF);  // 读取中8位
            uint8_t lsb = SPI_TransmitReceive(0xFF);  // 读取低8位
            

            SPI_CS_HIGH();

            /* 组合24位数据 */
            adc_value = (uint32_t)msb << 16 | (uint32_t)mid << 8 | lsb;
            
            /* 检查数据有效性 */
            if(Is_ADC_Data_Valid(adc_value))
            {
                /* 转换为电压值 */
                float voltage = Convert_ADC_To_Voltage(adc_value);
                
                /* 将浮点数分解为整数部分和小数部分 */
                int32_t int_part = (int32_t)voltage;
                int32_t decimal_part = (int32_t)((voltage - int_part) * 1000000); // 保留6位小数
                if(decimal_part < 0) decimal_part = -decimal_part; // 确保小数部分为正
                
                /* 打印原始数据和转换后的电压值 */
                printf("ADC Raw: 0x%06lX (%ld), Voltage: %ld.%06ld V\n", 
                       adc_value, (int32_t)adc_value, int_part, decimal_part);
            }
            else
            {
                printf("Invalid data received: 0x%06lX\n", adc_value);
            }

            vTaskDelay(pdMS_TO_TICKS(1));  // 等待td(SCCS)
            SPI_CS_HIGH();  // 拉高CS结束本次读取
        }
        
        last_state = current_state;  // 保存当前状态为上一次状态
        vTaskDelay(pdMS_TO_TICKS(1));  // 短暂延时，避免过于频繁检查
    }
}
