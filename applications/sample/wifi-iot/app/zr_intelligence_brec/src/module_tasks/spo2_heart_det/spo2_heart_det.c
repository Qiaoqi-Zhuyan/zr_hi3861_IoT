#include <stdio.h>
#include <unistd.h>
#include <securec.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "wifiiot_i2c.h"
#include "hos_types.h"
#include "MAX30102.h"
#include "blood.h"

#include "spo2_heart_det.h"

void spo2_heart_task(void)
{
	osThreadAttr_t attr;

	attr.name = "spo2_heart_task";
	attr.attr_bits = 0U;
	attr.cb_mem = NULL;
	attr.cb_size = 0U;
	attr.stack_mem = NULL;
	attr.stack_size = MAX30102_TASK_STACK_SIZE;
	attr.priority = MAX30102_TASK_PRIO;

	if (osThreadNew((osThreadFunc_t)MAX30102Task, NULL, &attr) == NULL)
	{
		printf("\r\n[MAX30102Demo] Falied to create MAX30102Task!\n");
	}
}
