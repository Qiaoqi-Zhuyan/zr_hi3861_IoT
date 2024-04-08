// std
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// ohos
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifi_device.h"
#include "lwip/netifapi.h"
#include "lwip/api_shell.h"
#include <at.h>
#include <hi_at.h>

// proj
#include "src/module_tasks/spo2_heart_det/spo2_heart_det.h"
#include "src/msg_queue/mq.h"
#include "src/mqtt_io/mqtt_task.h"
// #include "src/msg_queue.h"


/*
静态函数：固定函数的作用域，
*/
static void main_task(void *arg)
{
    (void)arg;
    // init_queue(&mq);
    sleep(1);
    spo2_heart_task();
    sleep(1);
    mqtt_task();
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
    attr.priority = 36;

    // 在新线程中执行函数mqtt_test_thread
    if (osThreadNew((osThreadFunc_t)main_task, NULL, &attr) == NULL)
    {
        printf("[LedExample] Falied to create LedTask!\n");
    }

    AT_RESPONSE_OK;
    // return HI_ERR_SUCCESS;
}

APP_FEATURE_INIT(main_task_thread);
