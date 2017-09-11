/*
 * adread.h
 *
 *  Created on: Aug 29, 2017
 *      Author: MJC
 */

#ifndef ADREAD_H_
#define ADREAD_H_
#include "stm32f4xx_hal.h"

#define LED_NUM				2
#define LED_ADC_NUM			LED_NUM*3
//#define	LED_ADC_NUM			6
#define	ADC_SAMPLE_DEEP		100
#define RED_LINE			580
#define GRN_LINE			500
#define BLU_LINE			100
/*************** ADC ***************/
typedef enum{
	RED=0,
	GRN,
	BLU
}Color_CHANNEL;

typedef struct{
	uint16_t 	led_color[100][3];
	uint8_t 	color_duty[3];
	uint8_t 	pin;
}LED_STATE;


HAL_StatusTypeDef AdcSample(void);

void HwStartPeriodicTimer(void);

#endif /* ADREAD_H_ */
