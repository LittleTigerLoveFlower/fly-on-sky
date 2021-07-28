#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include "SysConfig.h"
#include "User_Task.h"
typedef struct
{
void(*task_func)(void);
uint16_t rate_hz;
uint16_t interval_ticks;
uint32_t last_run;
}sched_task_t;

/*xy�ٶȻ�����*/
extern _PID_arg_st PID_Speed_arg_x; 
extern _PID_arg_st PID_Speed_arg_y; 
/*xy�ٶȻ�λ��*/
extern _PID_val_st PID_Speed_val_x;
extern _PID_val_st	PID_Speed_val_y;

/*xyλ�û�����*/
extern _PID_arg_st PID_Distance_arg_x; 
extern _PID_arg_st PID_Distance_arg_y;
/*xyλ�û�����*/
extern _PID_val_st PID_Distance_val_x;
extern _PID_val_st	PID_Distance_val_y;

/*yaw�ٶȻ�����*/
extern _PID_arg_st PID_Speed_arg_yaw;
/*yaw�ٶȻ�����*/
extern _PID_val_st PID_Speed_val_yaw;

/*yawλ�û�����*/
extern _PID_arg_st PID_Distance_arg_yaw;
/*yawλ�û�����*/
extern _PID_val_st PID_Distance_val_yaw;

/*z�ٶȻ�����*/
extern _PID_arg_st PID_Speed_arg_z;
/*z�ٶȻ�����*/
extern _PID_val_st PID_Speed_val_z;

/*zλ�û�����*/
extern _PID_arg_st PID_Distance_arg_z;
/*zλ�û�����*/
extern _PID_val_st PID_Distance_val_z;

/*flag��*/
typedef struct
{
	u8 of_dis_clear_cmd; //���λ������
	u8 init_pid_flag; //pid��ʼ����־λ
	u8 tfmini_ctl_flag;
	u8 openmv_clr_flag;
} _user_flag_set;

extern _user_flag_set user_flag;

void Init_PID(void);
void Scheduler_Setup(void);
void Scheduler_Run(void);

#endif

