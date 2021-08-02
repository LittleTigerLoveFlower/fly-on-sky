/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * ����    �������ƴ�
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
 * ����    ���������
**********************************************************************************/
#include "Ano_Scheduler.h"
#include "Drv_OpenMV.h"
#include "ANO_DT_LX.h"
#include "Drv_AnoOf.h"
#include "Drv_TFMini_Plus.h"
#include "Drv_HWT101CT.h"
#include "User_Task.h"
#include "LX_FC_Fun.h"
#include "Drv_OpenMV.h"
#include "Drv_Uart.h"

extern _ano_of_st ano_of;
_user_flag_set user_flag = {0};
s16 dx, dy;
u16 cnt = 0;

//��ͨ�˲����Բ���
s16 dis_fix_x, dis_fix_y;
s16 dis_x, dis_y;

/*xy�ٶȻ�����*/
_PID_arg_st PID_Speed_arg_x; 
_PID_arg_st PID_Speed_arg_y; 
/*xy�ٶȻ�λ��*/
_PID_val_st PID_Speed_val_x;
_PID_val_st	PID_Speed_val_y;

/*xyλ�û�����*/
_PID_arg_st PID_Distance_arg_x;
_PID_arg_st PID_Distance_arg_y; 
/*xyλ�û�����*/
_PID_val_st PID_Distance_val_x;
_PID_val_st	PID_Distance_val_y;

/*yaw�ٶȻ�����*/
_PID_arg_st PID_Speed_arg_yaw;
/*yaw�ٶȻ�����*/
_PID_val_st PID_Speed_val_yaw;

/*yawλ�û�����*/
_PID_arg_st PID_Distance_arg_yaw;
/*yawλ�û�����*/
_PID_val_st PID_Distance_val_yaw;

/*z�ٶȻ�����*/
_PID_arg_st PID_Speed_arg_z;
/*z�ٶȻ�����*/
_PID_val_st PID_Speed_val_z;

/*zλ�û�����*/
_PID_arg_st PID_Distance_arg_z;
/*zλ�û�����*/
_PID_val_st PID_Distance_val_z;

/*x����*/
_user_exp_fdb_set user_exp_fdb_x;
/*x��ֵ*/
_user_threshold_set user_threshold_x;
/*x�������*/
s16 test_output_x;

/*�Ƹ�x��ֵ*/
_user_threshold_set user_threshold_pole_x;
/*�Ƹ�x�������*/
s16 test_output_pole_x;


/*y����*/
_user_exp_fdb_set user_exp_fdb_y;
/*y��ֵ*/
_user_threshold_set user_threshold_y;
/*y�������*/
s16 test_output_y;

/*z����*/
_user_exp_fdb_set user_exp_fdb_z;
/*z��ֵ*/
_user_threshold_set user_threshold_z;
/*z�������*/
s16 test_output_z;

/*��������*/
_user_exp_fdb_set user_exp_fdb_alt_z;
/*������ֵ*/
_user_threshold_set user_threshold_alt_z;
/*���߲������*/
s16 test_output_alt_z;

/*yaw����*/
_user_exp_fdb_set user_exp_fdb_yaw;
/*yaw��ֵ*/
_user_threshold_set user_threshold_yaw;
/*yaw�������*/
s16 test_output_yaw;

/*imuŷ��������*/
_user_eula_set user_eula;

s16 Position_now;
s16 Position_pre;
s16 Position_incre;

static s16 pos_now;
static s16 pos_pre;
static s16 pos_incre;
static s16 pos_start;
//////////////////////////////////////////////////////////////////////
//�û����������
//////////////////////////////////////////////////////////////////////

static void Loop_1000Hz(void) //1msִ��һ��
{
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
}

static void Loop_500Hz(void) //2msִ��һ��
{
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
}

static void Loop_200Hz(void) //5msִ��һ��
{
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
}

static void Loop_100Hz(void) //10msִ��һ��
{
	
	
}

static void Loop_50Hz(void) //20msִ��һ��
{
	//////////////////////////////////////////////////////////////////////
  OpenMV_Offline_Check(20);
  TFmini_Offline_Check(20);
	UserTask_OneKeyCmd();
	//////////////////////////////////////////////////////////////////////
	u8 _dt = 20;
	static s32 dis_dx, dis_dy;
	static s32 _dis_x, _dis_y;
	
	/*�ٶȻ���*/
	if(user_flag.of_dis_clear_cmd){
		dis_dx = 0;
		dis_dy = 0;
	}
	else{
		dis_dx += _dt * ano_of.of2_dx_fix;
		dis_dy += _dt * ano_of.of2_dy_fix;
	}
	
	/*��ͨ�˲�*/
	_dis_x += _dt * ano_of.of1_dx;
	_dis_y += _dt * ano_of.of1_dy;
	dis_x = _dis_x / 1000;
	dis_y = _dis_y / 1000;
	dis_fix_x += (dis_x - dis_fix_x) * 0.2;
	dis_fix_y += (dis_y - dis_fix_y) * 0.2;
	
	/*ʵ�ʾ�������*/
	dx = dis_dx / 1000;
	dy = dis_dy / 1000;
	
	dt.fun[0xf1].WTS = 1;
	dt.fun[0xf2].WTS = 1;
	dt.fun[0xf3].WTS = 1;
//		/*���ݷ���*/
//		User_DT_Send(ano_of, dx, dy);
	//////////////////////////////////////////////////////////////////////
}

static void Loop_20Hz(void) //50msִ��һ��
{
	/*********************************����*******************************************/
	if(mission_task){
		TaskSet(50);
	}
	/*********************************TFmini x�ᶨλ*******************************************/
  if(user_flag.tfmini_ctl_flag){
		TFMiniPosCtl(100); //x���򶨾���
	}
	/*********************************OpenMV yz�ᶨλ*******************************************/
	if(user_flag.opmv_ctl_flag){
		OpMVPosCtl(0, 0); //y����������z��������
	}
	
	/*********************************HWT101 yaw�ᶨλ*******************************************/
	if(user_flag.hwt101_ctl_flag){
		HWT101PosCtl(0); //���ò���
	}
		
	/*********************************�Ƹ�*******************************************/
	if(user_flag.pole_ctl_flag){
		PolePosCtl(50, -10, 0); //x���򱣳־��롢y�����ƶ��ٶȡ�yaw����λ��
	}
	
	/*********************************�������ⶨ��*******************************************/
	if(user_flag.of_alt_ctl_flag){
		OFAltCtl(130); //�����߶�
	}
	
	/*********************************����λ����*******************************************/
	if(user_flag.openmv_clr_flag){
		DataClr();
	}
	
	/*����ʵʱ����֡*/
		dt.fun[0x41].WTS = 1;
	//////////////////////////////////////////////////////////////////////
}

static void Loop_2Hz(void) //500msִ��һ��
{
    opmv.mode_sta[0] = 0x0A;
    opmv.mode_sta[1] = 2;
    opmv.mode_sta[2] = opmv.mode_sta[1] + opmv.mode_sta[0];
    DrvUart1SendBuf(&opmv.mode_sta[0],3);
}

/*
*@fn:			void Init_PID(void)
*@brief:	PID������ʼ��
*@para:		none
*@return:	none
*@comment:
*/
void Init_PID(void){
//x�ٶȻ�
	PID_Speed_arg_x.kp = 0.5f;
	PID_Speed_arg_x.ki = 0;
	PID_Speed_arg_x.kd_ex = 0;
	PID_Speed_arg_x.fb_d_mode = 0;
	PID_Speed_arg_x.kd_fb = 0;
	PID_Speed_arg_x.k_ff = 0;
	
//xλ�û�
	PID_Distance_arg_x.kp = 10.0f;
	PID_Distance_arg_x.ki = 0;
	PID_Distance_arg_x.kd_ex = 0;
	PID_Distance_arg_x.fb_d_mode = 0;
	PID_Distance_arg_x.kd_fb = 0;
	PID_Distance_arg_x.k_ff = 0;
	
//y�ٶȻ�
	PID_Speed_arg_y.kp = 0.5f;
	PID_Speed_arg_y.ki = 0;
	PID_Speed_arg_y.kd_ex = 0;
	PID_Speed_arg_y.fb_d_mode = 0;
	PID_Speed_arg_y.kd_fb = 0;
	PID_Speed_arg_y.k_ff = 0;
	
//yλ�û�
	PID_Distance_arg_y.kp = 5.0f;
	PID_Distance_arg_y.ki = 0;
	PID_Distance_arg_y.kd_ex = 0;
	PID_Distance_arg_y.fb_d_mode = 0;
	PID_Distance_arg_y.kd_fb = 0;
	PID_Distance_arg_y.k_ff = 0;	
	
//yaw�ٶȻ�
	PID_Speed_arg_yaw.kp = 0.5f;
	PID_Speed_arg_yaw.ki = 0;
	PID_Speed_arg_yaw.kd_ex = 0;
	PID_Speed_arg_yaw.fb_d_mode = 0;
	PID_Speed_arg_yaw.kd_fb = 0;
	PID_Speed_arg_yaw.k_ff = 0;
	
//yawλ�û�
	PID_Distance_arg_yaw.kp = 3.0f;
	PID_Distance_arg_yaw.ki = 0;
	PID_Distance_arg_yaw.kd_ex = 0;
	PID_Distance_arg_yaw.fb_d_mode = 0;
	PID_Distance_arg_yaw.kd_fb = 0;
	PID_Distance_arg_yaw.k_ff = 0;	
	
//z�ٶȻ�
	PID_Speed_arg_z.kp = 0.5f;
	PID_Speed_arg_z.ki = 0;
	PID_Speed_arg_z.kd_ex = 0;
	PID_Speed_arg_z.fb_d_mode = 0;
	PID_Speed_arg_z.kd_fb = 0;
	PID_Speed_arg_z.k_ff = 0;
	
//zλ�û�
	PID_Distance_arg_z.kp = 3.0f;
	PID_Distance_arg_z.ki = 0;
	PID_Distance_arg_z.kd_ex = 0;
	PID_Distance_arg_z.fb_d_mode = 0;
	PID_Distance_arg_z.kd_fb = 0;
	PID_Distance_arg_z.k_ff = 0;	
}

/*
*@fn:			void Init_GeneralCtlArg(void)
*@brief:	ͨ�ÿ��Ʋ�����ʼ��
*@para:		none
*@return:	none
*@comment:
*/
void Init_GeneralCtlArg(void){
	/*x*/
	user_threshold_x.max_speed = 20;
	user_threshold_x.normalize_distance = 500.0f;
	user_threshold_x.normalize_speed = 20.0f;
	
	/*x�Ƹ�*/
	user_threshold_pole_x.max_speed = 3;
	user_threshold_pole_x.normalize_distance = 60.0f;
	user_threshold_pole_x.normalize_speed = 3.0f;
	
	/*y*/
	user_threshold_y.max_speed = 20;
	user_threshold_y.normalize_distance = 80.0f;
	user_threshold_y.normalize_speed = 20.0f;
	
	/*z*/
	user_threshold_z.max_speed = 15;
	user_threshold_z.normalize_distance = 40.0f;
	user_threshold_z.normalize_speed = 15.0f;
	
	/*����*/
	user_threshold_alt_z.max_speed = 15;
	user_threshold_alt_z.normalize_distance = 200;
	user_threshold_alt_z.normalize_speed = 15.0f;
	
	/*yaw*/
	user_threshold_yaw.max_speed = 15;
	user_threshold_yaw.normalize_distance = 200.0f;
	user_threshold_yaw.normalize_speed = 15.0f;
}

//////////////////////////////////////////////////////////////////////
//��������ʼ��
//////////////////////////////////////////////////////////////////////
//ϵͳ�������ã�������ִͬ��Ƶ�ʵġ��̡߳�
static sched_task_t sched_tasks[] =
	{
		{Loop_1000Hz, 1000, 0, 0},
		{Loop_500Hz, 500, 0, 0},
		{Loop_200Hz, 200, 0, 0},
		{Loop_100Hz, 100, 0, 0},
		{Loop_50Hz, 50, 0, 0},
		{Loop_20Hz, 20, 0, 0},
		{Loop_2Hz, 2, 0, 0},
};
//�������鳤�ȣ��ж��߳�����
#define TASK_NUM (sizeof(sched_tasks) / sizeof(sched_task_t))

void Scheduler_Setup(void)
{
	uint8_t index = 0;
	//��ʼ�������
	for (index = 0; index < TASK_NUM; index++)
	{
		//����ÿ���������ʱ������
		sched_tasks[index].interval_ticks = TICK_PER_SECOND / sched_tasks[index].rate_hz;
		//�������Ϊ1��Ҳ����1ms
		if (sched_tasks[index].interval_ticks < 1)
		{
			sched_tasks[index].interval_ticks = 1;
		}
	}
}
//��������ŵ�main������while(1)�У���ͣ�ж��Ƿ����߳�Ӧ��ִ��
void Scheduler_Run(void)
{
	uint8_t index = 0;
	//ѭ���ж������̣߳��Ƿ�Ӧ��ִ��

	for (index = 0; index < TASK_NUM; index++)
	{
		//��ȡϵͳ��ǰʱ�䣬��λMS
		uint32_t tnow = GetSysRunTimeMs();
		//�����жϣ������ǰʱ���ȥ��һ��ִ�е�ʱ�䣬���ڵ��ڸ��̵߳�ִ�����ڣ���ִ���߳�
		if (tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{

			//�����̵߳�ִ��ʱ�䣬������һ���ж�
			sched_tasks[index].last_run = tnow;
			//ִ���̺߳�����ʹ�õ��Ǻ���ָ��
			sched_tasks[index].task_func();
		}
	}
}


u8 TFMiniPosCtl(s16 expect){
	user_exp_fdb_x.exp_distance = expect;
	user_exp_fdb_x.fdb_distance = tfmini.Dist;
	
	test_output_x = GeneralPosCtl(user_exp_fdb_x, Direction_x, PID_Distance_arg_x, PID_Distance_val_x, user_threshold_x, 1);
	
	return 1;
}

u8 OpMVPosCtl(s16 expect1, s16 expect2){
	user_exp_fdb_z.exp_distance = expect2;
	user_exp_fdb_y.exp_distance = expect1;
		
	if(opmv.at.is_invalid){
		user_exp_fdb_y.fdb_distance = user_exp_fdb_y.exp_distance;
		user_exp_fdb_z.fdb_distance = user_exp_fdb_z.exp_distance;
	}
	else{
		user_exp_fdb_y.fdb_distance = opmv.at.pos_y;
		user_exp_fdb_z.fdb_distance = opmv.at.pos_z;
	}
		
	test_output_y = GeneralPosCtl(user_exp_fdb_y, Direction_y, PID_Distance_arg_y, PID_Distance_val_y, user_threshold_y, 1);
	test_output_z = GeneralPosCtl(user_exp_fdb_z, Direction_z, PID_Distance_arg_z, PID_Distance_val_z, user_threshold_z, 1);
	
	return 1;
}
u8 OpMVPosCtl_Down(s16 expect1, s16 expect2){
	user_exp_fdb_x.exp_distance = expect1;
	user_exp_fdb_y.exp_distance = expect2;
		
	if(opmv.mol.is_invalid){
		user_exp_fdb_x.fdb_distance = user_exp_fdb_x.exp_distance;
		user_exp_fdb_y.fdb_distance = user_exp_fdb_y.exp_distance;
	}
	else{
		user_exp_fdb_x.fdb_distance = opmv.mol.pos_y;
		user_exp_fdb_y.fdb_distance = opmv.mol.pos_z;
	}
		
	test_output_x = GeneralPosCtl(user_exp_fdb_x, Direction_x, PID_Distance_arg_x, PID_Distance_val_x, user_threshold_y, 1);
	test_output_y = GeneralPosCtl(user_exp_fdb_y, Direction_y, PID_Distance_arg_x, PID_Distance_val_y, user_threshold_y, 1);
	
	return 1;
}
u8 HWT101PosCtl(s16 expect){
	/*hwt101��֤yaw��ƽ��*/
	if(user_flag.yaw_set_flag){
		user_exp_fdb_yaw.exp_distance = hwt101ct.yaw_angle;
		user_flag.yaw_set_flag = 0;
	}
	
	if(hwt101ct.offline){
		user_exp_fdb_yaw.fdb_distance = user_exp_fdb_yaw.exp_distance;
	}
	else{
	/*������ж�*/
		Position_now = hwt101ct.yaw_angle;
		if((Position_now - Position_pre) > 180)
		{
			Position_incre  += (Position_now - Position_pre) - 359;
		}
		else if((Position_now - Position_pre) < -180)
		{
			Position_incre  += 359 + (Position_now - Position_pre);
		}
		else
		{
			Position_incre  += (Position_now - Position_pre);
		}
		Position_pre = Position_now;
			
		user_exp_fdb_yaw.fdb_distance = Position_incre;
	}
	test_output_yaw = GeneralPosCtl(user_exp_fdb_yaw, Direction_yaw, PID_Distance_arg_yaw, PID_Distance_val_yaw, user_threshold_yaw, 1);
	
	return 1;
}	

u8 PolePosCtl(s16 exp_x, s16 exp_y, s16 exp_yaw){
	user_exp_fdb_x.exp_distance = exp_x;
	user_exp_fdb_yaw.exp_distance = exp_yaw;
		
	if(opmv.pole.is_invalid){
		user_exp_fdb_x.fdb_distance = user_exp_fdb_x.exp_distance;
		user_exp_fdb_yaw.fdb_distance = user_exp_fdb_yaw.exp_distance;
	}
	else{
		user_exp_fdb_x.fdb_distance = opmv.pole.Dist;
		user_exp_fdb_yaw.fdb_distance = opmv.pole.pos_y;
	}
			
	test_output_x = GeneralPosCtl(user_exp_fdb_x, Direction_x, PID_Distance_arg_x, PID_Distance_val_x, user_threshold_pole_x, 1);
	test_output_yaw = GeneralPosCtl(user_exp_fdb_yaw, Direction_yaw, PID_Distance_arg_yaw, PID_Distance_val_yaw, user_threshold_yaw, 1);
	rt_tar.st_data.vel_y = exp_y;	
	
	return 1;
}
u8 OFAltCtl(u16 expect){
	user_exp_fdb_alt_z.exp_distance = expect;
	user_exp_fdb_alt_z.fdb_distance = ano_of.of_alt_cm;
	if(user_exp_fdb_alt_z.fdb_distance < 30)
		rt_tar.st_data.vel_z = 0;
	else
		test_output_alt_z = GeneralPosCtl(user_exp_fdb_alt_z, Direction_z, PID_Distance_arg_z, PID_Distance_val_z, user_threshold_alt_z, 0);	

	return 1;
}
u8 DataClr(void){
	user_flag.openmv_clr_flag = 0;
	RealTimeSpeedControl(0, Direction_x);
	RealTimeSpeedControl(0, Direction_y);
	RealTimeSpeedControl(0, Direction_z);
	RealTimeSpeedControl(0, Direction_yaw);
	Position_incre = 0;
	Position_pre = 0;
	cnt = 0;
	//RealTimeSpeedControlSend(0, Direction_yaw);
	
	return 1;
}	

//����
u8 TaskSet(s16 dT){
	switch(mission_step){
		case 0:
			cnt = 0;
			break;
		
		case 1:
			/*3s��ʱ*/
			//FC_Unlock();
			cnt += dT;
			if(cnt >= 3000){
				cnt = 0;
				mission_step += 1;
			}
			break;
			
		case 2:
			/*һ�����*/
			//mission_step += OneKey_Takeoff(100);
			break;
		
		case 3:
			user_flag.yaw_set_flag = 1;
			Position_incre = 0;
			Position_pre = 0;
			mission_step += 1;
			break;
		
		case 4:
			/*ʶ��ͣ��ƺ��Ħ��������ͣ8s*/
			opmv.mode_sta[1] = 2; //Ħ����ʶ��ģʽ
			
			OpMVPosCtl_Down(0, 0);
			HWT101PosCtl(0);
			
			cnt += dT;
			if(cnt >= 8000){
				cnt = 0;
				mission_step += 1;
			}
			break;
			
		case 5:
			/*���ߵ�130cm*/
			OFAltCtl(130);
			if(ano_of.of_alt_cm > 120 && ano_of.of_alt_cm < 140){
				cnt += dT;
			}
			if(cnt >= 1000){
				cnt = 0;
				mission_step += 1;
			}
			break;
			
		case 6:
			/*����yaw������*/
			user_flag.yaw_set_flag = 1;
			Position_incre = 0;
			Position_pre = 0;
		
			mission_step += 1;
			break;
		
		case 7:
			HWT101PosCtl(0);
			/*��10cm/s�ٶ���ǰ�ƶ�10s*/
			RealTimeSpeedControl(10, Direction_x);
			cnt += dT;
		
			/*ûʶ��Ŀ��*/
			if(cnt >= 10000){
				cnt = 0;
				mission_step += 1;
			}
			
			/*ʶ��Ŀ��*/
			if(!opmv.mol.is_invalid){
				RealTimeSpeedControl(0, Direction_x);
				mission_step = 9;
			}
			break;
			
		/*δʶ�𵽸�*/
		case 8:
			/*ԭ������ֱ��ʶ�𵽸�*/
			RealTimeSpeedControl(15, Direction_yaw);
			if(!opmv.mol.is_invalid){
				mission_step += 1;
			}
			break;
		
		/*ʶ�𵽸�*/
		case 9:
			pos_now = hwt101ct.yaw_angle;
			pos_incre = ZeroPointCross(pos_now, pos_pre, pos_incre);
			pos_pre = pos_now;
			pos_start = pos_incre;
			mission_step += 1;	
			break;
		
		case 10:
			PolePosCtl(50, -10, 0);
			pos_now = hwt101ct.yaw_angle;
			pos_incre = ZeroPointCross(pos_now, pos_pre, pos_incre);
			pos_pre = pos_now;
			if(UserAbs(pos_incre - pos_start) > 360){
				mission_step += 1;
				pos_incre = 0;
				pos_pre = 0;
			}
			break;
		
		case 11:
			RealTimeSpeedControl(-10, Direction_x);
			cnt += dT;
			if(cnt > 3000){
				cnt = 0;
				mission_step += 1;
			}
			break;
			
		case 12:
			OneKey_Land();
			mission_step = 0;
			break;
		default:
			break;
	}
	
	return 1;
}

s16 ZeroPointCross(s16 pos_now, s16 pos_pre, s16 pos_incre){
	if((pos_now - pos_pre) > 180)
	{
		pos_incre  += (pos_now - pos_pre) - 359;
	}
	else if((pos_now - pos_pre) < -180)
	{
		pos_incre  += 359 + (pos_now - pos_pre);
	}
	else
	{
		pos_incre  += (pos_now - pos_pre);
	}
	
	return pos_incre;
}

s16 UserAbs(s16 num){
	if(num < 0)
		return -1 * num;
	else	
		return num;
}
/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/


