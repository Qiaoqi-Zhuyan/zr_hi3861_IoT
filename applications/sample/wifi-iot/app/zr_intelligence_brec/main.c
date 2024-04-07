#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifi_device.h"
#include "lwip/netifapi.h"
#include "lwip/api_shell.h"
#include "wifi_utils.h"
#include "mqtt_utils.h"
#include <at.h>
#include <hi_at.h>

#define SSID "xiaoqi"
#define PASSWORD "qazplmg3323"
#define HOST_ADDR "192.168.203.202"


/*
mqtt 通讯任务
*/
static void mqtt_task(void *arg)
{
    (void)arg;
    connect_wifi(SSID, PASSWORD); // 连接WIFI热点
    printf("begin mqtt task\n");
    mqtt_connect(HOST_ADDR); // 运行mqtt测试程序
}
