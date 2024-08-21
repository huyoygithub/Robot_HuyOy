#include "stm32f4xx.h"
#include "lcd.h"
#include "main.h"

//--------------------------------------------------------------------
//unsigned char van, van2;

//vu8	valueRaise = 0;

unsigned char SanDo, SanXanh;

//========================================================================================

void _SetMode (void)
{
	if (!L1)	
		{			
			while(!L1)			
				{
					lcd_gotoxy(3,1);
					printf("L1");
				}
			//if (SanDo)		{ Red_Zone_0_1();	  }
			//if (SanXanh)	{ Blue_Zone_0_1();	}
		}
	
	//----------------------------------------------------
	if (!L2)	
		{		
			while	(!L2)
				{
					lcd_gotoxy(3,1);
					printf("L2");
				}
			//if (SanDo)		{ Red_Zone_1_3();	  }
			//if (SanXanh)	{ Blue_Zone_1_3();	}
		}
	
	//----------------------------------------------------
	if (!L3)	
		{				
			while	(!L3)
				{
					lcd_gotoxy(3,1);
					printf("L3");
				}
				
			//if (SanDo)		{ Red_Zone_4_6();	  }
			//if (SanXanh)	{ Blue_Zone_4_6();	}
		}
		
	//----------------------------------------------------
	if (!L4)	
		{			
			while (!L4)
				{
					lcd_gotoxy(3,1);
					printf("L4");
				}
			//if (SanDo)		{ Red_Zone_4_6();	  }
			//if (SanXanh)	{ Blue_Zone_4_6();	}
		}
	
	//----------------------------------------------------
	if (!R1)	
		{	
			while (!R1)
				{
					lcd_gotoxy(3,1);
					printf("R1");
				}
			//if (SanDo)		{ Red_Zone_7_9();	  }
			//if (SanXanh)	{ Blue_Zone_7_9();	}
		}
	
	//----------------------------------------------------
//	if (!R2)	{					
//		while(!R2)
//				{
//					lcd_gotoxy(3,1);
//					printf("  R2   ");
//				}
//		if (SanDo)		RedRow6();
//		if (SanXanh)	BlueRow6();
//	}
//	
//	if (!X)	{				// Cot 2m
//			while (!X)
//				{
//					lcd_gotoxy(3,1);
//					printf("  X   ");
//				}
//			if (SanDo)		RedRow7();
//			if (SanXanh)	BlueRow7();
//		}
//	
//	if (!START)	{ 		// Chay Lay Dia Tu Vung Xuat Phat
//		while(!START)
//				{
//					lcd_gotoxy(3,1);
//					printf("Start ");
//				}
//		if (SanDo)		LayThungDiaDo();
//		if (SanXanh)	LayThungDiaXanh();
//	}
//	
//	if (!SELECT)	{ 	// Chay Ve Vung Lay Dia ReTry
//		while(!SELECT)
//				{
//					lcd_gotoxy(3,1);
//					printf("Select");
//				}
//		if (SanDo)		RetryRed();
//		if (SanXanh)	RetryBlue();
//	}
//	
//	if (!L1 && RIGHT && LEFT)	{ 			// Nhan Giu Ban Lien Tuc.
//		while (!L1 && RIGHT && LEFT)	set_morProvide(100),	tuDongBan (1);
//		mor_provide = 0; mor_raise = ham;
//	}
	
//	if (!UP)	{				// Turn Up the Speed
//		SPEED_SHOT++;
//		while (!UP)	{
//		}
//	}
//	
//	if (!DOWN)	{			// Turn Down the Speed
//		SPEED_SHOT--;
//		while (!DOWN)	{
//		}
//	}
	
//	if(LJOY_LR <= 45)	{	// Nang Ha Xi Lanh Sau
//		van2 = ~van2;
//		if (van2) 	BackRaise_1_on;
//		else 				BackRaise_1_off;
//		while (LJOY_LR <= 45);
//	}
//	
//	if(LJOY_LR >= 245)	{	// Nang Ha Xi Lanh Ben
//		van = ~van;
//		if (van) 	RightRaise_on;
//		else 			RightRaise_off;
//		while (LJOY_LR >= 245);
//	}
//	
//	if(LJOY_UD <= 45) 	{					// nang thung dia
//			if(!countDisc)	capDia(),	valueRaise = ham;		}							
//			else if(LJOY_UD >= 245){	// ha thung dia
//				if (!countDisc)set_morRaiseSensor(-100);}	
//             else  set_morRaiseSensor(ham); 								// dung dong co nag
}
