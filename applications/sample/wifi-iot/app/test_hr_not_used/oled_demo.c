#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_pwm.h"
#include "wifiiot_adc.h"

#include "oled_ssd1306.h"
#include "hi_time.h"
#include "rtc_base.h"


#define ANALOG_KEY_CHAN_NAME WIFI_IOT_ADC_CHANNEL_2

// extern char *traffic_light_states[2];
static void OledTask(void *arg)
{
    (void)arg;
    GpioInit();
    OledInit();

    OledFillScreen(0x00);
    OledShowString(0, 0, "Hello, HarmonyOS", 1);
    sleep(1);

    OledFillScreen(0x00);
    // while (1) {
        // OledShowString(0, 3, traffic_light_states[0], 1);
        // OledShowString(0, 4, traffic_light_states[1], 1);
    //     usleep(30*1000);
    // }
}

static void OledDemo(void)
{
    osThreadAttr_t attr;

    attr.name = "OledTask";
    attr.attr_bits = 0U;
    attr.cb_mem = NULL;
    attr.cb_size = 0U;
    attr.stack_mem = NULL;
    attr.stack_size = 4096;
    attr.priority = osPriorityNormal;

    if (osThreadNew(OledTask, NULL, &attr) == NULL) {
        printf("[OledDemo] Falied to create OledTask!\n");
    }
    printf("\r\n[OledDemo] Succ to create OledTask!\n");
}

APP_FEATURE_INIT(OledDemo);
