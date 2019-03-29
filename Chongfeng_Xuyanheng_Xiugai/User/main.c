#include <stm32f10x.h>
#include "EIE3810_Clock.h"
#include "EIE3810_USART.h"
#include "EIE3810_LED.h" 
#include "EIE3810_Buzzer.h"
#include "EIE3810_Key.h"
#include "EIE3810_Timer.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_Myown.h"
#include "EIE3810_Joypad.h"
#include "EIE3810_zhuxi.h"
#include "EIE3810_zhuxidebug.h"

// put your code here
// this is all the code designed for Professor Zhuxi and programming by Yanheng and Chongfeng based on STM32 Developing Board


/*****   ���Դ��ڴ򿪴���������  OK    *****/


/*****   2018-5-22   ��ʼ����ֱ����ˢ���  ������    ******/


/***** 2018-6-5 ����� ������  ���ӹ��ܳ���:  ������Ʒ ����

���� Һ�� ���Ϊ��

        Һ��1     Һ��2     Һ��3���䶯��3���ѽӣ�    Һ��4    ����1�����������   ����2   ����3    ����4
��ţ�    0         1         2                         3        4                   5       6        7 


����Э�飺 ���յ����ݣ� 23     03            02      05    05      04      0A      05      02    05    05   24
��Ӧ��                  ͷ   ������         ��Ʒ1  1����  1�ٶ�   ��Ʒ2  2����   2�ٶ�   ��Ʒ3 3����  3�ٶ�  β
����                      ����3���Լ�Ҫ���� Һ��3                 ����1                  Һ��3

��ô����˳��Ϊ��   �䶯��3  ����  ֮��  ����������� ����  ֮��   �䶯��3 ��  ����

Ŀǰ ��ʱû�ж��Լ� �Ľ�����  ��  �����ٶ�  ���� ����

********/

int main(void) 
{
		EIE3810_LED_Init();
	  GPIO_Config_Relay();
	  EIE3810_Buzzer_Init();
		EIE3810_KEY_Init();
    EIE3810_clock_tree_init();
	  EIE3810_NVIC_SetPriorityGroup(5);     // interrupt group     
	
	  EIE3810_USART1_ExtiInit();
	  EIE3810_USART2_ExtiInit();
	  
    EIE3810_USART1_init(72, 9600);
    EIE3810_USART2_init(36, 9600);
	
	  EIE3810_TIM4_Init(7199,99);           //  99 equals to 10ms  timer

    
		while(1)  
	  {
/*****   ������������  ��������   FE B2 03 E8 00 9D     ****/			
			#if 1			
				Receive_Process();
			#endif
			
			
			#if 1			                         //   ����ĳ� 0  �� ��ִ�� ����ĳ���
//			Open_Door_Relay();
//			Open_Motor1_Run();                 //   ����
//			Close_Door_Relay();
//			Close_Motor1_Run();
//			
//			StepMotor1_Run();
//			StepMotor_Reset();
			
			// Pump_Jinyang();
			Pump_1_pumping();
			Pump_2_pumping();
			Pump_4_pumping();
			Pump_3_pumping();
//			System_Heat_Run(); 
//			Over_Open_Door_Relay();
//			Over_Open_Motor1_Run();
//			Experiment_Over();
			#endif
			                                  // �¶� �ϴ�ָ��  
			#if 0
			
			 Multi_Jinyang_Process();
			
			#endif
      						
		}  	
}






