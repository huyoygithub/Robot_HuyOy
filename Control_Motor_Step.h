
#include "stm32f4xx.h"

typedef enum{
	CW=0,     																	// Theo chieu kim dong ho
	CCW,																				// Nguoc chieu kim dong ho
	STOP,																				// Dung Lai
	ACCEL,																			// Gia Toc
	DECEL,																			// Giam Toc
	RUN,																				// Chay voi toc do khong doi
	R_MOTOR,																		// Quay Angle Wheel Front
	F_MOTOR																			// Quay Angle Wheel Rear
}Motor_Status;

//=================================================================================================================
typedef struct{
	uint8_t 					dir;                      // Huong quay Motor Step
	int16_t 					total_count;							// Dem Tong So
	int16_t 					step_arr;                 // Gia tri arr, So luong xung don
	uint32_t 					decel_start;              // So luong xung vao luc bat dau GIAM TOC
	int32_t 					accel_count;              // So luong xung vao luc bat dau GIA TOC
	uint8_t 					run_state;                // Trang thai chay
	Motor_Status 			R_MOTOR;									// Ghi lai So dong co
	Motor_Status 			F_MOTOR;									// Ghi lai So dong co
}SpeedData;

//=================================================================================================================
extern uint8_t 			Tatus_Angle_R;						// Trang thai co phai Goc Banh Sau Dang Thay Doi Khong
extern uint8_t 			Tatus_Angle_F;  					// Trang thai co phai Goc Banh Truoc Dang Thay Doi Khong

extern uint32_t 		Angle_Wheel_Rear;					// Goc Hien Tai Cua Banh Sau
extern uint32_t 		Angle_Wheel_Front;        // Goc Hien Tai Cua Banh Truoc

extern SpeedData 	srd;      									// Ban ghi trang thai dong co tay X/Z

static double exp(double x);

static void 	Calculate_S_Model_Line				( float fre_max, float fre_min);
void 					X_ENA													( TIM_TypeDef* MOTOx, FunctionalState NewState);

void 					Motor_Config									( SpeedData temp);
void			 		Motor_Speed_Adjust						( TIM_TypeDef * TIM, SpeedData *m_srd);
void 					Motor_Move										( int32_t step, float fre_max, float fre_min, Motor_Status flag);
void 					Motor_Move_Abs								( int32_t step, float fre_max, float fre_min, Motor_Status flag);

void 					Control_Test									(int32_t X_mm, int32_t Z_mm);
void 					Control_Try_Motor_NEMA_23			(void) ;
//================================================= END ============================================================
