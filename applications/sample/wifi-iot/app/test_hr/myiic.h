#ifndef __MYIIC_H_
#define __MYIIC_H_
//#include "sys.h"

#include <stdio.h>
#include <unistd.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "wifiiot_gpio.h"
#include "wifiiot_gpio_ex.h"
#include "hi_gpio.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//IIC 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/10 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  

#define u8  unsigned char
#define u16 unsigned short

#if 0   	   		   
//IO方向设置
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=4;}	
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=7;}

//IO操作函数	 
#define IIC_SCL    PBout(7) //SCL
#define IIC_SDA    PBout(8) //SDA	 
#define READ_SDA   PBin(8)  //输入SDA 
 
#else

#define PIN_SCL		WIFI_IOT_IO_NAME_GPIO_14
#define PIN_SDA		WIFI_IOT_IO_NAME_GPIO_13
#define PIN_INT		WIFI_IOT_IO_NAME_GPIO_10

//IO方向设置
#define SDA_IN()  			GpioSetDir(PIN_SDA, WIFI_IOT_GPIO_DIR_IN);   
#define SDA_OUT() 			GpioSetDir(PIN_SDA, WIFI_IOT_GPIO_DIR_OUT);  

//IO操作函数	 
#define IIC_SCL(value)    	iic_scl(value)
#define IIC_SDA(value)    	iic_sda(value)
#define READ_SDA          	read_sda()
#endif

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
void IIC_Read_One_Byte(u8 daddr,u8 addr,u8* data);

void IIC_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength);
void IIC_ReadBytes(u8 deviceAddr, u8 writeAddr,u8* data,u8 dataLength);

void delay_ms(int32_t t);
void iic_scl(WifiIotGpioValue value);
void iic_sda(WifiIotGpioValue value);
#endif
















