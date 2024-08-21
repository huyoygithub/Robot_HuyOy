
#include "stm32f4xx_it.h"
#include "stdlib.h"
#include "stdio.h"

void NMI_Handler(void)				{					}
void HardFault_Handler(void)	{	while (1){} } // Go to infinite loop when Hard Fault exception occurs 
void MemManage_Handler(void)	{	while (1){} } // Go to infinite loop when Memory Manage exception occurs 
void BusFault_Handler(void)		{	while (1){} } // Go to infinite loop when Bus Fault exception occurs 
void UsageFault_Handler(void)	{ while (1){} } // Go to infinite loop when Usage Fault exception occurs 
void SVC_Handler(void)				{					}
void DebugMon_Handler(void)		{					}
void PendSV_Handler(void)			{					}
//=========================================================================
extern vu8 en_gyro;
vu32 	_view_systick = 0;
u8 		_index_gyro   = 0;

void SysTick_Handler(void)   				// Ngat 1ms truyen du lieu Usart den cac slaver
{
	static u32 tam_gyro=0;
	//------------------------------
	if(++tam_gyro>10)
	{	tam_gyro=0;
		if(en_gyro==1)
		{ 	
			USART2->DR='z';
			_index_gyro=0;
		}
	}
}

//=========================================================================
void USART1_IRQHandler			(void){ }
void DMA2_Stream2_IRQHandler(void){ }
//=========================================================================
extern vs16 IMU;
extern vu8 	RxBuffer_compass[2],RX_USART2[15];

void USART2_IRQHandler(void)				// Xu ly Ngat Usart2 TX - Gui di ky tu 'z' or 'y' sau moi 20ms
{ 
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)
     {      
			 USART_ClearFlag(USART2, USART_FLAG_RXNE);
		  
			 RxBuffer_compass[_index_gyro++] = USART2->DR;
		  
			 if(_index_gyro > 1) { IMU =(RxBuffer_compass[0]<<8)|RxBuffer_compass[1]; }	 
			// _view_systick++;
	  }	
}
//=========================================================================
void DMA1_Stream5_IRQHandler(void)	// Ngat DMA uart2 RX // La Ban
{
	DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_TCIF5);
	IMU=-((RX_USART2[0]<<8)|RX_USART2[1]);

}

//=========================================================================
extern __IO int32_t num_over_t3 , num_over_t5;

void TIM3_IRQHandler(void){ }	
//=========================================================================
void TIM5_IRQHandler(void)					// encoder motor
{
  //TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	 TIM5->SR = (uint16_t)~TIM_IT_Update;
	
	 if(TIM5->CR1&TIM_CounterMode_Down)num_over_t5--;	
	 else num_over_t5++;	
}	


