void fuck(){
	switch(mission_step){
		case 0:
			cnt = 0;
			break;
		
		case 1:
			/*�ɿؽ��� */
			 FC_Unlock();
		
			/*3s��ʱ*/
			cnt += dT;
			if(cnt >= 3000){
				cnt = 0;
				mission_step += 1;
			}
			break;
			
		case 2:
			/*һ�����*/
			mission_step += OneKey_Takeoff(130);
			break;
		
		case 3:
			/*����yaw������*/
			user_flag.yaw_set_flag = 1;
			Position_incre = 0;
			Position_pre = 0;
			cnt+=dT;
			if(cnt>=6000)
			mission_step += 2;
			break;
		
		case 4:
			/*ʶ��ͣ��ƺ��Ħ��������ͣ8s*/
			opmv.mode_cmd[1] = 4; //Ħ����ʶ��ģʽ
			HWT101PosCtl(0);
			
			/*��mv���ص�ģʽΪĦ����ʱ��������λ������ͬʱ��ʱ*/
			if(opmv.mode_sta == 4){
				OpMVPosCtl_Down(0, 0);
				cnt += dT;
			}
			else{
				DataClr();
			}
			
			if(cnt >= 800){
				cnt = 0;
				mission_step += 1;
				DataClr();
				user_flag.yaw_set_flag = 1;
			}
			break;
			
		case 5:
			/*���ߵ�130cm*/
			//HWT101PosCtl(0);
			//RealTimeSpeedControl(10, Direction_x);
			OFAltCtl(160);
			if(ano_of.of_alt_cm > 140 && ano_of.of_alt_cm < 180){
				cnt += dT;
			}
			else{
				cnt = 0;
			}
			
			if(cnt >= 20000){
				cnt = 0;
				mission_step += 1;
				user_flag.yaw_set_flag = 1;
				DataClr();
			}
			break;
		
		case 6:
			HWT101PosCtl(0);
			/*��10cm/s�ٶ���ǰ�ƶ�10s*/
			//RealTimeSpeedControl(10, Direction_x);
			opmv.mode_cmd[1] = 5; //��ɫ��ʶ��ģʽ
			cnt += dT;
		
			/*ûʶ��Ŀ��*/
			if(cnt >= 10000){
				cnt = 0;
				mission_step += 4;
				DataClr();
			}
			
			/*ʶ��Ŀ��*/
			if(!opmv.pole.is_invalid){
				DataClr();
				mission_step = 8;
			}
			break;
			
		/*δʶ�𵽸�*/
		case 7:
			/*ԭ������ֱ��ʶ�𵽸�*/
			opmv.mode_cmd[1] = 5; //��ɫ��ʶ��ģʽ
			RealTimeSpeedControl(15, Direction_yaw);
			if(!opmv.mol.is_invalid){
				DataClr();
				mission_step += 1;
			}
			break;
		
		/*ʶ�𵽸�*/
		case 8:
			/*��¼��ʼʱ��yaw�Ƕ�*/
			pos_now = hwt101ct.yaw_angle;
			pos_incre = ZeroPointCross(pos_now, pos_pre, pos_incre);
			pos_pre = pos_now;
			pos_start = pos_incre;
			mission_step += 1;	
			break;
		
		case 9:
			opmv.mode_cmd[1] = 5; //��ɫ��ʶ��ģʽ
			if(opmv.mode_sta == 5)
				PolePosCtl(50, -10, 0);
			pos_now = hwt101ct.yaw_angle;
			pos_incre = ZeroPointCross(pos_now, pos_pre, pos_incre);
			pos_pre = pos_now;
			
			if(UserAbs(pos_incre - pos_start) > 360){
				mission_step += 1;
				pos_incre = 0;
				pos_pre = 0;
				DataClr();
				user_flag.yaw_set_flag = 1;
			}
			break;
		
		case 10:
			/*yaw�����ȣ���-10cm/s�ٶ���x�����3s*/
			//RealTimeSpeedControl(-10, Direction_x);
			HWT101PosCtl(0);
			
			cnt += dT;
			if(cnt > 3000){
				cnt = 0;
				DataClr();
				mission_step += 1;
			}
			break;
			
		case 11:
			OneKey_Land();
			mission_step = 0;
			break;
		default:
			break;
	}
	
	return 1;
}