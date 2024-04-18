#ifndef ____GAS_MEASURE_H___
#define ____GAS_MEASURE_H___

#include "msg_queue/mq.h"
#include "cmsis_os2.h"

// extern MQueue gas_mq;
extern MQueue mq;
// extern osMessageQueueId_t gas_mq_id;

void gas_task(void);
unsigned short get_gas_data(void);

#endif /* ____GAS_MEASURE_H____ */
