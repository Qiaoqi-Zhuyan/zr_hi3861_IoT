#include "temp_humi_measure.h"


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "stdlib.h"

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_i2c.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_errno.h"
#include "aht20.h"
#include "packet.h"

#define I2C_TASK_STACK_SIZE 4096
#define AHT20_BAUDRATE (400 * 1000)       //波特率
#define AHT20_I2C_IDX WIFI_IOT_I2C_IDX_0

MQueue mq;
float humidity;
float temperature;

void init(void)
{
    GpioInit();
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_13, WIFI_IOT_IO_FUNC_GPIO_13_I2C0_SDA);
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_14, WIFI_IOT_IO_FUNC_GPIO_14_I2C0_SCL);
    I2cInit(AHT20_I2C_IDX, AHT20_BAUDRATE);
}

void I2C_task(void *arg){
    (void)arg;
    init();
    // init_queue(&temp_humi_mq);
    // th_mq_id = osMessageQueueNew(256, sizeof(float), NULL);

//    SendPacket packet1;
    SendPacket packet2;

    while(WIFI_IOT_SUCCESS != AHT20_Calibrate()){
        printf("AHT20 sensor init failed!\r\n");
        usleep(1000);
    }
    while(1){
        AHT20_StartMeasure();
        AHT20_GetMeasureResult(&temperature, &humidity);
        // void* tp = (flaot *)malloc(sizeof(temperature));
        // void* hm = (flaot *)malloc(sizeof(humidity));
        // memcpy(tp, temperature);
        // memcpy(hm, humidity);
        // push_back(&temp_humi_mq, &temperature);
        // push_back(&temp_humi_mq, &humidity);
        
        // packet1.id = 0x1A;
        // packet1.data = &temperature;
        // push_back(&mq, &packet1);

        // packet2.id = 0x3A;
        // packet2.data = &humidity;
        // push_back(&mq, &packet2);
        
        packet2.id = 0x2B;
        // char data[128];
        sprintf(packet2.data,"{\"temp\": %.2f, \"humi\": %.2f}",temperature, humidity);
        push_back(&mq, &packet2);

        printf("{\"temp\": %.2f, \" humi\": %.2f}", temperature, humidity);
        
        sleep(1);
    }
}

float get_temp_data(void){
    return temperature;
}

float get_humi_data(void){
    return humidity;
}


void temp_humi_task(void){
    osThreadAttr_t attr;
    
    attr.name = "I2C_Task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = I2C_TASK_STACK_SIZE;
    attr.priority = osPriorityNormal;

    if (osThreadNew((osThreadFunc_t)I2C_task, NULL, &attr) == NULL)
    {
        printf("[EnvironmentDemo] Falied to create AhtDemoTask!\n");
    }
}



