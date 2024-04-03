/******************************************************************************\
                                 Includes
\******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <securec.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "ohos_types.h"
#include "wifiiot_errno.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_adc.h"
#include "Table.h"

/******************************************************************************\
                             Variables definitions
\******************************************************************************/
#define IR_TEMP_TASK_STACK_SIZE             (1024*4)                //任务栈大小
#define IR_TEMP_TASK_PRIO                   (10)                    //任务优先等级

#define NTC_ADC_CHANNEL                     WIFI_IOT_ADC_CHANNEL_5        //NTC脚位
#define THERMOPILE_1_ADC_CHANNEL            WIFI_IOT_ADC_CHANNEL_2        //热电堆+脚位
#define REFERENCE_ADC_CHANNEL               WIFI_IOT_ADC_CHANNEL_0        //参考电平

static uint8_t s_Temp;

float convert_to_voltage(uint16_t data)
{
    float voltage = (float)data * 1.8 * 4 / 4096; /* adc code equals: voltage/4/1.8*4096  */
    return voltage;
}

static char Measure_Temp(WifiIotAdcChannelIndex channel, uint16_t *Temp)
{
    char ret = 0;
    uint16_t data;

    if( AdcRead(channel, &data, WIFI_IOT_ADC_EQU_MODEL_8, WIFI_IOT_ADC_CUR_BAIS_DEFAULT, 0) == WIFI_IOT_SUCCESS )
    {
        *Temp = (uint16_t)(convert_to_voltage(data) * 10000);  //ad值转换成电压值
        // printf("\r\nvoltage = %d\n", *Temp);
        ret = 0;                                    //测量成功
    }
    else
    {
        printf("Measure temp fales!!!\n");
        ret = -1;                                   //测量失败                                   
    }
    return ret;
}

void Ir_Task(void *arg) 
{
    (void)arg;
    uint16_t nr_volt_1,nr_volt_2, ntc_volt, volt_diff, base_volt;
    uint8_t i;
    while(1)
    {
        for (i = 0; i < 40; i++)
        {
            Measure_Temp(THERMOPILE_1_ADC_CHANNEL, &nr_volt_1);
            usleep(1000 * 50);
        }
        // printf("\r\n[Ir_Task]nr_volt_1 = %d\n", nr_volt_1);

        for (i = 0; i < 40; i++)
        {
            Measure_Temp(THERMOPILE_1_ADC_CHANNEL, &nr_volt_2);
            Measure_Temp(NTC_ADC_CHANNEL, &ntc_volt);
            usleep(1000 * 50);
        }
        // printf("\r\n[Ir_Task]nr_volt_2 = %d\n", nr_volt_2);

        if (nr_volt_2 > nr_volt_1)
        {
            volt_diff = nr_volt_2 - nr_volt_1;
        }
        else
        {
            volt_diff = nr_volt_1 - nr_volt_2;
        }
        // printf("\r\n[Ir_Task]volt_diff = %d\n", volt_diff);

        Measure_Temp(REFERENCE_ADC_CHANNEL, &base_volt);
        Temperature_Transverter(ntc_volt / 10, volt_diff, &s_Temp);
        printf(" s_Temp = %d\n",s_Temp);
    }
}

void Ir_Measure_Temp_Config(void)
{
    osThreadAttr_t attr;

    attr.name       = "Ir-Temp-Task";
    attr.attr_bits  = 0U;
    attr.cb_mem     = NULL;
    attr.cb_size    = 0U;
    attr.stack_mem  = NULL;
    attr.stack_size = IR_TEMP_TASK_STACK_SIZE;
    attr.priority   = IR_TEMP_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)Ir_Task, NULL, &attr) == NULL) 
    {
        printf("\r\n [Ir_Measure_Temp_Config] Falied to create Ir-Task!\n");
    }
}

APP_FEATURE_INIT(Ir_Measure_Temp_Config);

/******************************* End of File (C) ******************************/