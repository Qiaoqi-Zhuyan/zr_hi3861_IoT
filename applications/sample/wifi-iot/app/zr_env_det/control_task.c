#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "oled_ssd1306.h"
#include "gas_measure.h"
#include "temp_humi_measure.h"
#include "control_task.h"

#define OLED_TASK_STACK_SIZE 4096
#define INTERVAL_TIME_US 500 * 1000
#define TASK_STACK_SIZE 512
#define TASK_PRIO 32


void dispinit(void)
{
    // oled screen init
    GpioInit();
    OledInit();
}

void led_control(void* arg){
    (void)arg;
    dispinit();
    OledFillScreen(0x00);       // 清屏
    OledShowString(0, 0, "Hello, HarmonyOS", 2);
    OledFillScreen(0x00);
    sleep(1);

    while(1){


        //oled 显示屏部分
        unsigned short gas_data = get_gas_data();
        float temp = get_temp_data();
        float humi = get_humi_data();

        char gas_str[64], temp_str[64], humi_str[64];
        sprintf(gas_str, "{\"gas\": %d}", gas_data);
        sprintf(temp_str, "{\"temperature\": %.2f}", temp);
        sprintf(humi_str, "{\"humidity\": %.2f}", humi);
        OledShowString(0, 0, gas_str, 2), sleep(2);
        OledFillScreen(0x00);
        OledShowString(0, 0, temp_str, 2), sleep(2);
        OledFillScreen(0x00);
        OledShowString(0, 0, humi_str, 2), sleep(2);
        OledFillScreen(0x00);







        // // 红灯
        // if(temp < 10.0f) GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_10, 1);
        // else GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_10, 0);

        // // 绿灯
        // if(temp > 28.0f) GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_11, 1);
        // else GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_11, 0);

        // // 黄灯
        // if(humi < 50.0f) GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_12, 1);
        // else GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_12, 0);

    }
}

void led_task(void)
{
    osThreadAttr_t attr;

    GpioInit();
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_10|WIFI_IOT_IO_NAME_GPIO_11|WIFI_IOT_IO_NAME_GPIO_12, WIFI_IOT_IO_FUNC_GPIO_10_GPIO|WIFI_IOT_IO_FUNC_GPIO_11_GPIO|WIFI_IOT_IO_FUNC_GPIO_12_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_10, WIFI_IOT_GPIO_DIR_OUT);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_11, WIFI_IOT_GPIO_DIR_OUT);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_12, WIFI_IOT_GPIO_DIR_OUT);

    printf("GpioInit OK!\n");

    attr.name = "led_task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = OLED_TASK_STACK_SIZE;
    attr.priority = TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)led_control, NULL, &attr) == NULL) 
    {
        printf("Falied to create led_task!\n");
    }
}