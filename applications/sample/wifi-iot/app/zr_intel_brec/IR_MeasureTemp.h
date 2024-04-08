#ifndef __IR_MEASURETEMP_H_
#define __IR_MEASURETEMP_H_

/******************************************************************************\
                                 Includes
\******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <securec.h>
#include "mqtt_send_package.h"

/******************************************************************************\
                             Functions definitions
\******************************************************************************/

void ir_measure_temp_task(void);
extern SendPacket temp_packet;

#endif /* __IR_MEASURETEMP_H_ */