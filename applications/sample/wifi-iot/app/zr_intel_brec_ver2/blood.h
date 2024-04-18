#ifndef _BLOOD_H
#define _BLOOD_H

#include "MAX30102.h"
#include "algorithm.h"
#include "mq.h"
#include "mqtt_send_package.h"
extern MQueue mq;
extern SendPacket blood_packet;


typedef enum
{
	BLD_NORMAL,		//正常
	BLD_ERROR,		//侦测错误
	
}BloodState;//血液状态

typedef struct
{
	int8_t 		heart;		//心率数据
	float 			SpO2;			//血氧数据
}BloodData;


extern void blood_data_translate(void);
extern void blood_data_update(void);
extern void blood_Loop(void);

#endif


