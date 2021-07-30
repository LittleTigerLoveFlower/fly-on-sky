#ifndef __USER_TASK_H
#define __USER_TASK_H

#include "SysConfig.h"
#include "stm32f4xx.h"
#include "Ano_Scheduler.h"

#define safe_div(numerator,denominator,safe_value) ( (denominator == 0)? (safe_value) : ((numerator)/(denominator)) )
#define LIMIT( x,min,max ) ( ((x) <= (min)) ? (min) : ( ((x) > (max))? (max) : (x) ) )
#define SAFE_SPEED 200

void UserTask_OneKeyCmd(void);
u8 Vertical_Up(u16 height_cm, u16 velocity_cms);
extern float PID_calculate( float dT_s,            //���ڣ���λ���룩
										float in_ff,				//ǰ��ֵ
										float expect,				//����ֵ���趨ֵ��
										float feedback,			//����ֵ����
										_PID_arg_st *pid_arg, //PID�����ṹ��
										_PID_val_st *pid_val,	//PID���ݽṹ��
										float inte_d_lim,//��������޷�
										float inte_lim			//integration limit�������޷�									
										 );

extern s16 out_speed;
extern s16 out_speed_y;
extern s16 out_speed_z;

enum AxialDirection{
	Direction_x = 1,
	Direction_y,
	Direction_z,
	Direction_yaw,
	Direction_xy,
	Direction_yz,
	Direction_xz
};
extern enum AxialDirection axialdirection;


u8 TFMini_Track(void);
u8 OpenMV_Track(void);
u8 OpenMV_Circle_Track(void);
u8 HWT101CT_TRACK(void);
float UserNormalize(float num, float max, float min);
s16 GeneralPosCtl(_user_exp_fdb_set exp_fdb, 		//��������������뷴��
									u8 direction, 								//���Ʒ���
									_PID_arg_st speed_arg, 			//pid�ٶȲ����ṹ��
									_PID_val_st speed_val,			//pid�ٶ����ݽṹ��
									_user_threshold_set threshold,//��һ���������ֵ
									u8 invert											//���ȡ��(1��-1)
								);
	
u8 RealTimeSpeedControl(s16 velocity, u8 direction);
u8 RealTimeSpeedControlSend(s16 velocity, u8 direction);
u8 RealTimeSpeedControl_Angle(s16 velocity, u8 direction, u16 degree);

#endif
