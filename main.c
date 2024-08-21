
#include "stm32f4xx.h"

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <math.h>

#include <main.h>

#include "Huyoy_Init_F4.c"
#include "Controler4Wheel.h"





//======================================================
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	
//======================================================
s16 data_gyro,temp, pwm;
u8	_lcd;
	
//u8 data_tx_gyro;
 
unsigned char arr[16], giu = 35;
float angle;
char dataUsart3;
char rxUsart3;
	
char autoRun, tang_toc = 0;
char buffer[10];

//char arr[7];
vs16 IMU;

unsigned int k;
unsigned char mode=0;

float he_so_xoay;

unsigned int thoi_gian, giay, time;

extern vu32 game 	;

GPIO_InitTypeDef  GPIO_InitStructure;

//======================== DELAY MICROSECOND ========================
void delay_ms(__IO uint32_t num)
{
	__IO uint32_t index = 0;
	for(index = (16800 * num); index != 0; index--){}
}
//===================================================================
void delay_us(__IO uint32_t num)
{
	__IO uint32_t index = 0;
	for(index = (17 * num); index != 0; index--){}
}
//===================================================================
void TIM7_IRQHandler	(void)
{
	TIM7->SR = (uint16_t)~TIM_IT_Update; // xoa co ngat timer 7
	robotAnalytics();	
	
	putchar4(16);
	
}
//===================================================================
void UART4_IRQHandler	(void)// ngat  uart1 RX
{ 
	static unsigned char ktra, cnt = 0, getchar_uart = 0;
	if(USART_GetITStatus(UART4,USART_IT_RXNE)!= RESET)
    {      
			 USART_ClearFlag(UART4, USART_FLAG_RXNE);
			
			 getchar_uart=UART4->DR;

				if(ktra == 4)
				{
					dataBuff[cnt]=getchar_uart;
					cnt++;
					if(cnt>5)
					{
						ktra = 0;
						cnt = 0;

					}
				}
				if(getchar_uart == 0xAA) ktra++;
		}	
}

//-------------------------------------------------------------------
void gp_update_btn 		(void)
{
	putchar4(16);
}

//===================================================================
////--------------------------------------------------------------------------------------

int lowSpeed=28;
int mediumSpeed=29;
int highSpeed=30;

int moveSpeed;

int tempSpeed=0;
//---------------  varialbe state--------------------------------


bool tkLeft_13=false;
bool tkLeft_2= false;

bool tkRight_13=false;
bool tkRight_2=false;

bool xlRaiseGun=false;
bool xlLiftBody=false;
bool xlDropArm=false;
bool xlShoot=false;

bool spinHand= false;
bool suckDick=false;
bool isFuck=false;
//---------------------------------------------------------------



//===============================================================

//===============================================================
void manual_RED(void)
{
	
	if(tempSpeed==1){moveSpeed=mediumSpeed;}else 
	if(tempSpeed==2){moveSpeed=highSpeed;  }else
	if(tempSpeed==0){moveSpeed=lowSpeed;   }else{moveSpeed=mediumSpeed;}
	
	
	if(SELECT)	{	putchar2('a'); }	else 
	if(START)	{
		
			
	tempSpeed++;	if(tempSpeed>=2){tempSpeed=0;}

		

	}	else 

	// control with joystick
//-----right---------------------------------------
	if(RAU)	{	robotRun (  0	 		,moveSpeed);}	else 
	if(RAD)	{	robotRun (  1800 	,moveSpeed);} else
	if(RAR)	{	robotRun ( 900 	,moveSpeed);}	else 
	if(RAL)	{	robotRun ( -900  	,moveSpeed);}	else 
	
//-----left----------------------------------------
	
	if(LAL)	{	robotRotate(-900,1.2,0);  }	else
	if(LAR)	{	robotRotate(900,1.2,0);	}	else 
	if(LAU) { robotRotate( 1,1.2,0);		} else 
	if(LAD) { robotRotate(1800,1.2,0);	}	else 
//-------------------------------------------------
//---- control speed--------------------------------
	
	if(RR1)	{
//	tempSpeed++;	if(tempSpeed>=2){tempSpeed=2;}
					FUCK_on;
		delay_ms(500);
			
			FUCK_off;	
	} else 
	if(RR2) {
					FUCK_on;
		delay_ms(500);
			
			FUCK_off;	
//	tempSpeed--;  if(tempSpeed<=0){tempSpeed=0;}
	}	else 
//------control spin hand and suck dick----------------------------------------
	
	if(LL1) {	
		
		if(!suckDick){
			SUCK_off;
			
		}else{
			SUCK_on;	
		}
		suckDick=!suckDick;
		delay_ms(300);
	
	}	else 
	if(LL2) { 
		if(!spinHand){
		
			SPIN_on;

		}else{
			SPIN_off;}
		spinHand=!spinHand;
		delay_ms(300);
	}	else 
//------------------------------------------------
//----- control button  for xilanh ----------------------	
	
	if(R1 )  {	

			if(!xlLiftBody){
				Xilanh_2_on;

			}else{
				Xilanh_2_off;	
				
				}
			xlLiftBody=!xlLiftBody;
			delay_ms(300);
	}else
	if(R2 )  {
		if(!xlDropArm){
			Xilanh_3_on;
				}else{
			Xilanh_3_off;
				}
			xlDropArm=!xlDropArm;
			delay_ms(300);

	}else 
	
	if(R3 )  {
		if(!xlRaiseGun){
			Xilanh_4_off;
				}else{
			Xilanh_4_on;
				} 
			xlRaiseGun=!xlRaiseGun;
			delay_ms(300);

	}	else 
	if(R4 )  {

		
		if(!xlShoot){
			Xilanh_1_off;
				}else{
			Xilanh_1_on;
				} 
			xlShoot=!xlShoot;
			delay_ms(300);
		
		
	}	  else


	if(L2 )  {
		if(!tkLeft_13){
			Xilanh_5_on;
			
		}else{
			Xilanh_5_off;}
		tkLeft_13=!tkLeft_13;
		delay_ms(300);

	}	  else 
	if(L1 )  {
		if(!tkLeft_2){
			Xilanh_6_on;
		}else{
			Xilanh_6_off;} 
		tkLeft_2 =!tkLeft_2;
		delay_ms(300);

	}else 
	if(L4 )  {
		if(!tkRight_13){
			Xilanh_7_on;
		}else{
			Xilanh_7_off;}
		tkRight_13=!tkRight_13;
		delay_ms(300);

	}	else 
	if(L3 )  {	
		if(!tkRight_2){
			Xilanh_8_on;
		}else{
			Xilanh_8_off;}	
		tkRight_2=!tkRight_2;
		delay_ms(300);

	}	  else	
	
//------------------------------------------------	
	
	{	robotStop(6);	
	 //delay_ms(700) ;
	}
	
}

//==================== CHUONG TRINH CHINH ===========================
int main(void)
{ 
	if (SysTick_Config(SystemCoreClock / 1000)) while (1); // 1ms truyen du lieu usart den cac slever
	
	configInOutPort();					//khai bao ngo vào - ra cua mot chan bat ky	

	//lcd_Init();	
	
	config_TIM8_PWM();					//cai dat timer 8 o che do dieu xung
	
	//Config_TIM9_PWM(TIM_PRELOAD, TIM_PRESCALER); // PWM Stepper PE5 and PE6
	
//	Config_encoder_timer4(); 	// khai bao encorder timer 4
//	reset_en_T4();					 	// reset encoder timer 4
	
	Config_encoder_timer5();	// khai bao encorder timer 5
	reset_en_T5();				 	 	// reset encoder timer 5
	
	Config_USART2_DMA(115200);	// usart giao tiep voi laban
	
	Config_UART_4		 (19200);		// Uart4 giao tiep voi PS3
	
			/*	Config_int_time7();	// su dung de doc sieu am, tang giam bien, chng nhieu, và các chuong trình hoat dong khong song song voi he thong*/
	
	//lcd_Clear();
	//delay_ms(100);
	
	run_read_gyro_uart2();			// reset lai laban 
	
	//note();											// ghi chu hien thi LCD
	
	config_TIM7();	
	// timer 1ms chay chuong trinh control omni
	
	//	robotStop(0);	
	// delay_ms(700) ;
	//---- confix speed------------------------------------------
	
	moveSpeed=mediumSpeed;
	 
	//------------------------------------------------------------
	while(1) 
		{	
			manual_RED();
	
			
		}
}
//========================== END ====================================
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here e.g. write a character to the USART */
  //USART_SendData(EVAL_COM1, (uint8_t) ch);
							lcd_Data_Write((uint8_t)ch);
  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
 // {}
  return ch;
}
