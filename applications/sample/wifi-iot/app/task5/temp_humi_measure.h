#ifndef __TEMP_HUMI_MEASURE_H__
#define __TEMP_HUMI_MEASURE_H__

#include "mq.h"
extern MQueue temp_humi_mq;

void temp_humi_task(void);

float get_temp_data(void);

float get_humi_data(void);


#endif /* __TEMP_HUMI_MEASURE_H__ */
