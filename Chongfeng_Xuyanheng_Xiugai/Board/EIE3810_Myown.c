#include "stm32f10x.h"
#include "EIE3810_Myown.h"
#include "EIE3810_USART.h"
#include "EIE3810_LED.h"
#include "EIE3810_Timer.h"
#include "EIE3810_TFTLCD.h"
#include "EIE3810_KEY.h"
#include "EIE3810_Joypad.h"
//#include "EIE3810_ADC.h"
#include "EIE3810_Buzzer.h"

// put your procedure and code here

u8 Timer1s_Flag;
u8 Game_InitFlag;

u8 Role_Server;
u8 Role_Catcher;

u8 Easy_Level;
u8 Hard_Level;

u8 First_StepFlag;
u8 Second_StepFlag;
u8 Get_RandomFlag;
u8 Game_RunFlag;
u8 Game_OverFlag;

u8 Game_RunningFlag;
u8 Game_StoppingFlag;

u8 Game_Stop_Restart;
u8 Game_Stop_Exit;

u8 Need_Random_Flag;
u8 Show_Random_Flag;

u8 Launch_Ball_Flag=0;


/***   ������Ч ****/
u8 Game_TitleFlag_Beep;
u8 Game_InitFlag_Beep_Step2;

u16 Temp,Cursor_x=210,Cursor_y=780;      // �µ���� ����
u8 Key_value;                          

u16 Cursor_x2=210,Cursor_y2=8;           // �ϵ���� ���� (�����γ�����β�)
u8 Joypad_value;

u16 x_speed=2,y_speed=3;                 //   ������ٶ� �ᱻ���µ�    �ᱻ y_speed ���Ѷ�ѡ�� ����

u16 x_move=240,y_move=780;               //   �������ʼλ��
//u8 x_speed,y_speed;                  
u16 speed[8][2]={{-4, 3}, {-3, 4},{-2, 4},{-1, 5}, {1, 5}, {2, 4},{3, 4},{4, 3}};
//u16 speed[8][2]={{-1, 1}, {-1, 1},{-1, 1},{-1, 1}, {1, 1}, {1, 1},{1, 1},{1, 1}};
/***  ��Ϸ��¼   ****/

u8 Game_Round_Record;
u32 Game_Time_Using;         //  10ms  unit
u8 Game_Time_1s;
u8 Game_Time_1mins;
u8 Game_Count_Down_Flag;
u8 multiplier;

extern u8 Delay(u32 count);


void EIE3810_Peripheral_Check(void)
{
/***    check usart 1 and DS0 ***/		
		if(USART1_RX_FinishFlag)	
		{
			int j;
			for(j=0;j<USART1_Length;j++)
			{
				USART1->DR = Usart1_Buffer[j];
				while (!((USART1->SR >> 7) & 0x1));
				
				EIE3810_LED0_On();   
			}
			
			USART1_RX_FinishFlag=0;
			USART1_Length=0;
			
			EIE3810_LED0_Off();		
		}
			
/***    check usart 2 and DS1 ***/					
		if(USART2_RX_FinishFlag)	
		{
			int j;
			for(j=0;j<USART2_Length;j++)
			{
				USART2->DR = Usart2_Buffer[j];
				while (!((USART2->SR >> 7) & 0x1));
				
				EIE3810_LED1_On();		  
				
			}
			
			USART2_RX_FinishFlag=0;
			USART2_Length=0;
			
			EIE3810_LED1_Off();		
		}	
			
/***    check Timer 10ms    ***/		
    if(Timer10ms_Flag)
		{
		   int static j;
			 j++;
			 if(j<=5)
		   {
			   EIE3810_LED0_On();
			 }
			 if(j>5)
			 {
				 EIE3810_LED0_Off();	
			 }
       if(j>9)
			 {
				 j=0;
			 }	 
		   Timer10ms_Flag=0;
			 #if 0
			 USART1->DR = j+ '0';
       while (!((USART1->SR >> 7) & 0x1));
			 #endif
		}
/***    check Timer 10ms  and LCD     ***/	
    if(Timer100ms_Flag)
		{
			int static k;
			k++;
			if(k>10)     //  k=10 times equals to 1 second
		  {
				Timer1s_Flag=1;
				k=0;
			}
			if(Timer1s_Flag)
			{
			  int static m;
				EIE3810_TFTLCD_SevenSegment(200, 400, 8, WHITE);
				m++;
				
				EIE3810_TFTLCD_SevenSegment(200, 400, m, BLUE);

				if(m>=9)
				{
				  m=0;
				}	
				Timer1s_Flag=0;
				
			}	
			Timer100ms_Flag=0;
		}	
		
/***    check KEY1     ***/	
    
    if(EIE3810_Read_Key1())
    {
			EIE3810_LED1_On();
		}
		else
		{
			EIE3810_LED1_Off();
		}
		
}
		

/********    ��Ϸ��ʼ��    *********/

/***** Delay ���� ��� ��ʱ�� ��ȫ����   ����������Ϸ��ʼ����  ������Ҫ����Ӱ��Ч�� ****/
void EIE3810_Game_Init(void)
{
	if(Game_InitFlag)
  {
		Delay(10000000);
		EIE3810_TFTLCD_ShowString2412(80,80,(u8 *)"Welcome to mini Project!", WHITE, BLUE);  // 2412 �������
		
		Delay(10000000);
		EIE3810_TFTLCD_ShowString(100,140,(u8 *)"This is the Final Lab.", WHITE, RED);
		Delay(10000000);
		EIE3810_TFTLCD_ShowString(100,180,(u8 *)"Are you ready?", WHITE, RED);
		Delay(10000000);
		EIE3810_TFTLCD_ShowString(100,220,(u8 *)"OK! Let's start.", WHITE, RED);
	
		Delay(10000000);
//			EIE3810_TFTLCD_Clear(WHITE);                          // use this subroutine, it is not ok,  I do not know why
      EIE3810_TFTLCD_FillRectangle(80, 380, 80, 380, WHITE);	

	/********  4-23 ȡ����ɫ ѡ��   �ĳ�  �Ѷ�ѡ��    ********/	
		
    	//Delay(10000000);
		EIE3810_TFTLCD_ShowString(100,260,(u8 *)"Please select the difficulty level:", WHITE, RED);
			//Delay(10000000);
			EIE3810_TFTLCD_ShowString(100,300,(u8 *)"Easy", WHITE, BLUE);
			
			EIE3810_TFTLCD_ShowString(100,340,(u8 *)"Hard", BLUE, WHITE);
	 
		  //Delay(10000000);
			EIE3810_TFTLCD_ShowString(100,380,(u8 *)"Pres KEY0 to enter.", WHITE, RED);

			Game_InitFlag=0;                                        // first 
		  First_StepFlag=1;
			Easy_Level =1;
			Hard_Level =0;
		  
			multiplier=1;                      //  ������ʼֵ ������ ��һ�ν����Ѷ�ѡ��  ֱ�� KEY0  ���� multiplier ��û�и�ֵ
			
	}
	
}	


void First_Step(void)
{		
   if(First_StepFlag)
	 {
	  
		 if(EIE3810_Read_KeyUp() || (EIE3810_JOYPAD_Read()==16))                   //  choose easy
		 {
				EIE3810_TFTLCD_ShowString(100,300,(u8 *)"Easy", WHITE, BLUE);
				EIE3810_TFTLCD_ShowString(100,340,(u8 *)"Hard", BLUE, WHITE);	 
			 Easy_Level =1;
			 Hard_Level =0;
			 multiplier=1;
			 
			 
		 } 
		 if(EIE3810_Read_Key1()  || (EIE3810_JOYPAD_Read()==32))                    //  choose hard
		 {
				EIE3810_TFTLCD_ShowString(100,300,(u8 *)"Easy", BLUE, WHITE);
				EIE3810_TFTLCD_ShowString(100,340,(u8 *)"Hard", WHITE, BLUE);
			 

			 Easy_Level =0;
			 Hard_Level =1;
			 multiplier=2;			 
			
		 } 
		 
		 if( ( EIE3810_Read_Key0() || (EIE3810_JOYPAD_Read()==4 ) )  && (Hard_Level==1) )     //   ���� KEY0  ���� SELECT
		 {
				EIE3810_TFTLCD_ShowString(100,380,(u8 *)"You choose Easy.   ", WHITE, WHITE); // clear
				EIE3810_TFTLCD_ShowString(100,380,(u8 *)"You choose Hard.   ", WHITE, RED);
				
			 First_StepFlag=0;                   //  �����־λ   ������һ������
			 Need_Random_Flag=1;
		 }
		 if( ( EIE3810_Read_Key0() || (EIE3810_JOYPAD_Read()==4 ) ) && (Easy_Level==1) )    //      ����KEY0  ���� SELECT
		 {
			 EIE3810_TFTLCD_ShowString(100,380,(u8 *)"You choose Hard.   ", WHITE, WHITE); // clear   ���� �����ʾ��
			 EIE3810_TFTLCD_ShowString(100,380,(u8 *)"You choose Easy.   ", WHITE, RED); 

			 First_StepFlag=0;                  //   �����־λ  ������һ������
			 Need_Random_Flag=1;
		 }
	 //Delay(6000000);
	}	
	 
}	



void Need_Random_Value(void)
{
    u8 static Need_Count;
	  if(Need_Random_Flag)
		{
				//Delay(10000000);		
		    EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);
      	//Delay(10000000);			
			  EIE3810_TFTLCD_ShowString2412(40,260,(u8 *)"Use USART for a random direction.", WHITE, RED);
			  //Delay(5000000);	
			  Need_Random_Flag=0;
			  Second_StepFlag=1;
		}	
}	


void Second_Step(void)                 
{

	if(Second_StepFlag)
	{
//			EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);	
//			EIE3810_TFTLCD_ShowString(100,260,(u8 *)"Please use USART to RECEIVE random", WHITE, RED);
      if(USART1_RX_FinishFlag)	
			{
				 #if 0
				 int j;
						for(j=0;j<USART1_Length;j++)              // printf out all the datas in received buffer, for debug  use
						{
							USART1->DR = Usart1_Buffer[j];
							while (!((USART1->SR >> 7) & 0x1));
						}
         #endif		
//						switch (Usart1_Buffer[0]){
//							case 0:
//								EIE3810_TFTLCD_ShowString(100,300, (u8 *)"A: 0", WHITE, RED); 
//								break;
//							case 1:
//								EIE3810_TFTLCD_ShowString(100,300, (u8 *)"B: 1", WHITE, RED); 
//								break;
//							case 2:
//								EIE3810_TFTLCD_ShowString(100,300, (u8 *)"C: 2", WHITE, RED); 
//								break;
//							case 3:
//								EIE3810_TFTLCD_ShowString(100,300, (u8 *)"D: 3", WHITE, RED); 
//								break;
//							case 4:
//								EIE3810_TFTLCD_ShowString(100,300, (u8 *)"E: 4", WHITE, RED); 
//								break;
//							case 5:
//								EIE3810_TFTLCD_ShowString(100,300, (u8 *)"F: 5", WHITE, RED); 
//								break;
//							case 6:
//								EIE3810_TFTLCD_ShowString(100,300, (u8 *)"G: 6", WHITE, RED); 
//								break;
//							case 7:
//								EIE3810_TFTLCD_ShowString(100,300, (u8 *)"H: 7", WHITE, RED); 
//								break;
//						}
						EIE3810_TFTLCD_ShowString(100,300, (u8 *)"The random number received is: ", WHITE, RED);
						EIE3810_TFTLCD_ShowNum(100+8*31,300, Usart1_Buffer[0], WHITE, RED);
						Delay(30000000);
					x_speed=speed[Usart1_Buffer[0]][0]*multiplier;
					y_speed=speed[Usart1_Buffer[0]][1]*multiplier;
					Second_StepFlag=0;
					Show_Random_Flag=1;
					USART1_RX_FinishFlag=0;
					USART1_Length=0;	
					Launch_Ball_Flag=1;
					
//						
//					if(Usart1_Buffer[0]==0x52)	//	R�� 0x52   �ϲ㷢�͵���  "Randomxy"    �ַ� x ����� ȡ��1�� ������߷���   ȡ ��2�� Ϊ���ұ߷���
//					{
//              x_speed=(Usart1_Buffer[7]-0x30);              // y ��  1-9 ֮��   
//						  EIE3810_TFTLCD_ShowString(100,300,(u8 *)"Random is Received!", WHITE, RED);    //  ����Show_Random_Seed ��ִ��
//              if(Usart1_Buffer[6]==0x31)              // 0x31 equals to '1'
//							{
//								x_speed=-x_speed;
//								EIE3810_TFTLCD_ShowString(100,300,(u8 *)"The random seed is: -", WHITE, RED);
//								EIE3810_TFTLCD_ShowNum(270,300,(u8 )-x_speed, WHITE, RED);
//							}
//							else 
//							{
//								EIE3810_TFTLCD_ShowString(100,300,(u8 *)"The random seed is: +", WHITE, RED);
//								EIE3810_TFTLCD_ShowNum(270,300,x_speed, WHITE, RED);
//							}	
//							
//						  Second_StepFlag=0;
//						  Show_Random_Flag=1;
//					}
						
			}
			
	 }	 

}


void Show_Random_Seed(void)
{

	if(Show_Random_Flag)
	{
			//Delay(15000000);                       // ����� ��ʱ ��  ���� ��һ�� ������ӵ� ��ʾ
	    Show_Random_Flag=0;		 
		  Game_Count_Down_Flag=1;
	}	

	
	
	

}	



void Game_Countdown(void)
{
	
		int static m=3;
		if(Game_Count_Down_Flag)
		{
				if(Timer100ms_Flag)
				{
						EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);	   // clear LCD
						EIE3810_TFTLCD_SevenSegment(200, 400, m, BLUE);
						m--;

					Timer100ms_Flag=0;
						
				}	
				if(m<0)
				{
					Game_Count_Down_Flag=0;
					EIE3810_TFTLCD_FillRectangle(0, 480, 0, 800, WHITE);	
					m=3;
					Game_RunFlag=1;
								
				}	
		}	
}	




/*****  4.23  ȡ��   ADC ������  �����  �˺�������   ******/

void EIE3810_Get_Random(void)
{
  u8 scrand_value;
	
	if(Get_RandomFlag)
	{	
		//scrand_value= 0x07 & EIE3810_Get_ADC(1);                     //PA1
		//scrand_value= 0x07 & (scrand_value + EIE3810_Get_ADC(1) );
		
		x_speed=(1+scrand_value)-4;                                  // ( 1---8 ) - 4 =  (-3  ----4) x����������֮��
		
//		scrand_value= 0x03 & EIE3810_Get_ADC(1);                     //PA1
//		scrand_value= 0x03 & (scrand_value + EIE3810_Get_ADC(1) );		

		
		//		y_speed=(1+scrand_value);
		
//	USART1->DR = x_speed+ '0';                                  //  ��Ҫ��ʱ��  ͨ�����ڷ��ͳ��� ���м��
//	 while (!((USART1->SR >> 7) & 0x1));
//		 USART1->DR = y_speed+ '0';
//	 while (!((USART1->SR >> 7) & 0x1));
		
		Game_RunFlag=1;
	  Get_RandomFlag=0;
		
	}
}	









void EIE3810_Game_Run(void)
{	
	u32 static Key_Count;
//	u8 static Key_Count_Flag;
	u8 static Key_Check;
	if(Timer20ms_Flag)                         //  ����ͨ���������ı��ٶ�
// if(Timer10ms_Flag)		
	{
/******    ���� ���� ��Ϸ��Ĺ�����  ͨ��������������Ϸ��ֹͣ �� ���¿�ʼ   ******/
/*****    �����Ǵ������ĳ���  �ö�ʱ������  ��ɿ�     *****/		

		if(EIE3810_Read_Key1()|| (EIE3810_JOYPAD_Read()==8))  //  �����������֮�����ö�ʱ������
    {   
			  Key_Count++;
			  if(Key_Count > 2  && !Key_Check)                  //  10ms ֮��  ȷ��ȷʵ������    
		    {
//					Key_Count_Flag++;     
					Game_RunningFlag=!Game_RunningFlag;            //   ״̬ȡ��
					Game_StoppingFlag=!Game_RunningFlag;
					Key_Check =1;					
				}
		}
    else                 //  ʵ�ʰ����������£��϶���ֹ10ms������һֱ��⵽���£�״̬Ƶ��ȡ��  
		{
				Key_Check=0;
				Key_Count=0;
		}
		
		if (Launch_Ball_Flag==1)
		{
			Game_RunningFlag=1;         
			Game_StoppingFlag=0;
			Launch_Ball_Flag=0;
			//Key_Check =1;			
		}

/******  ��������Ϊ ��ȷ��⵽����  ����  �л�״̬   ֹͣ���� ����  ******/
		/*****   �������ʼ  ִ������    ********/
		
		if(Game_RunningFlag)
		{
				EIE3810_TFTLCD_ShowString(200,300,(u8 *)"Start", WHITE, WHITE);     //  ��� ֹͣ��Ϸʱ ѡ�� ����  ����  ֱ���˳��� ��ʾ
				EIE3810_TFTLCD_ShowString(200,340,(u8 *)"Exit!", WHITE, WHITE);				
			  
			  EIE3810_Buzzer_Off();              //  ÿ��10ms����   Ĭ�Ϸ������ǹرյ�  ��������ײ���� �� 10ms
			
/*****  �ð���������  �µ��� �� �ƶ�           *******/
  			Key_value=EIE3810_KEY_Scan();
				switch(Key_value)
				{
						case 1:         				
							EIE3810_TFTLCD_FillRectangle(Cursor_x,60, Cursor_y,12,WHITE);	 //  �����һ�εĺۼ�			
							if (Cursor_x<418) 
							{
								Cursor_x+=2;
							}
							break;
						case 3:
							EIE3810_TFTLCD_FillRectangle(Cursor_x,60, Cursor_y,12,WHITE);  //  �����һ�κۼ�
							if (Cursor_x>=2)
							{
								Cursor_x-=2;	
							}
						break;
						default:
							break;
				}	        

				EIE3810_TFTLCD_DrawCircle(x_move,y_move-15,15,1,WHITE); //   ������κۼ�
				
/*****  ���� �� ��ײ ����ж�    *****/		
				EIE3810_Reflection_Judge();	

/*****  ����λ��       ******/				
				x_move+=x_speed;                //  ǰ�������  �Ѿ� ���з����� ��������ֻ��Ҫ����д
				y_move-=y_speed;

				EIE3810_TFTLCD_FillRectangle(Cursor_x , 60, Cursor_y, 12, BLACK);
				EIE3810_TFTLCD_DrawCircle(x_move,y_move-15,15,1,RED);	
				
/*****   �����Ǵ�����Ϸ�ֱ�����Ϸ     ******/
				Joypad_value=EIE3810_JOYPAD_Read();    //   copy from professor
				switch(Joypad_value)
				{
						case 128:         				
							EIE3810_TFTLCD_FillRectangle(Cursor_x2,60, Cursor_y2,12,WHITE);	 //  �����һ�εĺۼ�	
							if (Cursor_x2<=418)
							{
								Cursor_x2+=2;
							}
						break;
						case 64:
							EIE3810_TFTLCD_FillRectangle(Cursor_x2,60, Cursor_y2,12,WHITE);  //  �����һ�κۼ�
							if (Cursor_x2>=2)
							{
								Cursor_x2-=2;	
							}
						break;
						default:
							break;
				}	     
				
				EIE3810_TFTLCD_FillRectangle(Cursor_x2 , 60, Cursor_y2, 12, BLACK);
/******     ��������ʾ ��Ϸ ʱ��   ��  �غ� ��    *******/
				Game_Time_Using++;
				if(Game_Time_Using > 100)                                           //  1��ʱ�䵽�� 1000 = 10ms * 100
				{
						Game_Time_1s++;
					  Game_Time_Using=0;
				}
				if(Game_Time_1s>60)         
				{
						Game_Time_1mins++;
					  Game_Time_1s=0;
				}	
				
				EIE3810_TFTLCD_ShowString(10,390,(u8*)"Round:",BLUE,WHITE);         //  �غ���   һ�� һ �� Ϊһ���غ�
				EIE3810_TFTLCD_ShowNum(60,390,(Game_Round_Record)/10,BLUE,WHITE);
				EIE3810_TFTLCD_ShowNum(68,390,(Game_Round_Record)%10,BLUE,WHITE);				
				
				EIE3810_TFTLCD_ShowString(10,410,(u8*)" Time:",BLUE,WHITE);
				EIE3810_TFTLCD_ShowNum(60,410,Game_Time_1mins,BLUE,WHITE);
				EIE3810_TFTLCD_ShowString(70,410,(u8*)"m",BLUE,WHITE);              //  mins  ��
				EIE3810_TFTLCD_ShowNum(78,410,Game_Time_1s/10,BLUE,WHITE);          //  ��   ʮ λ
				EIE3810_TFTLCD_ShowNum(86,410,Game_Time_1s%10,BLUE,WHITE);          //  ��   �� λ
				EIE3810_TFTLCD_ShowString(94,410,(u8*)"s",BLUE,WHITE); 
		}
		
/********   �����Ϸ������  ����  ֹͣ״̬  ��    *******/
		if(Game_StoppingFlag)
		{
			 if(EIE3810_Read_Key2() || (EIE3810_JOYPAD_Read()==16) )
			 {
					EIE3810_TFTLCD_ShowString(200,300,(u8 *)"Start", WHITE, BLUE);
					EIE3810_TFTLCD_ShowString(200,340,(u8 *)"Exit!", BLUE, WHITE);
				  
				  Game_Stop_Restart=1;
				  Game_Stop_Exit=0;
			 } 
			 if(EIE3810_Read_Key0()  || (EIE3810_JOYPAD_Read()==32))
			 {
					EIE3810_TFTLCD_ShowString(200,300,(u8 *)"Start", BLUE, WHITE);
					EIE3810_TFTLCD_ShowString(200,340,(u8 *)"Exit!", WHITE, BLUE);
	        
				  Game_Stop_Restart=0;
				  Game_Stop_Exit=1;
			 } 
			 
			 if( (EIE3810_Read_KeyUp() || (EIE3810_JOYPAD_Read()==4 )) && (Game_Stop_Restart==1) )
			 {
			     //  ������Ϸ
			    Game_RunningFlag=1;
				  Game_StoppingFlag=0;  
				  //   �����Ļ
				  EIE3810_TFTLCD_ShowString(200,300,(u8 *)"Start", WHITE, WHITE);
					EIE3810_TFTLCD_ShowString(200,340,(u8 *)"Exit!", WHITE, WHITE);
			 }
		   else if((EIE3810_Read_KeyUp() || (EIE3810_JOYPAD_Read()==4 )) && (Game_Stop_Exit==1))
		   {
					//  ֱ���˳�
			    Game_OverFlag=1;
				  Game_StoppingFlag=0; 
				  
				  Game_RunningFlag=0;
			    Game_StoppingFlag=0;  
				  
				  Game_RunFlag=0;                      //  professor ���������ӵ�
				 
			 }
		}	
		
		//  10ms  ��ʱ�� ����  ÿһ��  ����  ����Ҫ 
//		Timer10ms_Flag=0;
		Timer20ms_Flag=0;
	}	
}	

void EIE3810_GameAgain(void)
{
	u8 static Count;
	if(Timer100ms_Flag)
	{
		Count++;
		switch(Count)
		{
			case 2:
				;
			break;
			case 3:          //  case 1 �����ȴ�
				EIE3810_TFTLCD_FillRectangle(0,480, 0,800,WHITE);
			  EIE3810_TFTLCD_ShowString2412(180,380,(u8 *)"Game over!",RED,BLUE);
			break;
			case 4:
				EIE3810_TFTLCD_FillRectangle(0,480, 0,800,WHITE);
			break;
			
			case 5:                        //  reset  all  the related parameters ang Flag
			  Game_OverFlag=0;
			
				Count=0;
			
			  Cursor_x=210;
			  Cursor_y=780;
			
			  Cursor_x2=210;
			  Cursor_y2=8; 
			  
			  x_move=240;
			  y_move=780;
			
				Game_Round_Record=0;         // ��ʱ����
			  Game_Time_1s=0;
				Game_Time_1mins=0; 
			 
			  Game_Stop_Restart=0;
				Game_Stop_Exit=0;
			
//			  Game_TitleFlag_Beep=1;       //   restart the game with beep
        Game_InitFlag =1;              //   restart the game without beep  
				
				Game_RunningFlag=0;
				
				Game_RunFlag=0;  
			break;
			
			default:
				break;
			
		}	
		Timer100ms_Flag=0;
	}	
}	


//  ********    ���µĺ�����ͨ����Ч�ķ�ʽ��ʵ��    ��Ϸ��ʼ����   **********//
u8 string[]="Welcome to mini Project";

void EIE3810_TFTLCD_ShowString_Beep(u16 x,u16 y,u8 *p, u16 color, u16 bgcolor)
{
	u8 static String_BeepFlag;
	u16 static x_keep;
	
	u8 static  Show_Count;
//	u8 static *p_keep;
//	x_keep=x;
//	p_keep=p;
	if(Timer10ms_Flag)
	{
	  String_BeepFlag++;
		switch(String_BeepFlag)
		{
			case 1:
				EIE3810_TFTLCD_ShowChar2412(x_keep+x,y,(string[Show_Count-1]),color,bgcolor);    // ��ӡ�ַ���ÿ��10ms ������ӡһ���ַ�
				x_keep+=24/2;
//				p++;			
        EIE3810_Buzzer_On();		                                                     //  ����Ч  ������	
			break;
			
			case 2:
				EIE3810_Buzzer_Off();
			break;
		
		  case 20:
					String_BeepFlag=0;
					Show_Count++;
				break;
		}	
	
		if(Show_Count>23)      // String  �ܹ���23���ַ� ��Ҫ��ʾ
		{
		   Game_TitleFlag_Beep=0;	
			 Show_Count=0;
			 x_keep=0;
			 Game_InitFlag=1;     // ����  ��ʼ�� 
			 
		}	
		
		Timer10ms_Flag=0;	
		
	}	
}	


//   ****   ���� ��  �ж� ����   *****//
void EIE3810_Reflection_Judge(void)
{
	if((x_move>450) ||( x_move<30) )                                //  ���ҷ���
	{
		x_speed=-x_speed;
		EIE3810_Buzzer_On();                    //  ÿһ����ײ�ᷢ����������
	}	
		
	if(((y_move-15)>765) || ( y_move <35))
	{
		EIE3810_Buzzer_On();                   //   ÿһ����ײ���� �ᷢ����������
		y_speed=-y_speed;
		
		Game_Round_Record++;                   //   ����غ��� 
		
		if(y_move>=760)
		{
			 if((x_move<Cursor_x) || ( x_move> Cursor_x+60))
			 {

					Game_RunFlag=0;
				  Game_RunningFlag=0;
					Game_OverFlag=1;
				  EIE3810_Buzzer_Off();
			 }   
		}
		if(y_move<35)
		{
			if((x_move<Cursor_x2) || ( x_move> Cursor_x2+60))
			{

				Game_RunFlag=0;
				Game_RunningFlag=0;
				Game_OverFlag=1;
				EIE3810_Buzzer_Off();
			}   
		}
     
	}	

}	

/*****   ͨ������2   �� �ƿ�����    *****/
/*****   �������� ��   Э��    0xFE B2  ��λ   ��λ    NULL  CheckSum    ****/

void Data_Process_From_Yunkai(void)
{

	if(USART2_RX_FinishFlag)	                       //   �������2  ���� �� ���� 
		{
				int j;
				for(j=0;j<USART2_Length;j++)
				{
					USART2->DR = Usart2_Buffer[j];
					while (!((USART2->SR >> 7) & 0x1));
					
					EIE3810_LED1_On();		  
				
			}
			
			USART2_RX_FinishFlag=0;
			USART2_Length=0;
			
			EIE3810_LED1_Off();		
		}	



}	







