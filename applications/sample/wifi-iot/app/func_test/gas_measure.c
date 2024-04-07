#include "gas_measure.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "stdlib.h"


#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_adc.h"
#include "wifiiot_errno.h"

#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_pwm.h"
#include "hi_pwm.h"


#define ADC_TASK_STACK_SIZE 4096
#define GAS_SENSOR_CHAN_NAME WIFI_IOT_ADC_CHANNEL_5

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long int u32;
typedef signed char s8;
typedef signed short int s16;
typedef signed long int s32;

unsigned short data_length[10];
unsigned char i = 0;

unsigned short gas_data;


static void ADC_Task(void *arg)
{
    (void)arg;
    // gas_mq_id = osMessageQueueNew(256, 64, NULL);

    GpioInit();
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_IO_FUNC_GPIO_9_PWM0_OUT);
    PwmInit(WIFI_IOT_PWM_PORT_PWM0);
    uint16_t freqDivisor = 34052;
    init_queue(&gas_mq);

    while (1)
    {
        if (AdcRead(GAS_SENSOR_CHAN_NAME, &gas_data, WIFI_IOT_ADC_EQU_MODEL_4, WIFI_IOT_ADC_CUR_BAIS_DEFAULT, 0) == WIFI_IOT_SUCCESS)
        {
            // void* gs = (int *)malloc(sizeof(gas_data));
            // memcpy(gs, gas_data);
            push_back(&gas_mq, &gas_data);
            // osMessageQueuePut(gas_mq_id, &gas_data, 31U, 1U);
            printf("{\"gas\": %d }\n", gas_data);
        }

        if (gas_data > 1000)
            PwmStart(WIFI_IOT_PWM_PORT_PWM0, freqDivisor / 2, freqDivisor);
        else
            PwmStop(WIFI_IOT_PWM_PORT_PWM0);

        sleep(1);
    }
}

unsigned short get_gas_data(void){
    return gas_data;
}

void gas_task(void)
{
    osThreadAttr_t attr;
    attr.name = "ADC_Task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = ADC_TASK_STACK_SIZE;
    attr.priority = osPriorityNormal;
    if (osThreadNew((osThreadFunc_t)ADC_Task, NULL, &attr) == NULL)
    {
        printf("[EnvironmentDemo] Falied to create EnvironmentTask!\n");
    }
}
