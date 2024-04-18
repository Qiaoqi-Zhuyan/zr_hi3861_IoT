#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "oled_ssd1306.h"
#include "oled_task.h"
#include "gas_measure.h"
#include "temp_humi_measure.h"

#define OLED_TASK_STACK_SIZE 4096

#define INTERVAL_TIME_US 500 * 1000
#define TASK_STACK_SIZE 512
#define TASK_PRIO 25



void oled_init(void)
{
    // oled screen init
    GpioInit();

    // led light init
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_10|WIFI_IOT_IO_NAME_GPIO_11|WIFI_IOT_IO_NAME_GPIO_12, WIFI_IOT_IO_FUNC_GPIO_10_GPIO|WIFI_IOT_IO_FUNC_GPIO_11_GPIO|WIFI_IOT_IO_FUNC_GPIO_12_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_10, WIFI_IOT_GPIO_DIR_OUT);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_11, WIFI_IOT_GPIO_DIR_OUT);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_12, WIFI_IOT_GPIO_DIR_OUT);
}


static void oled_disp(void *arg)
{
    (void)arg;
    oled_init();
    air_condition = 0;
    humidifier = 0;
    temperature_change = 0;


    while(1){

        
        // led light 
        
        // 红灯 空调开关
        if(air_condition == 1)
            GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_10, 1);
        if(air_condition == 0) 
            GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_10, 0);

        // 调节温度 绿灯闪烁
        if(temperature_change == 1){
            GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_11, 1);
            usleep(INTERVAL_TIME_US);
            GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_11, 0);
            usleep(INTERVAL_TIME_US);
            temperature_change = 0;
        }

        // 黄灯 加湿机开关
        if(humidifier == 1) GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_12, 1);
        if(humidifier == 0) GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_12, 0);
        sleep(1);


    }

  
}

void oled_task(void)
{
    osThreadAttr_t attr;
    attr.name = "led_task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = OLED_TASK_STACK_SIZE;
    attr.priority = osPriorityNormal;
    if (osThreadNew(oled_disp, NULL, &attr) == NULL)
    {
        printf("[OledDemo] Falied to create OledTask!\n");
    }
}