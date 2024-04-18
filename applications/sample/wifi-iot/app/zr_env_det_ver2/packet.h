#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdint.h"

/**
 * id: 
 * 0x1A: 温度 temperature:uint8_t
 * 0x2A: gas值 spo2:float
 * 0x3A: 湿度 heart:int
 * 
*/
typedef struct mqtt_send_package{
    uint8_t id;
    char data[128];
}SendPacket;

