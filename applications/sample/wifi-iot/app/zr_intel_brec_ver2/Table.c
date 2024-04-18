
/******************************************************************************\
                                 Includes
\******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <securec.h>

#include "ohos_init.h"
#include "cmsis_os2.h"
#include "ohos_types.h"
#include "wifiiot_errno.h"
#include "wifiiot_gpio.h"

/******************************************************************************\
                             Variables definitions
\******************************************************************************/
#define TABLE_FILTER_NUM        (4)


typedef enum
{
    TAB_OK          =  0,
    TAB_ERROR_MIN   = -1,
    TAB_ERROR_MAX   = -2,
}TE_TAB_ERROR;

//查询NTC表格(温度对应电压值(放大1000倍))
static const uint16_t s_NtcTableData[]={
    955,//0℃ 
    944,//1℃ 
    932,//2℃ 
    920,//3℃ 
    908,//4℃ 
    896,//5℃ 
    883,//6℃ 
    870,//7℃ 
    857,//8℃ 
    844,//9℃ 
    831,//10℃ 
    818,//11℃ 
    804,//12℃ 
    791,//13℃ 
    777,//14℃ 
    763,//15℃ 
    749,//16℃ 
    735,//17℃ 
    722,//18℃ 
    708,//19℃ 
    694,//20℃ 
    680,//21℃ 
    666,//22℃ 
    652,//23℃ 
    638,//24℃ 
    625,//25℃ 
    611,//26℃ 
    597,//27℃ 
    584,//28℃ 
    570,//29℃ 
    557,//30℃ 
    544,//31℃ 
    531,//32℃ 
    518,//33℃ 
    506,//34℃ 
    493,//35℃ 
    481,//36℃ 
    469,//37℃ 
    457,//38℃ 
    445,//39℃ 
    433,//40℃ 
    422,//41℃ 
    411,//42℃ 
    400,//43℃ 
    389,//44℃ 
    379,//45℃ 
    369,//46℃ 
    358,//47℃ 
    349,//48℃ 
    339,//49℃ 
    330,//50℃ 
    320,//51℃ 
    311,//52℃ 
    303,//53℃ 
    294,//54℃ 
    286,//55℃ 
    278,//56℃ 
    270,//57℃ 
    262,//58℃ 
    254,//59℃ 
    247,//60℃ 
    240,//61℃ 
    233,//62℃ 
    226,//63℃ 
    220,//64℃ 
    213,//65℃ 
    207,//66℃ 
    201,//67℃ 
    195,//68℃ 
    190,//69℃ 
    184,//70℃ 
    179,//71℃ 
    174,//72℃ 
    168,//73℃ 
    164,//74℃ 
    159,//75℃ 
    154,//76℃ 
    150,//77℃ 
    145,//78℃ 
    141,//79℃ 
    137,//80℃ 
    133,//81℃ 
    129,//82℃ 
    126,//83℃ 
    122,//84℃ 
    118,//85℃ 
    115,//86℃ 
    112,//87℃ 
    108,//88℃ 
    105,//89℃ 
    102,//90℃ 
    99 ,//91℃ 
    97 ,//92℃ 
    94 ,//93℃ 
    91 ,//94℃ 
    89 ,//95℃ 
    86 ,//96℃ 
    84 ,//97℃ 
    81 ,//98℃ 
    79 ,//99℃ 
    77 ,//100℃ 
};
//热电堆电压差值(mv)，放大10000倍
static const uint16_t s_VoltDiffTableData[21][26]=
{
    //20~45℃
    //NTC 20℃
    {0, 726, 1460, 2203, 2953, 3712, 4478, 5253, 6037, 6828, 7628, 8436, 9252, 10077, 10910, 11752, 12602, 13461, 14328, 15204, 16088, 16982, 17884, 18794, 19714, 20642},
    //NTC 21℃
    {0,   0,  734, 1476, 2227, 2985, 3752, 4527, 5310, 6102, 6901, 7709, 8526,  9350, 10184, 11025, 11876, 12734, 13602, 14478, 15362, 16255, 17157, 18068, 18987, 19915},
    //NTC 22℃
    {0,   0,    0,  742, 1492, 2251, 3018, 3793, 4576, 5367, 6167, 6975, 7791,  8616,  9449, 10291, 11141, 12000, 12867, 13743, 14628, 15521, 16423, 17333, 18253, 19181},
    //NTC 23℃
    {0,   0,    0,    0,  750, 1509, 2275, 3050, 3833, 4625, 5424, 6233, 7049,  7874,  8707,  9549, 10399, 11258, 12125, 13001, 13885, 14778, 15680, 16591, 17510, 18439},
    //NTC 24℃
    {0,   0,    0,    0,    0,  758, 1525, 2300, 3083, 3874, 4674, 5482, 6298,  7123,  7956,  8798,  9648, 10507, 11374, 12250, 13135, 14028, 14930, 15841, 16760, 17688},
    //NTC 25℃
    {0,   0,    0,    0,    0,    0,  766, 1541, 2324, 3116, 3915, 4723, 5540,  6365,  7198,  8040,  8890,  9748, 10616, 11492, 12376, 13269, 14171, 15082, 16001, 16930},
    //NTC 26℃
    {0,   0,    0,    0,    0,    0,    0,  774, 1558, 2349, 3149, 3957, 4773,  5598,  6431,  7273,  8123,  8982,  9849, 10725, 11609, 12503, 13405, 14315, 15235, 16163},
    //NTC 27℃
    {0,   0,    0,    0,    0,    0,    0,    0,  783, 1574, 2374, 3182, 3998,  4823,  5656,  6498,  7348,  8207,  9074,  9950, 10835, 11728, 12630, 13540, 14460, 15388},
    //NTC 28℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,  791, 1591, 2399, 3215,  4040,  4873,  5715,  6565,  7424,  8291,  9167, 10051, 10945, 11846, 12757, 13677, 14605},
    //NTC 29℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,  799, 1607, 2424,  3248,  4082,  4923,  5774,  6632,  7500,  8375,  9260, 10153, 11055, 11966, 12885, 13813},
    //NTC 30℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  808, 1624,  2449,  3282,  4124,  4974,  5833,  6700,  7576,  8460,  9353, 10255, 11166, 12085, 13014},
    //NTC 31℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,  816,  1641,  2474,  3316,  4166,  5025,  5892,  6768,  7652,  8545,  9447, 10358, 11277, 12206},
    //NTC 32℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,   824,  1658,  2499,  3349,  4208,  5075,  5951,  6836,  7729,  8631,  9542, 10461, 11389},
    //NTC 33℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,     0,   833,  1674,  2525,  3383,  4251,  5127,  6011,  6904,  7806,  8717,  9636, 10564},
    //NTC 34℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,     0,     0,   841,  1691,  2550,  3417,  4293,  5178,  6071,  6973,  7884,  8803,  9731},
    //NTC 35℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,     0,     0,     0,   850,  1708,  2576,  3452,  4336,  5229,  6131,  7042,  7961,  8889},
    //NTC 36℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,     0,     0,     0,     0,   858,  1726,  2601,  3486,  4379,  5281,  6192,  7111,  8039},
    //NTC 37℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,     0,     0,     0,     0,     0,   867,  1743,  2627,  3520,  4422,  5333,  6252,  7181},
    //NTC 38℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,     0,     0,     0,     0,     0,     0,   875,  1760,  2653,  3555,  4466,  5385,  6313},
    //NTC 39℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,     0,     0,     0,     0,     0,     0,     0,   884,  1777,  2679,  3590,  4509,  5437},
    //NTC 40℃
    {0,   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,     0,     0,     0,     0,     0,     0,     0,     0,   893,  1795,  2705,  3625,  4553},
};


/******************************************************************************\
                             Functions definitions
\******************************************************************************/
/*
* 函数名称 : Table_QueryMedian
* 功能描述 : 根据传入的表,查询出AD值处于表的哪个位置
* 参    数 : Dirt    - 表排序
             Arr     - 表地址
             ArrSize - 表个数
             AD      - ad值
             Num     - 表的第几个
* 返回值   : 返回错误代码
*/
/******************************************************************************/
static int Table_QueryMedian(uint8_t Dirt, const uint16_t *Arr, uint16_t ArrSize, uint16_t AD, uint8_t *Num)
/******************************************************************************/
{
    short head = 0, mid, tail = ArrSize - 1;
 
    if (Dirt) // 倒叙
    {
        if(AD > Arr[head])    // ad值大于表的第一个值
        {
            *Num = 0;
            return TAB_ERROR_MIN;
        }
        else if(AD < Arr[ArrSize - 1]) // ad值小于表的最后一个值
        {
            *Num = tail;
            return TAB_ERROR_MAX;
        }
        
        while(head <= tail)
        {
            mid = head + ((tail - head) >> 1);

            if(AD > Arr[mid])
            {
                tail = mid - 1;
            }
            else if(AD < Arr[mid])
            {
                head = mid + 1;
            }
            else
            {
                *Num = mid;
                return TAB_OK;
            }
        }
        
        *Num = head;
    }
    else // 正序
    {
        if(AD < Arr[head])    // ad值小于表的第一个值
        {
            *Num = 0;
            return TAB_ERROR_MIN;
        }
        else if(AD > Arr[ArrSize - 1]) // ad值大于表的最后一个值
        {
            *Num = tail;
            return TAB_ERROR_MAX;
        }
        
        while(head <= tail)
        {
            mid = head + ((tail - head) >> 1);

            if(AD > Arr[mid])
            {
                head = mid + 1;
            }
            else if(AD < Arr[mid])
            {
                tail = mid - 1;
            }
            else
            {
                *Num = mid;
                return TAB_OK;
            }
        }
        
        *Num = tail;
    }

    return TAB_OK;
}

uint8_t Temperature_Transverter(uint16_t NtcVal, uint16_t VoltDiff, uint8_t *Num)
{
    uint8_t ntc_num;
    short head = 0, mid, tail = 26 - 1;
    Table_QueryMedian(1,s_NtcTableData,100,NtcVal,&ntc_num);    //查询NTC温度值
    printf("\r\n[Temperature_Transverter] ntc_num = %d\n",ntc_num);
    if(VoltDiff == 0)                                           //差值为0，温度为NTC采集温度
    {
       *Num = ntc_num;
       return TAB_OK;
    }
    if(ntc_num > 20)                    
    {
        ntc_num = ntc_num - 20;                                 //温度起始值为20℃
    }
    if(VoltDiff < s_VoltDiffTableData[ntc_num][head])           // 值小于表的第一个值
    {
        *Num = 0;
        printf("\r\n[Temperature_Transverter] TAB_ERROR_MIN \n");
        return TAB_ERROR_MIN;
    }
    else if(VoltDiff > s_VoltDiffTableData[ntc_num][tail])      // 值大于表的最后一个值
    {
        *Num = tail;
        printf("\r\n[Temperature_Transverter] TAB_ERROR_MAX \n");
        return TAB_ERROR_MAX;
    }
    while(head <= tail)
    {
        mid = head + ((tail - head) >> 1);

        if(VoltDiff > s_VoltDiffTableData[ntc_num][mid])
        {
            head = mid + 1;
        }
        else if(VoltDiff < s_VoltDiffTableData[ntc_num][mid])
        {
            tail = mid - 1;
        }
        else
        {
            *Num = mid+20; 
            return TAB_OK;
        }
    }
    *Num = tail+20;

    return TAB_OK;
}



