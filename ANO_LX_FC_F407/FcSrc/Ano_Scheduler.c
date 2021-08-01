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

extern _ano_of_st ano_of;
_user_flag_set user_flag = {0};
s16 dx, dy;

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
	/*********************************TFmini x�ᶨλ*******************************************/
  if(user_flag.tfmini_ctl_flag){
		/*TFmini����x��*/
		user_exp_fdb_x.exp_distance = 100;
		user_exp_fdb_x.fdb_distance = tfmini.Dist;
		test_output_x = GeneralPosCtl(user_exp_fdb_x, Direction_x, PID_Distance_arg_x, PID_Distance_val_x, user_threshold_x, 1);
	}
	
	/*********************************OpenMV yz�ᶨλ*******************************************/
	if(user_flag.opmv_ctl_flag){
		/*OpenMV����yz*/
		if(opmv.at.is_invalid){
			user_exp_fdb_y.fdb_distance = 0;
			user_exp_fdb_z.fdb_distance = 0;
		}
		else{
			user_exp_fdb_y.fdb_distance = opmv.at.pos_y;
			user_exp_fdb_z.fdb_distance = opmv.at.pos_z;
		}
		
		user_exp_fdb_y.exp_distance = 0;
		test_output_y = GeneralPosCtl(user_exp_fdb_y, Direction_y, PID_Distance_arg_y, PID_Distance_val_y, user_threshold_y, 1);
		
		user_exp_fdb_z.exp_distance = 0;
		test_output_z = GeneralPosCtl(user_exp_fdb_z, Direction_z, PID_Distance_arg_z, PID_Distance_val_z, user_threshold_z, 1);
	}
	
	/*********************************HWT101 yaw�ᶨλ*******************************************/
	if(user_flag.hwt101_ctl_flag){
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
	}
		
	/*********************************�Ƹ�*******************************************/
	if(user_flag.pole_ctl_flag){
		user_exp_fdb_x.exp_distance = 30;
		user_exp_fdb_x.fdb_distance = opmv.pole.pos_x;
		test_output_x = GeneralPosCtl(user_exp_fdb_x, Direction_x, PID_Distance_arg_x, PID_Distance_val_x, user_threshold_pole_x, 1);
		
		user_exp_fdb_yaw.exp_distance = 0;
		user_exp_fdb_yaw.fdb_distance = opmv.pole.angle_yaw;
		test_output_yaw = GeneralPosCtl(user_exp_fdb_yaw, Direction_yaw, PID_Distance_arg_yaw, PID_Distance_val_yaw, user_threshold_yaw, 1);
		
		rt_tar.st_data.vel_y = 10;
	}
	
	/*********************************����λ����*******************************************/
	if(user_flag.openmv_clr_flag){
		user_flag.openmv_clr_flag = 0;
		RealTimeSpeedControl(0, Direction_x);
		RealTimeSpeedControl(0, Direction_y);
		RealTimeSpeedControl(0, Direction_z);
		RealTimeSpeedControl(0, Direction_yaw);
		Position_incre = 0;
		Position_pre = 0;
		//RealTimeSpeedControlSend(0, Direction_yaw);
	}
	
	/*����ʵʱ����֡*/
		dt.fun[0x41].WTS = 1;

	//////////////////////////////////////////////////////////////////////
}

static void Loop_2Hz(void) //500msִ��һ��
{
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
	PID_Distance_arg_y.kp = 3.0f;
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
	PID_Distance_arg_yaw.kp = 5.0f;
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
	PID_Distance_arg_z.kp = 1.0f;
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
	user_threshold_pole_x.max_speed = 10;
	user_threshold_pole_x.normalize_distance = 50.0f;
	user_threshold_pole_x.normalize_speed = 10.0f;
	
	/*y*/
	user_threshold_y.max_speed = 20;
	user_threshold_y.normalize_distance = 80.0f;
	user_threshold_y.normalize_speed = 20.0f;
	
	/*z*/
	user_threshold_z.max_speed = 15;
	user_threshold_z.normalize_distance = 40.0f;
	user_threshold_z.normalize_speed = 15.0f;
	
	/*yaw*/
	user_threshold_yaw.max_speed = 20;
	user_threshold_yaw.normalize_distance = 200.0f;
	user_threshold_yaw.normalize_speed = 20.0f;
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

/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
