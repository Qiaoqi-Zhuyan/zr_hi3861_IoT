#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"

#define INTERVAL_TIME_US 500 * 1000
#define TASK_STACK_SIZE 512
#define TASK_PRIO 25

void init(void)
{
    GpioInit();
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_10, WIFI_IOT_IO_FUNC_GPIO_10_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_10, WIFI_IOT_GPIO_DIR_OUT);

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_11, WIFI_IOT_IO_FUNC_GPIO_11_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_11, WIFI_IOT_GPIO_DIR_OUT);

    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_12, WIFI_IOT_IO_FUNC_GPIO_12_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_12, WIFI_IOT_GPIO_DIR_OUT);
}

static void *TrafficLightTask(const char *arg)
{
    (void)arg;
    while (1) 
    {
        // 绿灯
        GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_10, 1);
        usleep(INTERVAL_TIME_US);
        GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_10, 0);
        usleep(INTERVAL_TIME_US);

        // 黄灯
        GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_11, 1);
        usleep(INTERVAL_TIME_US);
        GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_11, 0);
        usleep(INTERVAL_TIME_US);

        // 红灯
        GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_12, 1);
        usleep(INTERVAL_TIME_US);
        GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_12, 0);
        usleep(INTERVAL_TIME_US);
    }
    return NULL;
}

static void TrafficLightExampleEntry(void)
{
    osThreadAttr_t attr;

    GpioInit();
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_10|WIFI_IOT_IO_NAME_GPIO_11|WIFI_IOT_IO_NAME_GPIO_12, WIFI_IOT_IO_FUNC_GPIO_10_GPIO|WIFI_IOT_IO_FUNC_GPIO_11_GPIO|WIFI_IOT_IO_FUNC_GPIO_12_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_10, WIFI_IOT_GPIO_DIR_OUT);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_11, WIFI_IOT_GPIO_DIR_OUT);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_12, WIFI_IOT_GPIO_DIR_OUT);

    printf("GpioInit OK!\n");

    attr.name = "TrafficLightTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = TASK_STACK_SIZE;
    attr.priority = TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)TrafficLightTask, NULL, &attr) == NULL) 
    {
        printf("Falied to create TrafficLightTask!\n");
    }
}

SYS_RUN(TrafficLightExampleEntry);