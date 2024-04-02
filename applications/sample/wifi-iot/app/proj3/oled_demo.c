#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "oled_ssd1306.h"
#include <stdint.h>
#include <string.h>
#include "wifiiot_adc.h"
#include "wifiiot_errno.h"
#include "aht20.h"
#include "wifiiot_i2c.h"
#include "wifiiot_errno.h"
#include "aht20.h"
#include "wifiiot_pwm.h"
#include "stdlib.h"

#define OLED_TASK_STACK_SIZE 4096
#define GAS_SENSOR_CHAN_NAME WIFI_IOT_ADC_CHANNEL_5
#define AHT20_BAUDRATE (400 * 1000)       //波特率

uint32_t retval = 0;
float humidity = 0.0f;
float temperature = 0.0f;
unsigned short data = 0;

void temp_humi_init(void){
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_13, WIFI_IOT_IO_FUNC_GPIO_13_I2C0_SDA);
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_14, WIFI_IOT_IO_FUNC_GPIO_14_I2C0_SCL);
    I2cInit(AHT20_I2C_IDX, AHT20_BAUDRATE);
}

void beeper_init(void){
    IoSetFunc(WIFI_IOT_IO_NAME_GPIO_9, WIFI_IOT_IO_FUNC_GPIO_9_PWM0_OUT);
    PwmInit(WIFI_IOT_PWM_PORT_PWM0);
}

void gas_task(void){
    if (AdcRead(GAS_SENSOR_CHAN_NAME, &data, WIFI_IOT_ADC_EQU_MODEL_4, WIFI_IOT_ADC_CUR_BAIS_DEFAULT, 0) == WIFI_IOT_SUCCESS){
            printf("gas:%d ppm\n", data);
    }
}

void init(void)
{
    GpioInit();
    OledInit();
    OledFillScreen(0x00);
    OledShowString(0, 0, "Hello, HarmonyOS", 2);

    temp_humi_init();
    beeper_init();
    
}

void temp_humi_task(void){
        // 发送 触发测量 命令，开始测量
    retval = AHT20_StartMeasure();
    if (retval != WIFI_IOT_SUCCESS){
        printf("trigger measure failed!\r\n");
    }
    else{
        retval = AHT20_GetMeasureResult(&temperature, &humidity);
            printf("temp: %.2f,  humi: %.2f\n", temperature, humidity);
    }
    // else{
    //     printf("temperature_humidity_task failed!\r\n");
    // }
}


static void proj_task(void *arg)
{
    (void)arg;
    init();
    sleep(1);
    uint16_t freqDivisor = 34052;
    while (1)
    {
        gas_task();
        sleep(1);
        temp_humi_task();
        sleep(1);
        if(data > 1000)
            PwmStart(WIFI_IOT_PWM_PORT_PWM0, freqDivisor / 2, freqDivisor);
        else
            PwmStop(WIFI_IOT_PWM_PORT_PWM0);
        
        char gas_str[64], temp_str[64], humi_str[64];
        sprintf(gas_str, "gas: %d ppm", data);
        sprintf(temp_str, "temperature: %.2f", temperature);
        sprintf(humi_str, "humidity: %.2f", humidity);
        OledShowString(0, 0, gas_str, 2), sleep(2);
        OledFillScreen(0x00);
        OledShowString(0, 0, temp_str, 2), sleep(2);
        OledFillScreen(0x00);
        OledShowString(0, 0, humi_str, 2), sleep(2);

    }

}

static void task_loop(void)
{
    osThreadAttr_t attr;
    attr.name = "proj_task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = OLED_TASK_STACK_SIZE;
    attr.priority = osPriorityNormal;
    if (osThreadNew(proj_task, NULL, &attr) == NULL)
    {
        printf("[OledDemo] Falied to create OledTask!\n");
    }
}

APP_FEATURE_INIT(task_loop);
