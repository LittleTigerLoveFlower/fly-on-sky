#ifndef __DRV_HC08_H
#define __DRV_HC08_H

//==����
#include "SysConfig.h"

//==����
typedef struct
{
	u8 offline;
	s16 Dist;
    s16 Strength;
    s16 Temp;
}_HC08_data_st;
//==��������
extern _HC08_data_st HC08;

//==��������

//static
static void HC08_Data_Analysis(u8 *buf_data,u8 len);
static void HC08_Check_Reset(void);

//public
void HC08_Offline_Check(u8 dT_ms);
void HC08_Byte_Get(uint8_t bytedata);


#endif

