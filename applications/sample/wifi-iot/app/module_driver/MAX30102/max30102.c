/******************************************************************************\
**  版    权 :  深圳开鸿数字产业发展有限公司（2021）
**  文件名称 :  max30102.c
**  功能描述 :  max30102 心率血氧传感器驱动
**  作    者 :  王滨泉
**  日    期 :  2021.12.17
**  版    本 :  V0.0.1
**  变更记录 :  V0.0.1/2021.12.17
                1 首次创建                 
\******************************************************************************/

/******************************************************************************\
                                 Includes
\******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <securec.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "ohos_types.h"
#include "iot_errno.h"
#include "iot_gpio.h"
#include "iot_i2c.h"
#include "max30102.h"
#include "algorithm.h"
/******************************************************************************\
                             Variables definitions
\******************************************************************************/
#define MAX30102_TASK_STACK_SIZE     4096               //任务栈大小
#define MAX30102_TASK_PRIO           10                 //任务优先等级
#define MAX30102_I2C_IDX             0                  //I2C设备号
#define MAX30102_I2C_BAUDRATE        (200*1000)         //I2C波特率
#define MAX30102_ADDR                0x57               //MAX30102设备地址

#define MAX30102_INT_GPIO           10                  //MAX30102中断引脚

//register addresses
#define REG_INTR_STATUS_1           0x00
#define REG_INTR_STATUS_2           0x01
#define REG_INTR_ENABLE_1           0x02
#define REG_INTR_ENABLE_2           0x03
#define REG_FIFO_WR_PTR             0x04
#define REG_OVF_COUNTER             0x05
#define REG_FIFO_RD_PTR             0x06
#define REG_FIFO_DATA               0x07
#define REG_FIFO_CONFIG             0x08
#define REG_MODE_CONFIG             0x09
#define REG_SPO2_CONFIG             0x0A
#define REG_LED1_PA                 0x0C
#define REG_LED2_PA                 0x0D
#define REG_PILOT_PA                0x10
#define REG_MULTI_LED_CTRL1         0x11
#define REG_MULTI_LED_CTRL2         0x12
#define REG_TEMP_INTR               0x1F
#define REG_TEMP_FRAC               0x20
#define REG_TEMP_CONFIG             0x21
#define REG_PROX_INT_THRESH         0x30
#define REG_REV_ID                  0xFE
#define REG_PART_ID                 0xFF


#define MAX_BRIGHTNESS              255



uint32_t aun_ir_buffer[500]; //IR LED sensor data
int32_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int32_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int32_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;
uint8_t s_Hr=0,s_Spo2=0;
/******************************************************************************\
                             Functions definitions
\******************************************************************************/
/*
* 函数名称 : maxim_max30102_write_reg
* 功能描述 : max30102写寄存器
* 参    数 : uch_addr   - 寄存器地址
             uch_data   - 数据内容
* 返回值   : 读取结果
* 示    例 : maxim_max30102_write_reg(uch_addr,uch_data);
*/
/******************************************************************************/ 
static unsigned int maxim_max30102_write_reg(uint8 uch_addr, uint8 uch_data)
/******************************************************************************/ 
{
    uint8 buffer[2]={uch_addr,uch_data};
    if(IoTI2cWrite(MAX30102_I2C_IDX,(MAX30102_ADDR<<1)|0,buffer,ARRAY_SIZE(buffer))){
        printf("max30102 write falled.\n");
    }
    return IOT_SUCCESS;
}

/*
* 函数名称 : maxim_max30102_read_reg
* 功能描述 : max30102读寄存器
* 参    数 : uch_addr   - 寄存器地址
             Data       - 数据指针
             len        - 数据长度
* 返回值   : 读取结果
* 示    例 : maxim_max30102_read_reg(uch_addr,&data,len);
*/
/******************************************************************************/ 
static unsigned int maxim_max30102_read_reg(uint8 uch_addr, uint8 *data, uint16 len)
/******************************************************************************/ 
{
    unsigned int ret=0;
    ret+=IoTI2cWrite(MAX30102_I2C_IDX,(MAX30102_ADDR<<1)|0,&uch_addr,1);
    ret+=IoTI2cRead(MAX30102_I2C_IDX,(MAX30102_ADDR<<1)|1,data,len);
    if(ret)
        printf("max30102 read falled.\n");
    return ret;
}
/*
* 函数名称 : max30102_FIFO_ReadBytes
* 功能描述 : max30102 fifo数据读取
* 参    数 : Register_Address - 寄存器地址
             Data - 数据指针
* 返回值   : 无
* 示    例 : max30102_FIFO_ReadBytes();
*/
/******************************************************************************/ 
void max30102_FIFO_ReadBytes(uint8_t Register_Address,uint8_t *Data)
/******************************************************************************/ 
{	
    uint8_t uch_temp;
    uint8_t buf[6];
    unsigned int ret=0;

    ret+=maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp, sizeof(uch_temp));
    ret+=maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp, sizeof(uch_temp));
    ret+=maxim_max30102_read_reg(REG_FIFO_DATA, Data, 6);
    if (ret)
    {
         printf("max30102 read fifo failed.\n");
    }


}
/*
* 函数名称 : max30102_reset
* 功能描述 : max30102软件复位
* 参    数 : 无
* 返回值   : 无
* 示    例 : max30102_reset();
*/
/******************************************************************************/ 
void max30102_reset(void)
/******************************************************************************/ 
{
	maxim_max30102_write_reg(REG_MODE_CONFIG,0x40);
	maxim_max30102_write_reg(REG_MODE_CONFIG,0x40);
}
/*
* 函数名称 : max30102_init
* 功能描述 : max30102初始化
* 参    数 : 无
* 返回值   : 无
* 示    例 : max30102_init();
*/
/******************************************************************************/ 
void max30102_init(void)
/******************************************************************************/ 
{
	IoTI2cInit(MAX30102_I2C_IDX, MAX30102_I2C_BAUDRATE);    //I2C初始化
    
	max30102_reset();
	
//	max30102_Bus_Write(REG_MODE_CONFIG, 0x0b);  //mode configuration : temp_en[3]      MODE[2:0]=010 HR only enabled    011 SP02 enabled
//	max30102_Bus_Write(REG_INTR_STATUS_2, 0xF0); //open all of interrupt
//	max30102_Bus_Write(REG_INTR_STATUS_1, 0x00); //all interrupt clear
//	max30102_Bus_Write(REG_INTR_ENABLE_2, 0x02); //DIE_TEMP_RDY_EN
//	max30102_Bus_Write(REG_TEMP_CONFIG, 0x01); //SET   TEMP_EN

//	max30102_Bus_Write(REG_SPO2_CONFIG, 0x47); //SPO2_SR[4:2]=001  100 per second    LED_PW[1:0]=11  16BITS

//	max30102_Bus_Write(REG_LED1_PA, 0x47); 
//	max30102_Bus_Write(REG_LED2_PA, 0x47); 
	
	
	
	maxim_max30102_write_reg(REG_INTR_ENABLE_1,0xc0);	// INTR setting
	maxim_max30102_write_reg(REG_INTR_ENABLE_2,0x00);
	maxim_max30102_write_reg(REG_FIFO_WR_PTR,0x00);  	//FIFO_WR_PTR[4:0]
	maxim_max30102_write_reg(REG_OVF_COUNTER,0x00);  	//OVF_COUNTER[4:0]
	maxim_max30102_write_reg(REG_FIFO_RD_PTR,0x00);  	//FIFO_RD_PTR[4:0]
	maxim_max30102_write_reg(REG_FIFO_CONFIG,0x0f);  	//sample avg = 1, fifo rollover=false, fifo almost full = 17
	maxim_max30102_write_reg(REG_MODE_CONFIG,0x03);  	//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	maxim_max30102_write_reg(REG_SPO2_CONFIG,0x27);  	// SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)  
	maxim_max30102_write_reg(REG_LED1_PA,0x24);   	//Choose value for ~ 7mA for LED1
	maxim_max30102_write_reg(REG_LED2_PA,0x24);   	// Choose value for ~ 7mA for LED2
	maxim_max30102_write_reg(REG_PILOT_PA,0x7f);   	// Choose value for ~ 25mA for Pilot LED											
}

/*
* 函数名称 : Get_Max30102_HeartRate
* 功能描述 : 获取心率
* 参    数 : 无
* 返回值   : 心率
* 示    例 : HeartRate = Get_Max30102_HeartRate();
*/
/******************************************************************************/ 
uint8_t Get_Max30102_HeartRate(void)
/******************************************************************************/
{
    return s_Hr;                        //传递心率值
}
/*
* 函数名称 : Get_Max30102_SpO2
* 功能描述 : 获取检测温度
* 参    数 : 无
* 返回值   : 血氧饱和度
* 示    例 : s_Spo2 = Get_Max30102_SpO2();
*/
/******************************************************************************/ 
uint8_t Get_Max30102_SpO2(void)
/******************************************************************************/
{
    return s_Spo2;                      //传递血氧饱和度值
}

/*
* 函数名称 : MAX30102Task
* 功能描述 : MAX30102任务，计算出心率血氧值(使用算法粗糙，心率血氧值不准确，只能演示)
* 参    数 : 无
* 返回值   : 无
* 示    例 : MAX30102Task();
*/
/******************************************************************************/ 
void MAX30102Task(void *arg)
/******************************************************************************/ 
{
    (void)arg;
    IotGpioValue value;
    uint8_t id;
    uint32_t un_min, un_max, un_prev_data;  
	int i;
	int32_t n_brightness;
	float f_temp;
	uint8_t temp[6];
	
    static uint8_t dis_hr_bank = 0; 
    static uint8_t dis_spo2_bank = 0;
    static uint8_t cnt = 0;
    static bool finger = false;

    IoTGpioInit(MAX30102_INT_GPIO);
    IoTGpioSetDir(MAX30102_INT_GPIO, IOT_GPIO_DIR_IN);
    
    maxim_max30102_read_reg(REG_PART_ID,&id,1);
    printf("\r\n id = %d \r\n",id);

    max30102_init();

    un_min=0x3FFFF;
	un_max=0;
	
	n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps
	//read the first 500 samples, and determine the signal range
    for(i=0;i<n_ir_buffer_length;i++)
    {
        do{
            IoTGpioGetInputVal(MAX30102_INT_GPIO,&value);
        }while(value == IOT_GPIO_VALUE1);
        // while(MAX30102_INT==1);   //wait until the interrupt pin asserts
        
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
		aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
            
        if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];    //update signal min
        if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];    //update signal max
    }
	un_prev_data=aun_red_buffer[i];
	//calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	
	while(1)
	{
		i=0;
        un_min=0x3FFFF;
        un_max=0;
		
		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            //update the signal min and max
            if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];
            if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];
        }
		//take 100 sets of samples before calculating the heart rate.
        for(i=400;i<500;i++)
        {
            un_prev_data=aun_red_buffer[i-1];
            // while(MAX30102_INT==1);
            do{
                IoTGpioGetInputVal(MAX30102_INT_GPIO,&value);
            }while(value == IOT_GPIO_VALUE1);
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
			aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
            if(aun_ir_buffer[i] < 10000)
            {
                break;
            }
            if(aun_red_buffer[i]>un_prev_data)
            {
                f_temp=aun_red_buffer[i]-un_prev_data;
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness-=(int)f_temp;
                if(n_brightness<0)
                    n_brightness=0;
            }
            else
            {
                f_temp=un_prev_data-aun_red_buffer[i];
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness+=(int)f_temp;
                if(n_brightness>MAX_BRIGHTNESS)
                    n_brightness=MAX_BRIGHTNESS;
            }
			//send samples and calculation result to terminal program through UART
			if(ch_hr_valid == 1 && n_heart_rate<120 && n_heart_rate > 0)//**/ ch_hr_valid == 1 && ch_spo2_valid ==1 && n_heart_rate<120 && n_sp02<101
			{
				// s_Hr = n_heart_rate;
				// s_Spo2 = n_sp02;
                dis_hr_bank = n_heart_rate;
				dis_spo2_bank = n_sp02;

                s_Hr = dis_hr_bank;
                s_Spo2 = dis_spo2_bank;

			}
			else
			{
				// s_Hr = 0;
				// s_Spo2 = 0;
                dis_hr_bank = 0;
                dis_spo2_bank = 0;
			}
            // printf("\r\nHR = %i, SpO2 = %i\r\n", n_heart_rate, n_sp02);
				// printf("HR=%i, ", n_heart_rate); 
				// printf("HRvalid=%i, ", ch_hr_valid);
				// printf("SpO2=%i, ", n_sp02);
				// printf("SPO2Valid=%i\r\n", ch_spo2_valid); 
		}
        if(i >= 500)
        {
            finger = true;
            maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
            printf("\r\nHR = %d, SpO2 = %d\r\n", s_Hr, s_Spo2);
        }
        else
        {
            s_Hr = 0;
            s_Spo2 = 0;
            if(finger == true)
            {
                finger = false;
                printf("\r\nHR = %d, SpO2 = %d, no finger!!! \r\n", s_Hr, s_Spo2);
            }
        }
        // if((dis_hr_bank==0) && (dis_hr_bank==0))
        // if((dis_hr_bank==0) && (dis_hr_bank==0))
        // {
        //     if(++cnt > 10)
        //     {
        //         cnt = 0;
        //         s_Hr = 0;
        //         s_Spo2 = 0;
        //     }
        // }
        // else
        // {
        //     cnt = 0;
        // }
        // printf("\r\nHR = %d, SpO2 = %d\r\n", s_Hr, s_Spo2);



        usleep(100); 
    }

}



/*
* 函数名称 : MAX30102Demo
* 功能描述 : MAX30102应用demo
* 参    数 : 空
* 返回值   : 空
* 示    例 : MAX30102Demo();
*/
/******************************************************************************/
void MAX30102Demo(void)
/******************************************************************************/
{
    osThreadAttr_t attr;

    attr.name       = "MAX30102Task";
    attr.attr_bits  = 0U;
    attr.cb_mem     = NULL;
    attr.cb_size    = 0U;
    attr.stack_mem  = NULL;
    attr.stack_size = MAX30102_TASK_STACK_SIZE;
    attr.priority   = MAX30102_TASK_PRIO;

    if (osThreadNew((osThreadFunc_t)MAX30102Task, NULL, &attr) == NULL) {
        printf("\r\n[MAX30102Demo] Falied to create MAX30102Task!\n");
    }else{
        printf("\r\n[MAX30102Demo] Succ to create MAX30102Task!\n");
    }
}

APP_FEATURE_INIT(MAX30102Demo);
/******************************* End of File (C) ******************************/