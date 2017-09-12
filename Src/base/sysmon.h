/*
 * sysmon.h
 *
 *  Created on: Aug 23, 2017
 *      Author: MJC
 */

#ifndef SYSMON_H_
#define SYSMON_H_
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#define LED_Status_Pin 		GPIO_PIN_8 //LED_Status_Pin
#define LED_Alarm_Pin 		GPIO_PIN_4 //LED_RE_Pin
#define LED_Wait_Pin	 	GPIO_PIN_5 //LED_YE_Pin
#define LED_Ready_Pin 		GPIO_PIN_6 //LED_GR_Pin
#define LED_Rsvd_Pin		GPIO_PIN_7 //LED_YE2_Pin
#define LED_Status_Port	 	GPIOD
#define LED_Alarm_Port 		GPIOD
#define LED_Wait_Port	 	GPIOD
#define LED_Ready_Port 		GPIOD
#define LED_Rsvd_Port 		GPIOD


typedef enum
{
	eStatus = 0,
	eAlarm 	= 1,
	eWait 	= 2,
	eRedy 	= 3,
	eRsvd	= 4,
	eAll	= 5
}LEDNum;

typedef enum
{
	eOn 	= 1,
	eOff	= 2,
	eBlink	= 3
} LEDState;

typedef struct {
	GPIO_TypeDef* LED_Port;
	uint16_t LED_Pin;
	LEDNum num;
	LEDState state;
	uint8_t Ontime;
	uint8_t Offtime;
} LEDConfig;


void osMonitorCreateTask( osPriority priority );
void vMonitorSetLED( LEDNum num, LEDState state );
LEDConfig* vMonitorGetState(LEDNum num);
void MonitorInit(void);
#endif /* SYSMON_H_ */
