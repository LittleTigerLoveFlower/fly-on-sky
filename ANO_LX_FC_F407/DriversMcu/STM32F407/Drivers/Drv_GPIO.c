/******************** (C) COPYRIGHT 2021 Lcmf Tech ********************************
 * ����    ��lxf
 * ����    ��IO������
**********************************************************************************/
#include "Drv_GPIO.h"

void Drv_GPIO_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	 //ʹ��PD�˿�ʱ��
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		        
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //��������Ϊ����ģʽ//
    GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��
    
    GPIO_SetBits(GPIOD,GPIO_Pin_12);						 //PD12 �����
    GPIO_ResetBits(GPIOD,GPIO_Pin_13);						 //PD13 �����
}
