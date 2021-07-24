#include "User_Task.h"
#include "Drv_RcIn.h"
#include "LX_FC_Fun.h"
#include "ANO_DT_LX.h"

s16 THR_Val = 800;
s16 CTRL_SPD_Z = 10;

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
								one_key_takeoff_f = 1;
	//							mission_task = 1;
	//							mission_step = 1;
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
								
									/*ʵʱ֡����*/
										rt_tar.st_data.vel_x = 25;
										dt.fun[0x41].WTS = 1;
										mission_step += 1;
									
									/*����֡����*/
									//mission_step += Horizontal_Move(40, 20, 0); //��20cm/s���ٶ���ǰ�ƶ�40cm
									
									//mission_step += FC_Unlock(); //����
									//LX_Change_Mode(3);
									break;
							case 2: 
									/*��ʱ10s*/
									
									if(_cnt < 100){
											if(_cnt % 10);
											else{
												rt_tar.st_data.vel_x = 25;
												dt.fun[0x41].WTS = 1;
											}
											_cnt++;
									}
									else{
											_cnt = 0;
											rt_tar.st_data.vel_x = 0;
											dt.fun[0x41].WTS = 1;
											mission_step += 1;
											//mission_step += OneKey_Takeoff(40);
									}
									break;
									
							case 3:
									/*��ʱ10s*/
									if(_cnt < 500)
											_cnt++;
									else{
											_cnt = 0;
											OneKey_Land();
											mission_step = 0;
											/*����20cm���ٶ�10cm/s*/
											//mission_step += Vertical_Up(10, 10);
											
											
											//mission_step += 1;
									}
									break;
							case 4:
									/*��ʱ2s*/
									
//									if(_cnt < 100)
//											_cnt++;
//									else{
//											_cnt = 0;
//									}
//									mission_step += 1;
									break;
							case 5:
									mission_step = 0;
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
