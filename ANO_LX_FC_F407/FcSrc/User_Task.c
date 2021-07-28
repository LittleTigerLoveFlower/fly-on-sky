#include "User_Task.h"
#include "Drv_RcIn.h"
#include "LX_FC_Fun.h"
#include "ANO_DT_LX.h"
#include "Ano_Math.h"
#include "Drv_TFMini_Plus.h"
#include "Drv_OpenMV.h"
#include "Ano_Scheduler.h"
#define TF_EXPECT_DIST  150.0f //��������
#define NORMALIZE_DIST  500.0f //������ֵ
#define NORMALIZE_SPEED 20.0f  //xy�����ٶ���ֵ
#define NORMALIZE_SPEED_YAW 30.0f //yaw�����ٶ���ֵ

#define MAX_SPEED_XY 20           //�������ٶ�
#define MAX_SPEED_Z 10
#define MAX_SPEED_YAW 10 //yaw�����������ٶ�

#define NORMALIZE_MV_DIST_Y 40.0f
#define NORMALIZE_MV_DIST_Z 35.0f
#define NORMALIZE_MPU_ANGLE_YAW 200.0f //yaw��Ƕȹ�һ��

s16 THR_Val = 800;
s16 CTRL_SPD_Z = 10;
s16 out_speed = 0;
s16 out_speed_y = 0;
s16 out_speed_z = 0;


enum AxialDirection axialdirection;

//20msִ��һ��
void UserTask_OneKeyCmd(void)
{
    //////////////////////////////////////////////////////////////////////
    //һ�����/��������
    //////////////////////////////////////////////////////////////////////
    //�þ�̬������¼һ�����/����ָ���Ѿ�ִ�С�
    static u8 one_key_takeoff_f = 1, one_key_land_f = 1, one_key_mission_f = 0;
    static u8 mission_step; 
		static u8 mission_task = 0;
		static u16 _cnt = 0;
    //�ж���ң���źŲ�ִ��
    if (rc_in.no_signal == 0)
    {
				
        //�жϵ�6ͨ������λ�� 1300<CH_6<1700
        if (rc_in.rc_ch.st_data.ch_[ch_6_aux2] > 1300)
        {
            //��û��ִ��
            if (one_key_takeoff_f == 0)
            {
//								one_key_takeoff_f = 1;
//								mission_task = 1;
//								mission_step = 1;
								user_flag.tfmini_ctl_flag = 1;
								user_flag.openmv_clr_flag = 0;
            }
						
        }
        else
        {
            //��λ��ǣ��Ա��ٴ�ִ��
            one_key_takeoff_f = 0;
        }
				
        //
        //�жϵ�6ͨ������λ�� 800<CH_6<1200
        if (rc_in.rc_ch.st_data.ch_[ch_6_aux2] > 800 && rc_in.rc_ch.st_data.ch_[ch_6_aux2] < 1200)
        {
            //��û��ִ��
            if (one_key_land_f == 0)
            {
                //����Ѿ�ִ��
                //Add_Send_Data(0x41, );
								//FC_Lock();
//								OneKey_Hold();
								user_flag.tfmini_ctl_flag = 0;
								user_flag.openmv_clr_flag = 1;
							  mission_task = 0;
								mission_step = 0;
								one_key_land_f = 1;
								_cnt = 0;
								
            }
        }
        else
        {
            //��λ��ǣ��Ա��ٴ�ִ��
            one_key_land_f = 0;
        }
				
				if(mission_task){
						switch(mission_step){
							case 0:
									break;
							case 1:
									
								
									break;
							case 2: 
								
									break;
									
							case 3:
						
									break;
							case 4:
							
									break;
							case 5:

									break;
							default:
									break;
						}
				}
			}
    ////////////////////////////////////////////////////////////////////////
}

//pid���㺯��
float PID_calculate( float dT_s,            //���ڣ���λ���룩
										float in_ff,				//ǰ��ֵ
										float expect,				//����ֵ���趨ֵ��
										float feedback,			//����ֵ����
										_PID_arg_st *pid_arg, //PID�����ṹ��
										_PID_val_st *pid_val,	//PID���ݽṹ��
										float inte_d_lim,//��������޷�
										float inte_lim			//integration limit�������޷�									
										 )
{
	float differential,hz;
	hz = safe_div(1.0f,dT_s,0);
	
//	pid_arg->k_inc_d_norm = LIMIT(pid_arg->k_inc_d_norm,0,1);
	

	
	pid_val->exp_d = (expect - pid_val->exp_old) *hz;
	
	if(pid_arg->fb_d_mode == 0)
	{
		pid_val->fb_d = (feedback - pid_val->feedback_old) *hz;
	}
	else
	{
		pid_val->fb_d = pid_val->fb_d_ex;
	}	
	differential = (pid_arg->kd_ex *pid_val->exp_d - pid_arg->kd_fb *pid_val->fb_d);
	
	pid_val->err = (expect - feedback);	

	pid_val->err_i += pid_arg->ki *LIMIT((pid_val->err ),-inte_d_lim,inte_d_lim )*dT_s;//)*T;//+ differential/pid_arg->kp
	//pid_val->err_i += pid_arg->ki *(pid_val->err )*T;//)*T;//+ pid_arg->k_pre_d *pid_val->feedback_d
	pid_val->err_i = LIMIT(pid_val->err_i,-inte_lim,inte_lim);
	
	
	
	pid_val->out = pid_arg->k_ff *in_ff 
	    + pid_arg->kp *pid_val->err  
			+	differential
//	    + pid_arg->k_inc_d_norm *pid_val->err_d_lpf + (1.0f-pid_arg->k_inc_d_norm) *differential
    	+ pid_val->err_i;
	
	pid_val->feedback_old = feedback;
	pid_val->exp_old = expect;
	
	return (pid_val->out);
}

/*
*@fn:			u8 TFMini_Track(void)
*@brief:	����TFmini����x�᷽��ľ��룬ʵ��x�������λ
*@para:		none
*@return:	1
*@comment:
*/
u8 TFMini_Track(void){
    //�������������������й�һ������
	float fdb_distance = tfmini.Dist > NORMALIZE_DIST? 1.0f: tfmini.Dist / NORMALIZE_DIST;
	float exp_distance = TF_EXPECT_DIST / NORMALIZE_DIST;
    
	float fdb_speed = 0;
	float exp_speed = 0;
	s16 _out_speed = 0;
	//λ�û�pid����
	PID_calculate(0.02, 0, exp_distance, fdb_distance, &PID_Distance_arg_x, &PID_Distance_val_x, 0, 0);
    //λ�û�pid�����Ϊ�ٶȻ��������ٶ�����
	exp_speed = PID_Distance_val_x.out * -1;
    
    //�������������һ������ó�ʵ���ٶ�����
	if(ano_of.of2_dx > NORMALIZE_SPEED)
		fdb_speed = 1.0f;
	else if(ano_of.of2_dx < -1 * NORMALIZE_SPEED)
		fdb_speed = -1.0f;
	else fdb_speed = (float)ano_of.of2_dx / NORMALIZE_SPEED;
	
    //�ٶȻ�pid����
	PID_calculate(0.02, 0, exp_speed, fdb_speed, &PID_Speed_arg_x, &PID_Speed_val_x, 0, 0);
    
	_out_speed = PID_Speed_val_x.out * NORMALIZE_SPEED;
	
    //����ٶ���λ
	if(_out_speed > MAX_SPEED_XY)
		out_speed = MAX_SPEED_XY;
	else if(_out_speed < -1 * MAX_SPEED_XY)
		out_speed = -1 * MAX_SPEED_XY;
	else 
		out_speed = _out_speed;
	
    //���Ͷ�Ӧ���ָ��
	RealTimeSpeedControlSend(out_speed, Direction_x);
	
	return 1;
}

/*
*@fn:			u8 MPU6050_TRACK(void)
*@brief:	����MPU6050����yaw��ƫ�ƣ�ʵ��yaw�ᶨλ
*@para:		none
*@return:	1
*@comment:
*/
//u8 MPU6050_Track(void){
//	//�������������������й�һ������
//	float fdb_angle = mpu6050.yaw.angle / NORMALIZE_MPU_ANGLE_YAW;
//	float exp_angle = 0;
//    
//	float fdb_speed = 0;
//	float exp_speed = 0;
//	s16 _out_speed = 0;
//	
//	//λ�û�pid����
//	PID_calculate(0.02, 0, exp_angle, fdb_angle, &PID_Distance_arg_yaw, &PID_Distance_val_yaw, 0, 0);
//  //λ�û�pid�����Ϊ�ٶȻ��������ٶ�����
//	exp_speed = PID_Distance_val_yaw.out;
//    
//  //���ٶȷ���ֵ��һ��
//	if(mpu6050.yaw.speed > NORMALIZE_SPEED_YAW)
//		fdb_speed = 1.0f;
//	else if(mpu6050.yaw.speed < -1 * NORMALIZE_SPEED_YAW)
//		fdb_speed = -1.0f;
//	else
//		fdb_speed = mpu6050.yaw.speed / NORMALIZE_SPEED_YAW;
//	
//  //�ٶȻ�pid����
//	PID_calculate(0.02, 0, exp_speed, fdb_speed, &PID_Speed_arg_yaw, &PID_Speed_val_yaw, 0, 0);
//    
//	_out_speed = PID_Speed_val_yaw.out * NORMALIZE_SPEED_YAW;
//	
//  //����ٶ���λ
//	if(_out_speed > MAX_SPEED_YAW)
//		out_speed = MAX_SPEED_YAW;
//	else if(_out_speed < -1 * MAX_SPEED_YAW)
//		out_speed = -1 * MAX_SPEED_YAW;
//	else 
//		out_speed = _out_speed;
//	
//    //���Ͷ�Ӧ���ָ��
//	RealTimeSpeedControlSend(out_speed, Direction_yaw);
//	
//	return 1;
//}

/*
*@fn:			u8 OpenMV_Track(void)
*@brief:	����OpenMV_Track������yzƽ������ͷ���ľ��ǩ�ľ��룬ʵ��yz����Ķ�λ
*@para:		none
*@return:	1
*@comment:
*/
u8 OpenMV_Track(void){
	//�ж�openmv����ֵ�Ƿ���Ч
	if(opmv.at.is_invalid){
		out_speed_y = 0;
		out_speed_z = 0;
		RealTimeSpeedControlSend(out_speed_y, Direction_y);
		RealTimeSpeedControlSend(out_speed_z, Direction_z);
		return 0;
	}
		
	//����openmv���ص���������һ������
	//y����
	float fdb_distance_y = opmv.at.pos_y / NORMALIZE_MV_DIST_Y; //NORMALIZE_MV_DIST_Y = 80.0f
	float exp_distance_y = 0;
	float fdb_speed_y = 0;
	float exp_speed_y = 0;
	s16 _out_speed_y = 0;
	//z����
	float fdb_distance_z = opmv.at.pos_z / NORMALIZE_MV_DIST_Z; //NORMALIZE_MV_DIST_Z = 60.0f
	float exp_distance_z= 0;
	float fdb_speed_z = 0;
	float exp_speed_z = 0;
	s16 _out_speed_z = 0;
	
  //λ�û�pid����
	PID_calculate(0.02, 0, exp_distance_y, fdb_distance_y, &PID_Distance_arg_y, &PID_Distance_val_y, 0, 0);
	PID_calculate(0.02, 0, exp_distance_z, fdb_distance_z, &PID_Distance_arg_z, &PID_Distance_val_z, 0, 0);
	//���ȡ��
	exp_speed_y = PID_Distance_val_y.out * -1;
	exp_speed_z = PID_Distance_val_z.out * -1;
	
	//��ֵ����
	if(ano_of.of2_dy > NORMALIZE_SPEED)
		fdb_speed_y = 1.0f;
	else if(ano_of.of2_dy < -1 * NORMALIZE_SPEED)
		fdb_speed_y = -1.0f;
	else fdb_speed_y = (float)ano_of.of2_dy / NORMALIZE_SPEED;
	
	//�ٶȻ�����(Z����ֻ��λ�û�)
	PID_calculate(0.02, 0, exp_speed_y, fdb_speed_y, &PID_Speed_arg_y, &PID_Speed_val_y, 0, 0);
	//PID_calculate(0.02, 0, exp_speed_z, fdb_speed_z, &PID_Speed_arg_z, &PID_Speed_val_z, 0, 0);
	_out_speed_y = PID_Speed_val_y.out * NORMALIZE_SPEED;
	_out_speed_z = exp_speed_z * NORMALIZE_SPEED;
	
  //����ٶ���λ
	//y����
	if(_out_speed_y > MAX_SPEED_XY)
		out_speed_y = MAX_SPEED_XY;
	else if(_out_speed_y < -1 * MAX_SPEED_XY)
		out_speed_y = -1 * MAX_SPEED_XY;
	else 
		out_speed_y = _out_speed_y;
	//z����
	if(_out_speed_z > MAX_SPEED_Z)
		out_speed_z = MAX_SPEED_Z;
	else if(_out_speed_z < -1 * MAX_SPEED_Z)
		out_speed_z = -1 * MAX_SPEED_Z;
	else 
		out_speed_z = _out_speed_z;
	
  //���Ͷ�Ӧ���ָ��
	RealTimeSpeedControlSend(out_speed_y, Direction_y);
	RealTimeSpeedControlSend(out_speed_z, Direction_z);
	
	return 1;
}

/*
*@fn:			u8 RealTimeSpeedControl(s16 velocity, u8 direction)
*@brief:	ͨ��ʵʱ����֡����ָ������(x��y��z��)���ٶ�
*@para:		s16 velocity ƽ���ٶ�, u8 direction ƽ�Ʒ���
*@return:	1
*@comment:
*/
u8 RealTimeSpeedControl(s16 velocity, u8 direction){
//	if(velocity > SAFE_SPEED)
//		velocity = SAFE_SPEED;
	
	switch(direction){
		case Direction_x:
			rt_tar.st_data.vel_x = velocity;
			break;
		case Direction_y:
			rt_tar.st_data.vel_y = velocity;
			break;
		case Direction_z:
			rt_tar.st_data.vel_z = velocity;
			break;
		case Direction_yaw:
			rt_tar.st_data.yaw_dps = velocity;
		default:
			break;
	}
	return 1;
}

/*
*@fn:			u8 RealTimeSpeedControlSend(s16 velocity, u8 direction)
*@brief:	ͨ��ʵʱ����֡����ָ������(x��y��z��)���ٶȣ������Ϳ���ָ��
*@para:		s16 velocity ƽ���ٶ�, u8 direction ƽ�Ʒ���
*@return:	1
*@comment:
*/
u8 RealTimeSpeedControlSend(s16 velocity, u8 direction){
	RealTimeSpeedControl(velocity, direction);
	dt.fun[0x41].WTS = 1;
	
	return 1;
}

/*
*@fn:			u8 RealTimeSpeedControl_Angle(s16 velocity, u8 direction, u16 degree)
*@brief:	ͨ��ʵʱ����֡����ָ��ƽ������Ƕȵ�ƽ���ٶȣ������Ϳ���ָ��
*@para:		s16 velocity ƽ���ٶ�, u8 direction ƽ��ƽ��, u16 degree ƽ�ƽǶ�(��λ����)
*@return:	1
*@comment:���ڲ�ͬƽ��Ƕ�ֵ�ķ�����
					xy	�Էɻ�ΪԲ�ģ�x��������Ϊ��㣬��y���������ƶ�Ϊ��
					yz	�Էɻ�ΪԲ�ģ�y��������Ϊ��㣬��z���������ƶ�Ϊ��
					xz	�Էɻ�ΪԲ�ģ�x��������Ϊ��㣬��z���������ƶ�Ϊ��
*/
u8 RealTimeSpeedControl_Angle(s16 velocity, u8 direction, u16 degree){
	if(direction == Direction_xy){
		RealTimeSpeedControl(velocity * my_cos(degree / 360 * 3.14159265), Direction_x);
		RealTimeSpeedControl(velocity * my_sin(degree / 360 * 3.14159265), Direction_y);
	}
	else if(direction == Direction_yz){
		RealTimeSpeedControl(velocity * my_cos(degree / 360 * 3.14159265), Direction_y);
		RealTimeSpeedControl(velocity * my_sin(degree / 360 * 3.14159265), Direction_z);
	}
	else if(direction == Direction_xz){
		RealTimeSpeedControl(velocity * my_cos(degree / 360 * 3.14159265), Direction_x);
		RealTimeSpeedControl(velocity * my_sin(degree / 360 * 3.14159265), Direction_z);
	}
	dt.fun[0x41].WTS = 1;
	
	return 1;
}



