#include "stm32f10x.h"
#include "EIE3810_zhuxidebug.h"
#include "EIE3810_USART.h"
#include "EIE3810_LED.h"
#include "EIE3810_Timer.h"
#include "EIE3810_Key.h"
#include "EIE3810_zhuxi.h"

// put your procedure and code here

u8 Step1_Start_Flag;
u8 Step2_Start_Flag;
u8 Step3_Start_Flag;
u8 Step4_Start_Flag;

u8 Step1_Done_Flag;
u8 Step2_Done_Flag;
u8 Step3_Done_Flag;


u8 Open_Door_Relay_Flag_Nolink;
u8 Motor_RunOpen_Flag_Nolink;
u8 Close_Door_Relay_Flag_Nolink;
u8 Motor_RunClose_Flag_Nolink;

u8 Step_Run_Flag_Nolink;
u8 Stepmotor1_Reset_Flag_Nolink;

u8 Pump_Jinyang_Flag_Nolink;






void Step1_Process(void)
{
	if(Step1_Start_Flag)
	{
		switch(Step1)
		{
			case 0:
				;                                  // Һ��1 ��־λ   �����䶯��û�н�                 
			break;
		  
			case 1:
					;                                // Һ��2 ��־λ   �����䶯��û�н�
				break;
			case 2:
				Pump_Jinyang_Flag_Nolink=1;        // Һ��3 ��־λ   �����䶯��  ֻ���� ��һ·
				break;
			case 3:
				;                                  // Һ��4 ��־λ    �����䶯��û�н�
			break;
			
			
			case 4:                              // ����1 ��־λ   ���ڲ������Ĭ������һ·
				Step_Run_Flag_Nolink=1;
			break;
			case 5:
				;                                  // ����2 ��־λ
			break;
			
			case 6:
				;                                  // ����3 ��־λ
			break; 
			case 7:
				;                                  // ����4 ��־λ
			break;
		
		  default:
				break;
		}	
			
		
	 Step1_Start_Flag=0;		
	 Step2_Start_Flag=1;	
	}	
	
}	


void Step2_Process(void)
{
	if(Step2_Start_Flag)
	{		
		switch(Step2)
		{
			case 0:
				;                                 // Һ��1 ��־λ   �����䶯��û�н�                 
			break;
			case 1:
				;                                 // Һ��2 ��־λ   �����䶯��û�н�
			break;
			case 2:
				Pump_Jinyang_Flag_Nolink=1;        // Һ��3 ��־λ   �����䶯��  ֻ���� ��һ·
			break;
			case 3:
				;                                  // Һ��4 ��־λ    �����䶯��û�н�
			break;
			
			
			case 4:                              // ����1 ��־λ   ���ڲ������Ĭ������һ·
				Step_Run_Flag_Nolink=1;
			break;
			case 5:
				;                                  // ����2 ��־λ
			break;
			case 6:
				;                                  // ����3 ��־λ
			break; 
			case 7:
				;                                  // ����4 ��־λ
			break;
		
		  default:
				break;
		}


		
			
	 Step2_Start_Flag=0;		

	 Step3_Start_Flag=1;
			
	}

}
void Step3_Process(void)
{
	if(Step3_Start_Flag)
	{
		switch(Step3)
		{
			case 0:
				;                                  // Һ��1 ��־λ   �����䶯��û�н�                 
			break;
		  
			case 1:
					;                                // Һ��2 ��־λ   �����䶯��û�н�
				break;
			case 2:
				Pump_Jinyang_Flag_Nolink=1;        // Һ��3 ��־λ   �����䶯��  ֻ���� ��һ·
				break;
			case 3:
				;                                  // Һ��4 ��־λ    �����䶯��û�н�
			break;
			
			
			case 4:                              // ����1 ��־λ   ���ڲ������Ĭ������һ·
				Step_Run_Flag_Nolink=1;
			break;
			case 5:
				;                                  // ����2 ��־λ
			break;
			
			case 6:
				;                                  // ����3 ��־λ
			break; 
			case 7:
				;                                  // ����4 ��־λ
			break;
		
		  default:
				break;
		}	
			
	 Step3_Start_Flag=0;		
	}		
}

	



void Multi_Jinyang_Process(void)
{
//   u16 static Multi_Count;
//	if(Timer10ms_Flag)
//	{	
//		Multi_Count++;
//		switch(Multi_Count)
//		{
//			case 1:
//			
//			break;
//			
//			case 1400:                   // ��һ����Ҫ 14��
//				
//			break;
//		}	
//	
//	
//	}
	
	
	
 	Step1_Process();

	
	Step2_Process();

	
	Step3_Process();

	
	StepMotor1_Run_Nolink();                      // 
	Pump_Jinyang_Nolink();
	StepMotor_Reset_Nolink();
	
	
}	




void Open_Door_Relay_Nolink(void)      
{
		if(Open_Door_Relay_Flag_Nolink)    //   ����������Ĭ���ǹ��ŵģ�
		{
			  GPIO_SetBits(GPIOE, GPIO_Pin_5);   // ���÷��� ������� ����
			  Delay(10000000);
			
			  Open_Door_Relay_Flag_Nolink=0;

		}		
}	


void Open_Motor1_Run_Nolink(void)            //  ����  ͨ���趨PWM�� �� ���ŵ�� BLDC3650 �� �ٶ�
{
	u16 static Open_Run_Count;
	if(Motor_RunOpen_Flag_Nolink)
	{
		
		if(Timer10ms_Flag)
		{
			Open_Run_Count++;               // ÿ 10ms ����  �� 1
			switch(Open_Run_Count)
			{
				case 1:
					BLDC_Motor_Config(359,9);       //  72 000000/ 3600 =  20K  Ƶ�� һ���� 20K   ---- 50k
					break;
				
				case 2:
					GPIO_SetBits(GPIOF, GPIO_Pin_5);   // ���Ƶ�� �ϵ�
					break;
				
				case 1250:                     // 15000ms ֮�� 15 ��

					GPIO_ResetBits(GPIOF, GPIO_Pin_5);     // ���Ƶ�� �ϵ�
					Delay(10000000);
				  GPIO_ResetBits(GPIOE, GPIO_Pin_5);     // ���÷��� ��λ  �ص�Ĭ�Ϸ���״̬
					Delay(10000000);
					BLDC_Motor_Config(0,0);	               // �ر�����
				
					Open_Run_Count=0;
					Motor_RunOpen_Flag=0;
				  
				  Send_YuXiao_Start();                             //  ��������� ��Х  ��ʾ �� �Ѿ�  ���� 
					break;
			}
			if (EIE3810_Read_Key2() == KEYON) // TEST
			{
					GPIO_ResetBits(GPIOF, GPIO_Pin_5);     // ���Ƶ�� �ϵ�
					Delay(10000000);
				  GPIO_ResetBits(GPIOE, GPIO_Pin_5);     // ���÷��� ��λ  �ص�Ĭ�Ϸ���״̬
					Delay(10000000);
					BLDC_Motor_Config(0,0);	               // �ر�����
				
					Open_Run_Count=0;
					Motor_RunOpen_Flag_Nolink=0;
				   
//				  Send_YuXiao_Start();                             //  ��������� ��Х  ��ʾ �� �Ѿ�  ���� 
			}	
			
			Timer10ms_Flag=0;
		}	
	}	
}	




void Close_Door_Relay_Nolink(void)      
{
		if(Close_Door_Relay_Flag_Nolink)               //   ������� Ĭ��״̬�ǹ��ŵ�
		{
			  GPIO_ResetBits(GPIOE, GPIO_Pin_5);   //  ��λ����   ����ص�Ĭ�Ϸ���
			  Delay(10000);

			  Close_Door_Relay_Flag_Nolink=0;
//			  Motor_RunClose_Flag=1;            // because this is no-link mode in this .c file
		}		
}	



void Close_Motor1_Run_Nolink(void)                        //   ���� ���ܣ� ���Ƶ�� ����  ����
{
	u16 static Close_Run_Count;
	if(Motor_RunClose_Flag_Nolink)
	{
		if(Timer10ms_Flag)
		{
			Close_Run_Count++;
			switch(Close_Run_Count)
			{
				case 1:				
					BLDC_Motor_Config(359,9);
					break;
				
				case 2:
					GPIO_SetBits(GPIOF, GPIO_Pin_5);       // ���Ƶ�� �ϵ�
					break;
				
				case 670:
					GPIO_ResetBits(GPIOF, GPIO_Pin_5);     // ���Ƶ�� �ϵ�
				
					Delay(10000);
				  
					BLDC_Motor_Config(0,0);
				
					Close_Run_Count=0;
					Motor_RunClose_Flag_Nolink=0;
				
//				  Step_Run_Flag=1;                      // because this is no-link mode in this .c file
					break;
			
			}
			if (EIE3810_Read_Key0() == KEYON) // TEST
			{
					GPIO_ResetBits(GPIOF, GPIO_Pin_5);     // ���Ƶ�� �ϵ�
				
					Delay(10000);
				  
					BLDC_Motor_Config(0,0);
				
					Close_Run_Count=0;
					Motor_RunClose_Flag_Nolink=0;          
				
//				  Step_Run_Flag=1;                      // because this is no-link mode in this .c file
			}
			Timer10ms_Flag=0;
			
		}	
	}	
}	





void StepMotor1_Run_Nolink(void)                                 // ������� ���  ���Կ�ʼ ����
{
	u16 static Stepmotor1_Run_Count;
	if(Step_Run_Flag_Nolink)                                      // ������յ� st ���� ��־λ�ᱻ��Ϊ 1       
	{
		if(Timer10ms_Flag)
		{
			Stepmotor1_Run_Count++;
			switch(Stepmotor1_Run_Count)
			{
				case 1:

					SteppingMotor_Config_1(1439,99);               // 72 000 000 / 720 00 =1000 HZ  ����PWM 		
				
				break;
			  
				case 600:
					SteppingMotor_Config_1(0,0);                  // 2000ms�� ��������һ��������  8�� 
				break;
			
				case 800:                                    
					Stepmotor1_Run_Count=0;
				  Step_Run_Flag_Nolink=0;
				  
//				  Stepmotor1_Reset_Flag=1;                   // because this is the no-link mode
				  GPIOB->BSRR|=(1<<5);                            // ������� Direction �ø�, ����ת �� LED0 ����һ· IO
				break;
			}
			if (EIE3810_Read_PE0() == KEYON)				// TEST
			{
				SteppingMotor_Config_1(0,0);
				Delay(10000000);
				Stepmotor1_Run_Count=0;
				Step_Run_Flag_Nolink=0;
				  
				Stepmotor1_Reset_Flag_Nolink=1;
				GPIOB->BSRR|=(1<<5);                            // ������� Direction �ø�, ����ת �� LED0 ����һ· IO
			}	
				Timer10ms_Flag=0;
		}	
	}	
}	



void StepMotor_Reset_Nolink(void)                            //   ������� �������֮��  �˻�ԭ����λ��
{
	u16 static Stepmotor_Reset_Count;
	if(Stepmotor1_Reset_Flag_Nolink)
	{
		if(Timer10ms_Flag)
		{
			Stepmotor_Reset_Count++;
			switch(Stepmotor_Reset_Count)
			{
				case 1:
//					SteppingMotor_Config_1(719,99);
					SteppingMotor_Config_1(1439,99);               // 72 000 000 / 720 00 =1000 HZ  ����PWM 				
				break;
			
				case 600:
					SteppingMotor_Config_1(0,0);
				
				break;
			  case 800:
					Stepmotor1_Reset_Flag_Nolink=0;
				  Stepmotor_Reset_Count=0;
				  
//				  Pump_Jinyang_Flag=1;
				  GPIOB->BRR|=(1<<5);                           // ���н���  ��λ PB5
				break;
			
			}	
			if (EIE3810_Read_PE1() == KEYON)
			{
				SteppingMotor_Config_1(0,0);
				Delay(10000000);
				Stepmotor1_Reset_Flag_Nolink=0;
				Stepmotor_Reset_Count=0;				  
//				Pump_Jinyang_Flag=1;
				GPIOB->BRR|=(1<<5);                           // ���н���  ��λ PB5
			}
				
			Timer10ms_Flag=0;
		}	

	}	

}	



void Pump_Jinyang_Nolink(void)               //  ���� �����Ҫ��� 4 ͨ��
{
  u16 static Pump_Jinyang_Count;    
	if(Pump_Jinyang_Flag_Nolink)
	{
	                    
		if(Timer10ms_Flag)								//  ��ʼ�䶯��1 ͨ���Ľ���
		{
			Pump_Jinyang_Count++;
			switch(Pump_Jinyang_Count)
			{
				case 1:
          GPIO_SetBits(GPIOF, GPIO_Pin_3);				// 
					break;
				
				case 100:
					GPIO_ResetBits(GPIOF, GPIO_Pin_3); //  �պϼ̵���   �䶯�ÿ�ʼ ����  PF3 �� �ߵ�ƽ
					break;
				
				case 200:
					GPIO_SetBits(GPIOF, GPIO_Pin_3); //  �պϼ̵���   �䶯�ÿ�ʼ ����  PF3 �� �ߵ�ƽ
					break;
				
				case 1100:                    //   30�� = 30 000 = 3000 �� 10ms
          GPIO_ResetBits(GPIOF, GPIO_Pin_3);                             //   �Ͽ��̵���				PF5 ��  �͵�ƽ
					Pump_Jinyang_Count=0;
				  Pump_Jinyang_Flag_Nolink=0;

//				  Send_YunKai();             //    ��������  �� �ƿ�  ���Կ�ʼ   ���м��� ����
          System_Heat_Flag=1;			
				break;
			
			}	
			Timer10ms_Flag=0;
			
		}
	}	
}
