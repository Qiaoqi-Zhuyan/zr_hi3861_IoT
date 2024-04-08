#ifndef __TEMP_HUMI_MEASURE_H__
#define __TEMP_HUMI_MEASURE_H__

#include "msg_queue/mq.h"
// extern MQueue temp_humi_mq;
#include "cmsis_os2.h"

// extern osMessageQueueId_t th_mq_id;

void temp_humi_task(void);

float get_temp_data(void);

float get_humi_data(void);


#endif /* __TEMP_HUMI_MEASURE_H__ */
