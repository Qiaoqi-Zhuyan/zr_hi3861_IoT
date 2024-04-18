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

#include "gas_measure.h"
#include "temp_humi_measure.h"
#include "mqtt_task.h"
#include "oled_task.h"


/*
静态函数：固定函数的作用域，
*/
static void main_task(void *arg)
{
    (void)arg;
    init_queue(&mq);
    gas_task();
    temp_humi_task();
    mqtt_task();
    oled_task();
    //led_task();
   

}


// 在函数中创建新线程
static void main_task_thread(void)
{
    osThreadAttr_t attr;

    attr.name = "main_task_thread";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = 12;

    // 在新线程中执行函数mqtt_test_thread
    if (osThreadNew((osThreadFunc_t)main_task, NULL, &attr) == NULL)
    {
        printf("[LedExample] Falied to create LedTask!\n");
    }

    AT_RESPONSE_OK;
    // return HI_ERR_SUCCESS;
}

APP_FEATURE_INIT(main_task_thread);
