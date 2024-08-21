
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "Control_Motor_Step.h"

//============================== Quay Goc Banh Sau =================================
#define Dir_Angel_Wheel_R_Next       						GPIO_SetBits   (GPIOA, GPIO_Pin_11)				// Chieu Quay Motor Step . Pin PA11
#define Dir_Angel_Wheel_R_Back     							GPIO_ResetBits (GPIOA, GPIO_Pin_11)

// TIMER PULSE : PE5
#define Angel_Wheel_R_TIMx				 							TIM9																			// Chi Can Sua So TIMER La OK
#define Angel_Wheel_R_TIM_SetAutoreload  				TIM_SetAutoreload 
#define Angel_Wheel_R_TIM_SetCompare    				TIM_SetCompare2														// Chi Can Sua Kenh TIMER La OK
#define Angel_Wheel_R_OCInit										TIM_OC2Init																// Chi Can Sua Kenh TIMER La OK

#define Angel_Wheel_R_TIM_EnableOC							Enable_TIMX_OCXInit  (Angel_Wheel_R_TIMx, Angel_Wheel_R_OCInit)
#define Angel_Wheel_R_TIM_DisableOC							Disable_TIMX_OCXInit (Angel_Wheel_R_TIMx, Angel_Wheel_R_OCInit)

//============================= Quay Goc Banh Truoc ================================
// DIR
#define Dir_Angel_Wheel_F_Next       						GPIO_SetBits  (GPIOC, GPIO_Pin_13)				// Chieu Quay Motor Step  . Pin PC13
#define Dir_Angel_Wheel_F_Back     							GPIO_ResetBits(GPIOC, GPIO_Pin_13)

// TIMER PULSE : PE6
#define Angel_Wheel_F_TIMx				 							TIM9																			// Chi Can Sua So TIMER La OK
#define Angel_Wheel_F_TIM_SetAutoreload  				TIM_SetAutoreload 
#define Angel_Wheel_F_TIM_SetCompare    				TIM_SetCompare1														// Chi Can Sua Kenh TIMER La OK
#define Angel_Wheel_F_OCInit										TIM_OC1Init																// Chi Can Sua Kenh TIMER La OK

#define Angel_Wheel_F_TIM_EnableOC							Enable_TIMX_OCXInit ( Angel_Wheel_F_TIMx, Angel_Wheel_F_OCInit )
#define Angel_Wheel_F_TIM_DisableOC							Disable_TIMX_OCXInit( Angel_Wheel_F_TIMx, Angel_Wheel_F_OCInit )

//==================================================================================
#define TIM_PRESCALER      											27                              					// Bo Dem TIMER Motor Step//9
#define TIM_PRELOAD															9999																			// Gia Tri Bo Dem TIMER Motor Step
#define TIM_FREQ     														(SystemCoreClock/(TIM_PRESCALER + 1))     // Tan So TIMER Motor Step
#define SPR         														800                                       // So Xung De Motor Step Quay 1 Vong , dong co khong doi //90

#define STEP_S																	150                                     		// So Luong BUOC TANG TOC tu 0
#define FLEXIBLE    														4.0																		  // He so dieu chinh duong cong chu S

#define ALPHA       														((float)((2*3.1415*100/SPR)/100))       	// Radian cua moi Xung, Goc Buoc
#define A_T         														((float)(ALPHA*TIM_FREQ))

//==================================================================================
SpeedData srd;

uint8_t 	Tatus_Angle_R = 0;									// Trang Thai: Co Phai Goc Banh Sau   Dang Thay Doi Khong ?		Neu gia tri la : 0-Stop , 1-Run
uint8_t 	Tatus_Angle_F = 0;  								// Trang Thai: Co Phai Goc Banh Truoc Dang Thay Doi Khong ?	  Neu gia tri la : 0-Stop , 1-Run

uint32_t 	Angle_Wheel_Rear 		= 0;            // Goc Hien Tai Cua Banh Sau
uint32_t 	Angle_Wheel_Front 	= 0;            // Goc Hien Tai Cua Banh Truoc

static float 		__FRE[STEP_S] = {0.0};  			// BO DEM TAN SO
static uint16_t __ARR[STEP_S] = {0};					// TAI LAI BO DEM GIA TRI

//================================== LUY THUA THU n cua Ham SO e ===================================
static double exp								( double x )															
{
    uint16_t i = 0;
	
    x = 1.0 + x/256;
	
    for(; i<8; i++)  {  x = x*x;	}
    return x;
}
//================================== Tinh Mang Duong Cong Chu S ====================================
static void Calculate_S_Model_Line	( float fre_max, float fre_min) 
	// fre_max ( Tan so toi da)
	// fre_min ( Tan suat toi thieu) 
	// flexible( He so dieu chinh duong cong S )	
{
    int32_t i = 0;
    float 	x = 0.0 ,y = 0;
    float D_value = fre_max - fre_min;
	
    for(; i<STEP_S; i++)
				{
						x = FLEXIBLE*(i-(STEP_S/2))/(STEP_S/2);           // CHUAN HOA
						y = 1.0/(1+exp(-x));
				
						__FRE[i] = D_value*y + fre_min;
						__ARR[i] = (uint16_t)((TIM_FREQ/__FRE[i]) -  1);
				}
}

//================================== Cau HInh Trang Thai Motor =====================================
void Motor_Config								(	SpeedData temp	)											
{
		srd = temp;
	
//		if(R_MOTOR == srd.R_MOTOR)
//				{
					if(CW == srd.dir) Dir_Angel_Wheel_R_Next;	else if(CCW == srd.dir) Dir_Angel_Wheel_R_Back; 
					
					TIM_PrescalerConfig									(Angel_Wheel_R_TIMx, TIM_PRESCALER, TIM_PSCReloadMode_Immediate); 
					
					Angel_Wheel_R_TIM_SetAutoreload			(Angel_Wheel_R_TIMx, srd.step_arr);
					Angel_Wheel_R_TIM_SetCompare				(Angel_Wheel_R_TIMx, (srd.step_arr/2));
//				}
//		else 
//			if(F_MOTOR == srd.R_MOTOR)
//				{
//					if(CW == srd.dir) Dir_Angel_Wheel_F_Next;	else if(CCW == srd.dir) Dir_Angel_Wheel_F_Back;

//					TIM_PrescalerConfig							( Angel_Wheel_F_TIMx, TIM_PRESCALER, TIM_PSCReloadMode_Immediate); 
//					
//					Angel_Wheel_F_TIM_SetAutoreload	( Angel_Wheel_F_TIMx, srd.step_arr);
//					Angel_Wheel_F_TIM_SetCompare		( Angel_Wheel_F_TIMx, (srd.step_arr/2));
//				}
}

//==================================== CHUYEN DONG TUYET DOI =======================================
void Motor_Move									( int32_t step, float fre_max, float fre_min, Motor_Status flag) 				
	// step		: ( SO BUOC Motor CHAY )	
	// fre_max: (	Tan so hoat dong Motor Max ) 
	// fre_min: (	Tan so hoat dong Motor Min )
	// flag		:	( So Motor )
{
		SpeedData temp;
	//	temp.total_count =0;
	
		temp.R_MOTOR = flag;
    Calculate_S_Model_Line(fre_max, fre_min);     // Tinh toan S-duong cong mang
		
    // X_ENA(X_TIMx, ENABLE);
    // Ðat huong di chuyen, buoc <0 nguoc chieu kim dong ho,> 0 theo chieu kim dong ho
    if(step < 0)
			{
				temp.dir 	= 	CCW; 
				step 			= 	-step; 
			}
		else	{		temp.dir = CW;   }

		temp.total_count = step;    // GHI LAI TONG SO BUOC CAN THUC HIEN

    // KHI GOC QUAY RAT NHO BAT DAU CHAY TRUC TIEP O TAN SO BAT DAU
    if(step == 1)
			{
					temp.accel_count 				= 1;
					temp.step_arr 					= (int16_t)((TIM_FREQ/fre_min) - 1);    // TINH TOAN GIA TRI TAI LAI
					temp.run_state 					= DECEL;
					Tatus_Angle_R 					= 1;
			}
		else 
			if(step != 0)
					{
						if(step <= (STEP_S*2))
							{
								temp.step_arr 						= __ARR[0];
								temp.accel_count 					= (int32_t)(step/2);
								temp.decel_start 					= temp.accel_count;
								temp.run_state 						= ACCEL;
								Tatus_Angle_R 						= 1;	
							}
						else
							{
								temp.step_arr 						= __ARR[0];
								temp.accel_count 					= STEP_S;
								temp.decel_start 					= step - STEP_S;
								temp.run_state 						= ACCEL;
								Tatus_Angle_R 						= 1;
							}
					}
		
		Motor_Config(temp);

		// BAT KENH DAU RA TIM
		//if(R_MOTOR == srd.R_MOTOR) 			Angel_Wheel_R_TIM_EnableOC;		else 	if( F_MOTOR == srd.R_MOTOR ) 	Angel_Wheel_F_TIM_EnableOC;
		
		Angel_Wheel_R_TIM_EnableOC;
		Angel_Wheel_F_TIM_EnableOC;
						
    TIM_Cmd(Angel_Wheel_R_TIMx, ENABLE); // KICH HOAT
		TIM_Cmd(Angel_Wheel_F_TIMx, ENABLE); // KICH HOAT
}

//==================================== CHUYEN DONG TUONG DOI ======================================
void Motor_Move_Abs							( int32_t step, float fre_max, 	float fre_min, 	Motor_Status flag) 	
{
		SpeedData temp;  

		Calculate_S_Model_Line(fre_max, fre_min);
  
    step 											= step - Angle_Wheel_Rear;         // DO DICH CHUYEN TUYET DOI
		temp.R_MOTOR 	= flag;
	
    // X_ENA(X_TIMx, ENABLE);
    // X_ENA(X_TIMx, ENABLE);
    // Ðat huong di chuyen, buoc <0 nguoc chieu kim dong ho,> 0 theo chieu kim dong ho
    if(step < 0)
				{
					temp.dir 	= CCW; 
					step 			= -step; 
				}
		else	{	temp.dir = CW; }

		temp.total_count = step;    // GHI LAI TONG SO BUOC CAN THUC HIEN

    // KHI GOC QUAY RAT NHO BAT DAU CHAY TRUC TIEP O TAN SO BAT DAU
    if(step == 1)
				{
						temp.accel_count 					= 1;
						temp.step_arr 						= (int16_t)((TIM_FREQ/fre_min) - 1);    // TINH GIA TRI TAI LAI
						temp.run_state 						= DECEL;
						Tatus_Angle_R 						= 1;
				}
		else if(step != 0)
						{
							if(step <= (STEP_S*2))
									{
											temp.step_arr 					= __ARR[0];
											temp.accel_count 				= (int32_t)(step/2);
											temp.decel_start 				= temp.accel_count;
											temp.run_state 					= ACCEL;
											Tatus_Angle_R 					= 1;	
									}
							else
									{
										temp.step_arr 						= __ARR[0];
										temp.accel_count 					= STEP_S;
										temp.decel_start 					= step - STEP_S;
										temp.run_state 						= ACCEL;
										Tatus_Angle_R 						= 1;
									}
						}
		Motor_Config(temp);			// KHOI TAO CAU HINH DONG CO	

		// BAT KENH DAU RA TIM	
		//if(R_MOTOR == srd.R_MOTOR) Angel_Wheel_R_TIM_EnableOC;		else if( F_MOTOR == srd.R_MOTOR) Angel_Wheel_F_TIM_EnableOC;
		
		Angel_Wheel_R_TIM_EnableOC;
		Angel_Wheel_F_TIM_EnableOC;
						
    TIM_Cmd(Angel_Wheel_R_TIMx, ENABLE); // KICH HOAT
		TIM_Cmd(Angel_Wheel_F_TIMx, ENABLE); // KICH HOAT
}

//===================================== Dieu chinh toc do Motor ====================================
void Motor_Speed_Adjust					( TIM_TypeDef *TIM, SpeedData *m_srd)				
{
	static uint16_t step_count = 0;                 // TONG SO BUOC DEM
	
	switch(m_srd->run_state)
		{
				case STOP:// Motor Step Dung lai
						step_count 					= 0;
						Tatus_Angle_R 			= 0;        			// DAT TRANG THAI BAN GHI
				
						TIM_Cmd(TIM, DISABLE);
						//X_ENA_RESET;
						
						if(R_MOTOR == srd.R_MOTOR) 	Angel_Wheel_R_TIM_DisableOC;	else if(F_MOTOR == srd.R_MOTOR) 	Angel_Wheel_F_TIM_DisableOC;

				break;
				
				//-------------------------------------------------------
				case ACCEL:// Motor Step Gia toc
						step_count++;

						if(m_srd->dir==CW)	{	Angle_Wheel_Rear++;}	else	{	Angle_Wheel_Rear--;}

						if(step_count < m_srd->accel_count)
								{
										m_srd->step_arr 	= __ARR[step_count];
										m_srd->run_state 	= ACCEL;
								}
						else 
								if(step_count >= m_srd->decel_start)
									{  // CHUYEN TRUC TIEP DEN GIAI DOAN GIAM TOC
										m_srd->step_arr 	= __ARR[step_count-1];
										m_srd->run_state 	= DECEL;
									}
						else 
								if(step_count >= STEP_S)
									{    
										// VAO GIAI DOAN TOC DO DONG NHAT
										m_srd->step_arr 	= __ARR[STEP_S-1];
										m_srd->run_state 	= RUN;
									}
				break;
						
				//-------------------------------------------------------
				case RUN:// Motor Step Chay voi toc do khong doi
						step_count++;

						if(m_srd->dir==CW)	{	Angle_Wheel_Rear++;}	else	{	Angle_Wheel_Rear--;}

						if(step_count >= m_srd->decel_start)
								{
										m_srd->step_arr 	= __ARR[STEP_S-1];
										m_srd->run_state 	= DECEL;
								}
				break;
								
				//-------------------------------------------------------
				case DECEL:// Motor Step Giam toc
						step_count++;

						if(m_srd->dir==CW)	{	Angle_Wheel_Rear++;}	else	{	Angle_Wheel_Rear--;}
						
						if(step_count < m_srd->total_count)	{	m_srd->step_arr = __ARR[m_srd->total_count-step_count];}	else	{	m_srd->run_state = STOP;}
				break;
				//-------------------------------------------------------
				default:
		break;
		}
}

//============================ Xu ly Ngat Timer 9, Dieu Chinh Toc Do Motor =========================
void TIM1_BRK_TIM9_IRQHandler				( void )						
{
    if(TIM_GetITStatus(Angel_Wheel_R_TIMx, TIM_IT_Update) != RESET)
				{
						TIM_ClearITPendingBit(Angel_Wheel_R_TIMx, TIM_IT_Update);
			 
						Motor_Speed_Adjust							( Angel_Wheel_R_TIMx, &srd);  																					// Dieu chinh toc do Motor 
					
						Angel_Wheel_R_TIM_SetAutoreload	( Angel_Wheel_R_TIMx, srd.step_arr);
					
						if( R_MOTOR == srd.R_MOTOR) Angel_Wheel_R_TIM_SetCompare(Angel_Wheel_R_TIMx, (srd.step_arr/2));      		// DAT CCR
					
						else if(F_MOTOR == srd.R_MOTOR) Angel_Wheel_F_TIM_SetCompare( Angel_Wheel_R_TIMx, (srd.step_arr/2)); 		// DAT CCR
				}
}
//============================================== END ===============================================
