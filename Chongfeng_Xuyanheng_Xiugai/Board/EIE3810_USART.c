//
// Copy from Professor Qian


#include "EIE3810_USART.h"
#include "EIE3810_Timer.h"
#include "EIE3810_Zhuxi.h"


u8 Usart1_Buffer[100];     //  �������ݻ�����
u8 USART1_Length;        //  ���ճ���
u8 USART1_RX_FinishFlag;    //  ������ɱ�־λ

u8 Usart2_Buffer[100];     //  �������ݻ�����
u8 USART2_Length;        //  ���ճ���
u8 USART2_RX_FinishFlag;    //  ������ɱ�־λ


void EIE3810_USART3_init(u32 pclk1, u32 baudrate) {
	  //USART3
    float temp;
    u16 mantissa;
    u16 fraction;
    temp = (float) (pclk1 * 1000000) / (baudrate * 16);
    mantissa = temp;
    fraction = (temp - mantissa) * 16;
    mantissa <<= 4;
    mantissa += fraction;
    RCC->APB2ENR |= 1 << 2; // enable APB2 Port A
    GPIOA->CRL &= 0xF00FFFFF; // reset CNF6 MODE6 CNF5 MODE5 for APB2_A
    GPIOA->CRL |= 0x08B00000; // set PA6 input pull-up/down; PA5 ouput@50MHz(max) alternative push pull
    RCC->APB1RSTR |= 1 << 17; // RESET USART3
    RCC->APB1RSTR &= ~(1 << 17); // SET USART3 (no effect)
    USART2->BRR = mantissa;// load baudrate
    USART2->CR1 = 0x200C; // USART enable; Transmitter EN
	
	  USART2->CR1 |=0X0020;    // enable receive interrupt
	
}




void EIE3810_USART2_init(u32 pclk1, u32 baudrate) {
    //USART2
    float temp;
    u16 mantissa;
    u16 fraction;
    temp = (float) (pclk1 * 1000000) / (baudrate * 16);
    mantissa = temp;
    fraction = (temp - mantissa) * 16;
    mantissa <<= 4;
    mantissa += fraction;
    RCC->APB2ENR |= 1 << 2; // enable APB2 Port A
    GPIOA->CRL &= 0xFFFF00FF; // reset CNF3 MODE3 CNF2 MODE2 for APB2_A
    GPIOA->CRL |= 0x00008B00; // set PA3 input pull-up/down; PA2 ouput@50MHz(max) alternative push pull
    RCC->APB1RSTR |= 1 << 18; // RESET USART2
    RCC->APB1RSTR &= ~(1 << 18); // SET USART2 (no effect)
    USART2->BRR = mantissa;// load baudrate
    USART2->CR1 = 0x200C; // USART enable; Transmitter EN
	
	  USART2->CR1 |=0X0020;    // enable receive interrupt
}


void EIE3810_USART1_init(u32 pclk1, u32 baudrate) {
    //USART1
    float temp;
    u16 mantissa;
    u16 fraction;
    temp = (float) (pclk1 * 1000000) / (baudrate * 16);
    mantissa = temp;
    fraction = (temp - mantissa) * 16;
    mantissa <<= 4;
    mantissa += fraction;
    RCC->APB2ENR |= 1 << 2; // enable APB2 Port A
    GPIOA->CRH &= 0xFFFFF00F; // reset CNF9 MODE9 CNF10 MODE10 for APB2_A
    GPIOA->CRH |= 0x000008B0; // set PA10 input pull-up/down; PA9 ouput@50MHz(max) alternative push pull
    RCC->APB2RSTR |= 1 << 14; // RESET USART1
    RCC->APB2RSTR &= ~(1 << 14); // SET USART1 (no effect)
    USART1->BRR = mantissa;// load baudrate
    USART1->CR1 = 0x200C; // USART enable; Transmitter EN

		//
	
	  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	
	  //
		
	  USART1->CR1 |=0X0020;    // enable receive interrupt  
	
	  USART1_RX_FinishFlag=0;
}




void EIE3810_NVIC_SetPriorityGroup(u8 prigroup)
{
	u32 temp,temp1;
	temp1 = prigroup & 0x00000007;
	temp1 <<=8;
	temp = SCB-> AIRCR;
	temp &= 0x0000F8FF;
	temp |= 0x05FA0000;     //  д���ʱ��   [31:16] ��ֵ  0x05FA  ���Ի���
	temp |=temp1;
	SCB->AIRCR=temp;

}	




void EIE3810_USART1_ExtiInit(void)
{
	NVIC->ISER[1] |=1<<5;     //  ���   �ж��ܿ��� 37�� �ж�
	NVIC->IP[37]  =0X30;      
}	




/* void USART1_IRQHandler(void)
{
   char temp;
	 if(USART1->SR &(1<<5))
	 {
		 temp =  USART1->DR;    // ��ʱ���� temp ���յ����ֽ�
//		 {
//			 USART1->DR = temp;
//			 while (!((USART1->SR >> 7) & 0x1));
//		 }
		 if(USART1_Length < 100)      // �������û�дﵽ����ȥ���ֵ�����Լ�������
		 {
				Usart1_CountDown=1;
  			Usart1_Buffer[USART1_Length++]=temp;
		 }
		 else
		 {
				USART1_RX_FinishFlag=1;   //  ǿ�ƽ������
		 } 
	 }	
	  
}	*/

void USART1_IRQHandler(void)
{
	uint8_t Clear=Clear; // avoid warning never used
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // get 1 byte
	{
		Usart1_Buffer[USART1_Length++] = USART1->DR;
	} else if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET) // get a package
	{
		Clear=USART1->SR; // read SR
		Clear=USART1->DR; // read DR, this could clear IDLE interrupt
		USART1_RX_FinishFlag = 1; // get 1 package
	}
	
}




void EIE3810_USART2_ExtiInit(void)
{
	NVIC->ISER[1] |=1<<6;
	NVIC->IP[38]  =0X30;      // ���  �ж��ܿ���   ����2  38��
}	

	


void USART2_IRQHandler(void)
{
	 char temp;
	 if(USART2->SR &(1<<5))
	 {
		 temp =  USART2->DR;    // ��ʱ���� temp ���յ����ֽ�

		 if(USART2_Length < 100)      // �������û�дﵽ����ȥ���ֵ�����Լ�������
		 {
				Usart2_CountDown=1;
				Usart2_Buffer[USART2_Length++]=temp;
		 }
		 else
		 {
				USART2_RX_FinishFlag=1;   //  ǿ�ƽ������
		 } 
	 }
}	






void USART_print(u8 USARTPort, char *st) {
    u8 i=0;
    while (st[i]!= 0x00) {
        if (USARTPort == 1) {
            USART1->DR = st[i];
            while (!((USART1->SR >> 7) & 0x1));
        }
				
        if (USARTPort == 2) {
            USART2->DR = st[i];
            while (!((USART2->SR >> 7) & 0x1));
        }
        if (i==255) break;
        i++;
    }
}






