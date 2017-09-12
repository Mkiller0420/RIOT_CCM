/*
 * adread.c
 *
 *  Created on: Aug 29, 2017
 *      Author: MJC
 */

#include "adread.h"
#include "sysmon.h"
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim6;

/// ADC sample
uint16_t ADC_RAW[LED_ADC_NUM] = {0};

/*************** TIMER ***************/

void HwStartPeriodicTimer(void)
{
	//HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim6);
	//HAL_TIM_Base_Start_IT(&htim7);
}

/*************** ADC ***************/

HAL_StatusTypeDef AdcSample(void)
{
	HAL_StatusTypeDef StaAdc = HAL_ERROR;
	uint8_t pin_curr, pin_1s_before;
	//assert(led_input[1].pin==led_input[2].pin);
	pin_curr = led_input[1].pin;
	if (pin_curr==ADC_SAMPLE_DEEP-1){
		pin_1s_before = 0;
	}else{
		pin_1s_before = pin_curr+1;
	}

	StaAdc = HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_RAW, LED_ADC_NUM);

	for (uint8_t i=0;i<LED_NUM;i++){
		for (uint8_t j=0;j<3;j++){
			led_input[i].led_color[pin_curr][j]=ADC_RAW[i*3+j];
			if (led_input[i].led_color[pin_curr][j]>RED_LINE &&
					led_input[i].led_color[pin_1s_before][j]<=RED_LINE){
				led_input[i].color_duty[j]++;
			}
			else if (led_input[i].led_color[pin_curr][j]<=RED_LINE &&
					led_input[i].led_color[pin_1s_before][j]>RED_LINE){
				led_input[i].color_duty[j]--;
			}
			/*if (j<2){
				LEDConfig* led = vMonitorGetState(i*3+j);
				if (led_input[i].color_duty[j]>50 && led->state==eOff){
					vMonitorSetLED(i*3+j, eOn);
				}else if (led_input[i].color_duty[j]<=50 && led->state==eOn){
					vMonitorSetLED(i*3+j, eOff);
				}
			}*/

		}
		if (led_input[i].pin>=ADC_SAMPLE_DEEP-1){
			led_input[i].pin=0;
		}
		else{
			led_input[i].pin++;
		}
	}

	return StaAdc;
}
