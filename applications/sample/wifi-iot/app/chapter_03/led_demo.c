#include <stdio.h>
#include "ohos_init.h"
#include <unistd.h>
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "ohos_types.h"
void leddemo(void)
{
    GpioInit(); //初始化GPIO设备
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_IO_FUNC_GPIO_9_GPIO);
    GpioSetDir(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_DIR_OUT);
    GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_EDGE_FALL_LEVEL_LOW);
    printf("led 点亮\n");

    usleep(4000000);
    GpioSetOutputVal(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_GPIO_EDGE_RISE_LEVEL_HIGH);
    printf("led 熄灭\n");
   
}
SYS_RUN(leddemo);