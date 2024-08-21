
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

//#else
// #error "Please select first the Evaluation board used in your application (in Project Options)"
//#endif

u8 dataBuff[6];

#define L1  ((dataBuff[0]&0x10)==0?1:0)
#define L2  ((dataBuff[0]&0x80)==0?1:0)
#define L3  ((dataBuff[0]&0x40)==0?1:0)
#define L4  ((dataBuff[0]&0x20)==0?1:0)

#define R1  ((dataBuff[1]&0x10)==0?1:0)
#define R2  ((dataBuff[1]&0x80)==0?1:0)
#define R3  ((dataBuff[1]&0x40)==0?1:0)
#define R4  ((dataBuff[1]&0x20)==0?1:0)

#define LL1 ((dataBuff[1]&0x04)==0?1:0)
#define LL2 ((dataBuff[1]&0x01)==0?1:0)
#define RR1 ((dataBuff[1]&0x08)==0?1:0)
#define RR2 ((dataBuff[1]&0x02)==0?1:0)

#define SELECT ((dataBuff[0]&0x01)==0?1:0)
#define START  ((dataBuff[0]&0x08)==0?1:0)  

#define LAL (dataBuff[4]==0?1:0)
#define LAR (dataBuff[4]==255?1:0)

#define LAU (dataBuff[5]==0?1:0)
#define LAD (dataBuff[5]==255?1:0)

#define RAL (dataBuff[2]==0?1:0)
#define RAR (dataBuff[2]==255?1:0)

#define RAU (dataBuff[3]==0?1:0)
#define RAD (dataBuff[3]==255?1:0)

/* Exported define -----------------------------------------------------------*/
void delay_ms(__IO uint32_t num);
void delay_us(__IO uint32_t num);

void gp_update_btn(void);



void printString (char row, char column, int temp);

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

