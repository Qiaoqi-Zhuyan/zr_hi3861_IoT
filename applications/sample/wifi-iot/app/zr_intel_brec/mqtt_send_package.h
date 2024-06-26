#ifndef __MQTT_SEND_PACKAGE_H__
#define __MQTT_SEND_PACKAGE_H__


#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdint.h"

/**
 * id: 
 * 0x4A: 体温 temperature:uint8_t
 * 0x5A: 血氧 spo2:float
 * 0x6A: 心率 heart:int
 * 
*/
typedef struct mqtt_send_package{
    int8_t id;
    char data[128];
}SendPacket;


#endif /* __MQTT_SEND_PACKAGE_H__ */
