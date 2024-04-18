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


#define SSID "Eawen"
#define PASSWORD "testspeed900"

static void mqtt_thread(void *arg)
{
    (void)arg;
    connect_wifi(SSID, PASSWORD); // 连接WIFI热点
    printf("begin mqtt  demo\n");

    mqtt_connect(); // 运行mqtt测试程序
}

// 在函数中创建新线程
void mqtt_task(void)
{

    osThreadAttr_t attr;

    attr.name = "wifi_config_thread";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = 36;

    // 在新线程中执行函数mqtt_test_thread
    if (osThreadNew((osThreadFunc_t)mqtt_thread, NULL, &attr) == NULL)
    {
        printf("[LedExample] Falied to create LedTask!\n");
    }

    AT_RESPONSE_OK;
    // return HI_ERR_SUCCESS;
}
