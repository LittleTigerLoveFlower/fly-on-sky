#ifndef __USER_TASK_H
#define __USER_TASK_H

#include "SysConfig.h"
#include "stm32f4xx.h"

#define safe_div(numerator,denominator,safe_value) ( (denominator == 0)? (safe_value) : ((numerator)/(denominator)) )
#define LIMIT( x,min,max ) ( ((x) <= (min)) ? (min) : ( ((x) > (max))? (max) : (x) ) )
#define SAFE_SPEED 200

//pid�����ṹ��
typedef struct
{
	u8 fb_d_mode;
	float kp;			 //����ϵ��
	float ki;			 //����ϵ��
	float kd_ex;		 	 //΢��ϵ��
	float kd_fb; //previous_d ΢������
//	float inc_hz;  //����ȫ΢�ֵ�ͨϵ��
//	float k_inc_d_norm; //Incomplete ����ȫ΢�� ��һ��0,1��
	float k_ff;		 //ǰ�� 

}_PID_arg_st;

//pid���ݽṹ��
typedef struct
{
	float err;
	float exp_old;
	float feedback_old;
	
	float fb_d;
	float fb_d_ex;
	float exp_d;
//	float err_d_lpf;
	float err_i;
	float ff;
	float pre_d;

	float out;
}_PID_val_st;

void UserTask_OneKeyCmd(void);
u8 Vertical_Up(u16 height_cm, u16 velocity_cms);
float PID_calculate( float dT_s,            //���ڣ���λ���룩
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
u8 MPU6050_Tack(void);

u8 RealTimeSpeedControl(s16 velocity, u8 direction);
u8 RealTimeSpeedControlSend(s16 velocity, u8 direction);
u8 RealTimeSpeedControl_Angle(s16 velocity, u8 direction, u16 degree);

#endif
