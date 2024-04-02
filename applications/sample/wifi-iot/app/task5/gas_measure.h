#ifndef ____GAS_MEASURE_H___
#define ____GAS_MEASURE_H___

#include "mq.h"

extern MQueue gas_mq;
void gas_task(void);
unsigned short get_gas_data(void);

#endif /* ____GAS_MEASURE_H____ */
