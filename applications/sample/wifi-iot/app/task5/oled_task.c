#include <stdio.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "oled_ssd1306.h"
#include "oled_task.h"
#include "gas_measure.h"
#include "temp_humi_measure.h"

#define OLED_TASK_STACK_SIZE 4096

void oled_init(void)
{
    GpioInit();
    OledInit();
}

static void oled_disp(void *arg)
{
    (void)arg;
    oled_init();
    OledFillScreen(0x00);       // 清屏
    OledShowString(0, 0, "Hello, HarmonyOS", 2);
    sleep(1);

    while(1){

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
    }
}

void oled_task(void)
{
    osThreadAttr_t attr;
    attr.name = "oled_task";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = OLED_TASK_STACK_SIZE;
    attr.priority = osPriorityNormal;
    if (osThreadNew(oled_disp, NULL, &attr) == NULL)
    {
        printf("[OledDemo] Falied to create OledTask!\n");
    }
}