#ifndef __EIE3810_ZHUXIDEBUG_H
#define __EIE3810_ZHUXIDEBUG_H

#include "stm32f10x.h"

// put header here

// zhuxidebug.c ר����������һЩ��ʱ��ӵ�����   ��ѭЭ��  �� �ƿ� �ƶ���Э��  ������� �ļ���

extern u8 Step1_Start_Flag;
extern u8 Step2_Start_Flag;
extern u8 Step3_Start_Flag;
extern u8 Step4_Start_Flag;

extern u8 Step1_Done_Flag;
extern u8 Step2_Done_Flag;
extern u8 Step3_Done_Flag;


extern u8 Open_Door_Relay_Flag_Nolink;
extern u8 Motor_RunOpen_Flag_Nolink;
extern u8 Close_Door_Relay_Flag_Nolink;
extern u8 Motor_RunClose_Flag_Nolink;

extern u8 Step_Run_Flag_Nolink;
extern u8 Stepmotor1_Reset_Flag_Nolink;

extern u8 Pump_Jinyang_Flag_Nolink;




void Multi_Jinyang_Process(void);

void 	Step1_Process(void);
void	Step2_Process(void);
void	Step3_Process(void);

void Open_Door_Relay_Nolink(void);
void Open_Motor1_Run_Nolink(void);

void Close_Door_Relay_Nolink(void);
void Close_Motor1_Run_Nolink(void);

void StepMotor1_Run_Nolink(void);
void StepMotor_Reset_Nolink(void);

void Pump_Jinyang_Nolink(void);






#endif
