/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * 作者    ：匿名科创
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
 * 描述    ：任务调度
**********************************************************************************/
#include "Ano_Scheduler.h"
#include "Drv_OpenMV.h"
#include "ANO_DT_LX.h"
#include "Drv_AnoOf.h"
#include "Drv_US42V2.h"
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
u8 user_send_buffer[50];
s16 pp;
static void ANO_DT_LX_Send_Data(u8 *dataToSend, u8 length)
{
	//
	UartSendLXIMU(dataToSend, length);
}
static u8 change1(s16 a)
{
	return a&((1<<8)-1);
}
static u8 change2(s16 a)
{
	return (a^change1(a))>>8;
}
int myabs(int x)
{
	if(x<0)return -x;
	return x;
}
static void user_send(u8 frame_num,s16 data1,s16 data2)
{
	u8 cnt=0;
	user_send_buffer[cnt++]=0xAA;
	user_send_buffer[cnt++]=0xff;
	user_send_buffer[cnt++]=frame_num;
	user_send_buffer[cnt++]=4;
	user_send_buffer[cnt++]=change1(data1);
	user_send_buffer[cnt++]=change2(data1);
	user_send_buffer[cnt++]=change1(data2);
	user_send_buffer[cnt++]=change2(data2);
	u8 check_sum1 = 0, check_sum2 = 0;
	for (u8 i = 0; i < cnt; i++)
	{
		check_sum1 += user_send_buffer[i];
		check_sum2 += check_sum1;
	}
	user_send_buffer[cnt++] = check_sum1;
	user_send_buffer[cnt++] = check_sum2;
	ANO_DT_LX_Send_Data(user_send_buffer, cnt);
}
//低通滤波测试参数
s16 dis_fix_x, dis_fix_y;
s16 dis_x, dis_y;

/*xy速度环参数*/
_PID_arg_st PID_Speed_arg_x; 
_PID_arg_st PID_Speed_arg_y; 
/*xy速度环位置*/
_PID_val_st PID_Speed_val_x;
_PID_val_st	PID_Speed_val_y;

/*xy位置环参数*/
_PID_arg_st PID_Distance_arg_x;
_PID_arg_st PID_Distance_arg_y; 
/*xy位置环参数*/
_PID_val_st PID_Distance_val_x;
_PID_val_st	PID_Distance_val_y;

/*yaw速度环参数*/
_PID_arg_st PID_Speed_arg_yaw;
/*yaw速度环参数*/
_PID_val_st PID_Speed_val_yaw;

/*yaw位置环参数*/
_PID_arg_st PID_Distance_arg_yaw;
/*yaw位置环参数*/
_PID_val_st PID_Distance_val_yaw;

/*z速度环参数*/
_PID_arg_st PID_Speed_arg_z;
/*z速度环参数*/
_PID_val_st PID_Speed_val_z;

/*z位置环参数*/
_PID_arg_st PID_Distance_arg_z;
/*z位置环参数*/
_PID_val_st PID_Distance_val_z;

/*x期望*/
_user_exp_fdb_set user_exp_fdb_x;
/*x阈值*/
_user_threshold_set user_threshold_x;
/*x测试输出*/
s16 test_output_x;

/*绕杆x阈值*/
_user_threshold_set user_threshold_pole_x;
/*绕杆x测试输出*/
s16 test_output_pole_x;


/*y期望*/
_user_exp_fdb_set user_exp_fdb_y;
/*y阈值*/
_user_threshold_set user_threshold_y;
/*y测试输出*/
s16 test_output_y;

/*z期望*/
_user_exp_fdb_set user_exp_fdb_z;
/*z阈值*/
_user_threshold_set user_threshold_z;
/*z测试输出*/
s16 test_output_z;

/*定高期望*/
_user_exp_fdb_set user_exp_fdb_alt_z;
/*定高阈值*/
_user_threshold_set user_threshold_alt_z;
/*定高测试输出*/
s16 test_output_alt_z;

/*yaw期望*/
_user_exp_fdb_set user_exp_fdb_yaw;
/*yaw阈值*/
_user_threshold_set user_threshold_yaw;
/*yaw测试输出*/
s16 test_output_yaw;

/*imu欧拉角数据*/
_user_eula_set user_eula;

s16 Position_now;
s16 Position_pre;
s16 Position_incre;
int test11,test22;
static s16 pos_now;
static s16 pos_pre;
static s16 pos_incre;
static s16 pos_start;
int p=1000;
bool timejudge(int s)
{
	if(cnt/p>=s)return 1;
	return 0;
}
//////////////////////////////////////////////////////////////////////
//用户程序调度器
//////////////////////////////////////////////////////////////////////

static void Loop_1000Hz(void) //1ms执行一次
{
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
}

static void Loop_500Hz(void) //2ms执行一次
{
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
}

static void Loop_200Hz(void) //5ms执行一次
{
	//////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////
}

static void Loop_100Hz(void) //10ms执行一次
{
	
	
}

static void Loop_50Hz(void) //20ms执行一次
{
	//////////////////////////////////////////////////////////////////////
  OpenMV_Offline_Check(20);
  TFmini_Offline_Check(20);
	UserTask_OneKeyCmd();
	//////////////////////////////////////////////////////////////////////
	u8 _dt = 20;
	static s32 dis_dx, dis_dy;
	static s32 _dis_x, _dis_y;
	
	/*速度积分*/
	if(user_flag.of_dis_clear_cmd){
		dis_dx = 0;
		dis_dy = 0;
	}
	else{
		dis_dx += _dt * ano_of.of2_dx_fix;
		dis_dy += _dt * ano_of.of2_dy_fix;
	}
	
	/*低通滤波*/
	_dis_x += _dt * ano_of.of1_dx;
	_dis_y += _dt * ano_of.of1_dy;
	dis_x = _dis_x / 1000;
	dis_y = _dis_y / 1000;
	dis_fix_x += (dis_x - dis_fix_x) * 0.2;
	dis_fix_y += (dis_y - dis_fix_y) * 0.2;
	
	/*实际距离数据*/
	dx = dis_dx / 1000;
	dy = dis_dy / 1000;
	
	dt.fun[0xf1].WTS = 1;
	dt.fun[0xf2].WTS = 1;
	dt.fun[0xf3].WTS = 1;
//		/*数据发送*/
	//user_send(0xf1,mission_step,0);
	//////////////////////////////////////////////////////////////////////
}

static void Loop_20Hz(void) //50ms执行一次
{
	//OFAltCtl(150);
	//user_send(0xf2,speed_zz,direction_z);
	user_send(0xf1,tfmini.Dist,opmv.pole.Dist);
	user_send(0xf2,mission_step,mission_step);
	//user_send(0xf3,mission_step,ano_of.of_alt_cm);
	/*********************************绕杆*******************************************/
	if(user_flag.pole_ctl_flag&&opmv.pole.is_invalid&&opmv.pole.pos_y>30&&opmv.pole.pos_y<70){
		PolePosCtl(50, -10, 0); //x方向保持距离、y方向移动速度、yaw期望位置
	}
	else if(user_flag.pole_ctl_flag)PolePosCtl(50, 0, 0);
	
	/*********************************OpenMV yz轴定位*******************************************/
	if(user_flag.opmv_ctl_flag){
		OpMVPosCtl_pole(0, 0); //y方向期望、z方向期望
	}
	test11=rt_tar.st_data.vel_y;
	/*********************************HWT101 yaw轴定位*******************************************/
	if(user_flag.hwt101_ctl_flag){
		HWT101PosCtl(0); //无用参数
		//RealTimeSpeedControl(-15,Direction_yaw);
	}
	/*********************************TFmini x轴定位*******************************************/
  if(user_flag.tfmini_ctl_flag){
		TFMiniPosCtl(50); //x方向定距离
	}	
	
	/*********************************光流激光定高*******************************************/
	if(user_flag.of_alt_ctl_flag==1){
		OFAltCtl(130); //期望高度
	}
	else if(user_flag.of_alt_ctl_flag==2){
		OFAltCtl(10); //期望高度
	}
	/*********************************任务集*******************************************/
	if(mission_task){
		//TaskSet(50);
		taskset2(50);
	}
	test22=rt_tar.st_data.vel_y;
	/*********************************数据位清零*******************************************/
	if(user_flag.openmv_clr_flag){
		DataClr();
		user_flag.openmv_clr_flag = 0;
	}
	
	/*发送实时控制帧*/
		
		dt.fun[0x41].WTS = 1;
	//////////////////////////////////////////////////////////////////////
}

static void Loop_2Hz(void) //500ms执行一次
{
    opmv.mode_cmd[0] = 0x0A;
    opmv.mode_cmd[1] = 2;
    opmv.mode_cmd[2] = opmv.mode_cmd[1] + opmv.mode_cmd[0];
    DrvUart1SendBuf(&opmv.mode_cmd[0],3);
}

/*
*@fn:			void Init_PID(void)
*@brief:	PID参数初始化
*@para:		none
*@return:	none
*@comment:
*/
void Init_PID(void){
//x速度环
	PID_Speed_arg_x.kp = 0.5f;
	PID_Speed_arg_x.ki = 0;
	PID_Speed_arg_x.kd_ex = 0;
	PID_Speed_arg_x.fb_d_mode = 0;
	PID_Speed_arg_x.kd_fb = 0;
	PID_Speed_arg_x.k_ff = 0;
	
//x位置环
	PID_Distance_arg_x.kp = 8.0f;
	PID_Distance_arg_x.ki = 0;
	PID_Distance_arg_x.kd_ex = 0;
	PID_Distance_arg_x.fb_d_mode = 0;
	PID_Distance_arg_x.kd_fb = 0;
	PID_Distance_arg_x.k_ff = 0;
	
//y速度环
	PID_Speed_arg_y.kp = 0.5f;
	PID_Speed_arg_y.ki = 0;
	PID_Speed_arg_y.kd_ex = 0;
	PID_Speed_arg_y.fb_d_mode = 0;
	PID_Speed_arg_y.kd_fb = 0;
	PID_Speed_arg_y.k_ff = 0;
	
//y位置环
	PID_Distance_arg_y.kp = 2.5f;
	PID_Distance_arg_y.ki = 0;
	PID_Distance_arg_y.kd_ex = 0;
	PID_Distance_arg_y.fb_d_mode = 0;
	PID_Distance_arg_y.kd_fb = 0;
	PID_Distance_arg_y.k_ff = 0;	
	
//yaw速度环
	PID_Speed_arg_yaw.kp = 0.5f;
	PID_Speed_arg_yaw.ki = 0;
	PID_Speed_arg_yaw.kd_ex = 0;
	PID_Speed_arg_yaw.fb_d_mode = 0;
	PID_Speed_arg_yaw.kd_fb = 0;
	PID_Speed_arg_yaw.k_ff = 0;
	
//yaw位置环
	PID_Distance_arg_yaw.kp = 6.0f;
	PID_Distance_arg_yaw.ki = 0;
	PID_Distance_arg_yaw.kd_ex = 0;
	PID_Distance_arg_yaw.fb_d_mode = 0;
	PID_Distance_arg_yaw.kd_fb = 0;
	PID_Distance_arg_yaw.k_ff = 0;	
	
//z速度环
	PID_Speed_arg_z.kp = 0.5f;
	PID_Speed_arg_z.ki = 0;
	PID_Speed_arg_z.kd_ex = 0;
	PID_Speed_arg_z.fb_d_mode = 0;
	PID_Speed_arg_z.kd_fb = 0;
	PID_Speed_arg_z.k_ff = 0;
	
//z位置环
	PID_Distance_arg_z.kp = 3.0f;
	PID_Distance_arg_z.ki = 0;
	PID_Distance_arg_z.kd_ex = 0;
	PID_Distance_arg_z.fb_d_mode = 0;
	PID_Distance_arg_z.kd_fb = 0;
	PID_Distance_arg_z.k_ff = 0;	
}

/*
*@fn:			void Init_GeneralCtlArg(void)
*@brief:	通用控制参数初始化
*@para:		none
*@return:	none
*@comment:
*/
void Init_GeneralCtlArg(void){
	/*x*/
	user_threshold_x.max_speed = 12;
	user_threshold_x.normalize_distance = 500.0f;
	user_threshold_x.normalize_speed = 20.0f;
	
	/*x绕杆*/
	user_threshold_pole_x.max_speed = 3;
	user_threshold_pole_x.normalize_distance = 60.0f;
	user_threshold_pole_x.normalize_speed = 3.0f;
	
	/*y*/
	user_threshold_y.max_speed = 12;
	user_threshold_y.normalize_distance = 80.0f;
	user_threshold_y.normalize_speed = 20.0f;
	
	/*z*/
	user_threshold_z.max_speed = 15;
	user_threshold_z.normalize_distance = 40.0f;
	user_threshold_z.normalize_speed = 15.0f;
	
	/*定高*/
	user_threshold_alt_z.max_speed = 15;
	user_threshold_alt_z.normalize_distance = 50.0f;
	user_threshold_alt_z.normalize_speed = 15.0f;
	
	/*yaw*/
	user_threshold_yaw.max_speed = 30;
	user_threshold_yaw.normalize_distance = 200.0f;
	user_threshold_yaw.normalize_speed = 15.0f;
}

//////////////////////////////////////////////////////////////////////
//调度器初始化
//////////////////////////////////////////////////////////////////////
//系统任务配置，创建不同执行频率的“线程”
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
//根据数组长度，判断线程数量
#define TASK_NUM (sizeof(sched_tasks) / sizeof(sched_task_t))

void Scheduler_Setup(void)
{
	uint8_t index = 0;
	//初始化任务表
	for (index = 0; index < TASK_NUM; index++)
	{
		//计算每个任务的延时周期数
		sched_tasks[index].interval_ticks = TICK_PER_SECOND / sched_tasks[index].rate_hz;
		//最短周期为1，也就是1ms
		if (sched_tasks[index].interval_ticks < 1)
		{
			sched_tasks[index].interval_ticks = 1;
		}
	}
}
//这个函数放到main函数的while(1)中，不停判断是否有线程应该执行
void Scheduler_Run(void)
{
	uint8_t index = 0;
	//循环判断所有线程，是否应该执行

	for (index = 0; index < TASK_NUM; index++)
	{
		//获取系统当前时间，单位MS
		uint32_t tnow = GetSysRunTimeMs();
		//进行判断，如果当前时间减去上一次执行的时间，大于等于该线程的执行周期，则执行线程
		if (tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{

			//更新线程的执行时间，用于下一次判断
			sched_tasks[index].last_run = tnow;
			//执行线程函数，使用的是函数指针
			sched_tasks[index].task_func();
		}
	}
}


u8 TFMiniPosCtl(s16 expect){
	user_exp_fdb_x.exp_distance = expect;
	if(mission_step==8&&tfmini.Dist>100)user_exp_fdb_x.fdb_distance = expect;
	else user_exp_fdb_x.fdb_distance = tfmini.Dist;
	if(opmv.pole.is_invalid)user_exp_fdb_x.fdb_distance=opmv.pole.pos_y;
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
u8 OpMVPosCtl_pole(s16 expect1, s16 expect2){
	//user_exp_fdb_z.exp_distance = expect2;
	user_exp_fdb_y.exp_distance = expect1;
		
	if(!opmv.pole.flag){
		user_exp_fdb_y.fdb_distance = user_exp_fdb_y.exp_distance;
		//user_exp_fdb_z.fdb_distance = user_exp_fdb_z.exp_distance;
	}
	else{
		user_exp_fdb_y.fdb_distance = opmv.pole.Dist;
		//user_exp_fdb_z.fdb_distance = opmv.pole.pos_y;
	}

	test_output_y = GeneralPosCtl(user_exp_fdb_y, Direction_y, PID_Distance_arg_y, PID_Distance_val_y, user_threshold_y, 1);
	//test_output_z = GeneralPosCtl(user_exp_fdb_z, Direction_z, PID_Distance_arg_z, PID_Distance_val_z, user_threshold_z, 1);
	if(myabs(test_output_y)<=5)user_flag.tfmini_ctl_flag=1;
	else user_flag.tfmini_ctl_flag=0,rt_tar.st_data.vel_x=0;
	return 1;
}
u8 HWT101PosCtl(s16 expect){
	/*hwt101保证yaw轴平稳*/
	if(user_flag.yaw_set_flag){
		user_exp_fdb_yaw.exp_distance = hwt101ct.yaw_angle;
		user_flag.yaw_set_flag = 0;
	}
	
	if(hwt101ct.offline){
		user_exp_fdb_yaw.fdb_distance = user_exp_fdb_yaw.exp_distance;
	}
	else{
	/*过零点判断*/
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
		
	if(!opmv.pole.is_invalid){
		//user_exp_fdb_x.fdb_distance = user_exp_fdb_x.exp_distance;
		user_exp_fdb_yaw.fdb_distance = user_exp_fdb_yaw.exp_distance;
	}
	else{
		//user_exp_fdb_x.fdb_distance = opmv.pole.Dist;
		user_exp_fdb_yaw.fdb_distance = opmv.pole.Dist;
	}
			
	//test_output_x = GeneralPosCtl(user_exp_fdb_x, Direction_x, PID_Distance_arg_x, PID_Distance_val_x, user_threshold_pole_x, 1);
	test_output_yaw = GeneralPosCtl(user_exp_fdb_yaw, Direction_yaw, PID_Distance_arg_yaw, PID_Distance_val_yaw, user_threshold_yaw, 1);
	rt_tar.st_data.vel_y = exp_y;	
	if(opmv.pole.Dist>-30||opmv.pole.Dist<30)user_flag.tfmini_ctl_flag=1;
	else user_flag.tfmini_ctl_flag=0,rt_tar.st_data.vel_x=0;
	return 1;
}
u8 OFAltCtl(u16 expect){
	user_exp_fdb_alt_z.exp_distance = expect;
	user_exp_fdb_alt_z.fdb_distance = ano_of.of_alt_cm;	
	//user_send(0xf2,test1,test2);
	if(user_exp_fdb_alt_z.fdb_distance < 30)
		rt_tar.st_data.vel_z = 0;
	else
		test_output_alt_z = GeneralPosCtl(user_exp_fdb_alt_z, Direction_z, PID_Distance_arg_z, PID_Distance_val_z, user_threshold_alt_z, 0);
	//user_send(0xf3,Direction_z,ano_of.of_alt_cm);
	//user_send(0xf3,test_output_alt_z,ano_of.of_alt_cm);
	return 1;
}
u8 DataClr(void){
	
	RealTimeSpeedControl(0, Direction_x);
	RealTimeSpeedControl(0, Direction_y);
	RealTimeSpeedControl(0, Direction_z);
	RealTimeSpeedControl(0, Direction_yaw);
	Position_incre = 0;
	Position_pre = 0;
	pos_incre = 0;
	pos_pre = 0;
	cnt = 0;
	//RealTimeSpeedControlSend(0, Direction_yaw);
	
	return 1;
}	

	//任务集
u8 TaskSet(s16 dT){
	int now=0;
	int cnt2=0;
	switch(mission_step)
	{
		case 0:
			cnt=0;
			break;
		case 1:
			FC_Unlock();
			cnt+=dT;
			if(timejudge(3))
			mission_step+=1,cnt=0;
			break;
		case 2:
			mission_step += OneKey_Takeoff(100);
			break;
		case 3:
			user_flag.yaw_set_flag = 1;
			Position_incre = 0;
			Position_pre = 0;
			cnt+=dT;
			if(timejudge(3))
				mission_step += 1,cnt=0;
			break;
		case 4:
			user_flag.of_alt_ctl_flag = 1;
			if(ano_of.of_alt_cm>125&&ano_of.of_alt_cm<135)
			{
				cnt+=dT;
			}
			else cnt=0;
			if(timejudge(3))
			{
				mission_step++;
				DataClr();
				user_flag.yaw_set_flag = 1;
				cnt=0;
			}
			break;
		case 5:
			opmv.mode_cmd[1] = 5; 
			user_flag.yaw_set_flag=0;
			RealTimeSpeedControl(20, Direction_yaw);
			if(opmv.pole.flag==1){
				cnt+=dT;
			}
			else cnt=0;
			
			if(cnt>=300)
			{
				DataClr();
				user_flag.yaw_set_flag=1;
				mission_step += 1;
				cnt=0;
			}
			if(now!=0)
			{
				mission_step=now;
				now=0;
			}
			break;
		case 6:
			user_flag.opmv_ctl_flag=1;
			if(opmv.pole.pos_y>40&&opmv.pole.pos_y<60)
			{
				cnt+=dT;cnt2=0;
			}
			else cnt=0,cnt2+=dT;
			if(cnt2>=5000)
			{
				cnt2=0;
				mission_step=5;
				now=6;
				user_flag.opmv_ctl_flag=0;
				break;
			}
			if(timejudge(2))
			{
				mission_step+=1;
				user_flag.opmv_ctl_flag=0;
				cnt=0;
			}
			break;
		case 7:
			pos_now = hwt101ct.yaw_angle;
			pos_incre = ZeroPointCross(pos_now, pos_pre, pos_incre);
			pos_pre = pos_now;
			pos_start = pos_incre;
			user_flag.yaw_set_flag=0;
			mission_step += 1;	
			break;
		case 8:
			opmv.mode_cmd[1] = 5; 
			if(opmv.mode_sta == 5)
				user_flag.pole_ctl_flag=1;
			pos_now = hwt101ct.yaw_angle;
			pos_incre = ZeroPointCross(pos_now, pos_pre, pos_incre);
			pos_pre = pos_now;
		if(UserAbs(pos_incre - pos_start) > 360){
				mission_step += 1;
				cnt=0;	
				pos_incre = 0;
				pos_pre = 0;
				DataClr();
				user_flag.yaw_set_flag = 1;
				user_flag.pole_ctl_flag=0;
			}
			break;
		
		case 9:
			/*yaw轴自稳，以-10cm/s速度沿x轴后退3s*/
			RealTimeSpeedControl(-20, Direction_x);
			HWT101PosCtl(0);
			
			cnt += dT;
			if(timejudge(3)){
				cnt = 0;
				DataClr();
				mission_step += 1;
			}
			break;
			
		case 10:
			user_flag.of_alt_ctl_flag = 2;
			cnt += dT;
			if(timejudge(3)){
				cnt = 0;
				DataClr();
				mission_step += 1;
			}
			break;
		case 11:
			FC_Lock();
			mission_step =0;
		default:
			break;
	}
}
u8 taskset2(s16 dT)
{
		int now=0;
	int cnt2=0;
	switch(mission_step)
	{
		case 0:
			cnt=0;
			break;
		case 1:
			FC_Unlock();
			cnt+=dT;
			if(timejudge(3))
			mission_step+=1,cnt=0;
			break;
		case 2:
			mission_step += OneKey_Takeoff(100);
			break;
		case 3:
			user_flag.yaw_set_flag = 1;
			Position_incre = 0;
			Position_pre = 0;
			cnt+=dT;
			if(timejudge(3))
				mission_step += 1,cnt=0;
			break;
		case 4:
			user_flag.of_alt_ctl_flag = 1;
			if(ano_of.of_alt_cm>125&&ano_of.of_alt_cm<135)
			{
				cnt+=dT;
			}
			else cnt=0;
			if(timejudge(3))
			{
				mission_step++;
				DataClr();
				user_flag.yaw_set_flag = 1;
				cnt=0;
			}
			break;
		case 5:
			RealTimeSpeedControl(50, Direction_yaw);
	    RealTimeSpeedControl(-50, Direction_y);
		  cnt+=dT;
			if(timejudge(10)){
			   mission_step++;
			   DataClr();
				 user_flag.yaw_set_flag = 1;
				 cnt=0;				
				}
			break;
		case 6:
			user_flag.of_alt_ctl_flag = 0;
			mission_step += OneKey_Land();		
			break;
		case 7:
			cnt+=dT;
			if(timejudge(5)){
				mission_step += 1,cnt=0;
			}
			break;
		case 8:
			FC_Lock();
			mission_step =0;
		default:
			break;
	}
	
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


