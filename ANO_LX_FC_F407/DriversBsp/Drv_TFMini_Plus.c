//Ĭ�����ã�
#include "Drv_TFMini_Plus.h"

//�趨
#define TFMINI_OFFLINE_TIME_MS  1000  //����

//ȫ�ֱ���
u16 tf_offline_check_time;
u8 tfmini_buf[20];
_tfminiplus_data_st tfmini;
/**********************************************************************************************************
*�� �� ��: OpenMV_Byte_Get
*����˵��: OpenMV�ֽ����ݻ�ȡ
*��    ��: �ֽ�����
*�� �� ֵ: ��
**********************************************************************************************************/
void TFMini_Byte_Get(uint8_t bytedata)
{	
	static u8 rec_sta;
	u8 check_val=0;

	tfmini_buf[rec_sta] = bytedata;

	if(rec_sta==0)
	{
		if(bytedata==0x59)
		{
			rec_sta++;
		}
		else
		{
			rec_sta=0;
		}
	}
	else if(rec_sta==1)
	{
		if(bytedata==0x59)
		{
			rec_sta++;
		}	
		else
		{
			rec_sta=0;
		}		
	}
	else if(rec_sta==8)
	{
		//
		for(u8 i=0;i<8;i++)
		{
			check_val += tfmini_buf[i];
		}
		//
		if(check_val == bytedata)
		{
			//�����ɹ�
			TFMini_Data_Analysis(tfmini_buf,9);
			//
			rec_sta=0;
		}
		else
		{
			rec_sta=0;
		}		
	}
	else
	{
		//	
		rec_sta++;
	}
	
}

/**********************************************************************************************************
*�� �� ��: TFMini_Data_Analysis
*����˵��: TFMini���ݽ���
*��    ��: �������ݣ��βΣ�������
*�� �� ֵ: ��
**********************************************************************************************************/
static void TFMini_Data_Analysis(u8 *buf_data,u8 len)
{
    tfmini.Dist = (s16)((*(buf_data+3)<<8)|*(buf_data+2));
    tfmini.Strength = (s16)((*(buf_data+5)<<8)|*(buf_data+4));
    tfmini.Temp = (s16)((*(buf_data+3)<<7)|*(buf_data+6));
	TFMini_Check_Reset();
}

/**********************************************************************************************************
*�� �� ��: TFmini_Offline_Check
*����˵��: TFmini���߼�⣬�������Ӳ���Ƿ�����
*��    ��: ʱ�䣨���룩
*�� �� ֵ: ��
**********************************************************************************************************/
void TFmini_Offline_Check(u8 dT_ms)
{
	if(tf_offline_check_time<TFMINI_OFFLINE_TIME_MS)
	{
		tf_offline_check_time += dT_ms;
	}
	else
	{
		tfmini.offline = 1;
	}
	
}

/**********************************************************************************************************
*�� �� ��: TFMini_Check_Reset
*����˵��: TFMini���߼�⸴λ��֤��û�е���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
static void TFMini_Check_Reset()
{
	tf_offline_check_time = 0;
	tfmini.offline = 0;
}
