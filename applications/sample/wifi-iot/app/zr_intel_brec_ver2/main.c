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
#include "spo2_heart_det.h"
// #include "mq.h"
#include "mqtt_task.h"
#include "blood.h"
#include "IR_MeasureTemp.h"
#include "oled_task.h"
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
    ir_measure_temp_task();
    sleep(1);
    mqtt_task();
    oled_task();
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
