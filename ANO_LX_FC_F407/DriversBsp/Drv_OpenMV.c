//Ĭ�����ã�
#include "Drv_OpenMV.h"

//�趨
#define OPMV_OFFLINE_TIME_MS  1000  //����

enum OpenMVmodeflag{
    Dot_Following = 1,
    Line_Following,
    AprilTag
}OpenMVmodeflagdata;

//ȫ�ֱ���
u16 offline_check_time;
u8 openmv_buf[20];
_openmv_data_st opmv;
/**********************************************************************************************************
*�� �� ��: OpenMV_Byte_Get
*����˵��: OpenMV�ֽ����ݻ�ȡ
*��    ��: �ֽ�����
*�� �� ֵ: ��
**********************************************************************************************************/
void OpenMV_Byte_Get(uint8_t bytedata)
{	
	static u8 len = 0,rec_sta;
	u8 check_val=0;
	
	//
	openmv_buf[rec_sta] = bytedata;
	//
	if(rec_sta==0)
	{
		if(bytedata==0xaa)
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
		if(bytedata==0x29)
		{
			rec_sta++;
		}	
		else
		{
			rec_sta=0;
		}		
	}
	else if(rec_sta==2)
	{
		if(bytedata==0x05)
		{
			rec_sta++;
		}
		else
		{
			rec_sta=0;
		}		
	}
	else if(rec_sta==3)
	{
		if(1)
		{
			rec_sta++;
		}
		else
		{
			rec_sta=0;
		}		
	}
	else if(rec_sta==4)
	{
		//
		len = bytedata;
		if(len<31)
		{
			rec_sta++;
		}		
		else
		{
			rec_sta=0;
		}
	}
	else if(rec_sta==(len+5))
	{
		//
		for(u8 i=0;i<len+5;i++)
		{
			check_val += openmv_buf[i];
		}
		//
		if(check_val == bytedata)
		{
			//�����ɹ�
			OpenMV_Data_Analysis(openmv_buf,len+6);
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
s16 pos_y,pos_z;
/**********************************************************************************************************
*�� �� ��: OpenMV_Data_Analysis
*����˵��: OpenMV���ݽ���
*��    ��: �������ݣ��βΣ�������
*�� �� ֵ: ��
**********************************************************************************************************/
static void OpenMV_Data_Analysis(uint8_t *buf_data,uint8_t len)
{
    
    
	if(*(buf_data+3)==0x41)
	{
		opmv.cb.color_flag = *(buf_data+5);
		opmv.cb.sta = *(buf_data+6);
		opmv.cb.pos_x = (s16)((*(buf_data+7)<<8)|*(buf_data+8));
		opmv.cb.pos_y = (s16)((*(buf_data+9)<<8)|*(buf_data+10));
		opmv.cb.dT_ms = *(buf_data+11);
		//
		opmv.mode_sta = 1;
	}
	else if(*(buf_data+3)==0x42)    
	{
		opmv.lt.sta = *(buf_data+5);
		opmv.lt.angle = (s16)((*(buf_data+6)<<8)|*(buf_data+7));
		opmv.lt.deviation = (s16)((*(buf_data+8)<<8)|*(buf_data+9));
		opmv.lt.p_flag = *(buf_data+10);
		opmv.lt.pos_x = (s16)((*(buf_data+11)<<8)|*(buf_data+12));
		opmv.lt.pos_y = (s16)((*(buf_data+13)<<8)|*(buf_data+14));
		opmv.lt.dT_ms = *(buf_data+15);
		//
		opmv.mode_sta = 2;
	}
    else if(*(buf_data+3)==0x43)//AprilTagʶ����    
	{
        opmv.at.is_invalid = *(buf_data+5);
        if (!opmv.at.is_invalid){
            pos_y = (s16)((*(buf_data+6)<<8)|*(buf_data+7));
            pos_z = (s16)((*(buf_data+8)<<8)|*(buf_data+9));
            opmv.at.pos_y = 80 - pos_y ;
            opmv.at.pos_z = 60 - pos_z ;
        }
        else {
            opmv.at.pos_y = 0;
            opmv.at.pos_z = 0 ;
        }
		opmv.mode_sta = 3;
	}
	//
	OpenMV_Check_Reset();
}

/**********************************************************************************************************
*�� �� ��: OpenMV_Offline_Check
*����˵��: OpenMV���߼�⣬�������Ӳ���Ƿ�����
*��    ��: ʱ�䣨���룩
*�� �� ֵ: ��
**********************************************************************************************************/
void OpenMV_Offline_Check(u8 dT_ms)
{
	if(offline_check_time<OPMV_OFFLINE_TIME_MS)
	{
		offline_check_time += dT_ms;
	}
	else
	{
		opmv.offline = 1;
		opmv.mode_sta = 0;
	}
	
}

/**********************************************************************************************************
*�� �� ��: OpenMV_Check_Reset
*����˵��: OpenMV���߼�⸴λ��֤��û�е���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
static void OpenMV_Check_Reset()
{
	offline_check_time = 0;
	opmv.offline = 0;
}


