/*
 * ccm_roit.c
 *
 *  Created on: Sep 12, 2017
 *      Author: MJC
 */
#include "..\base\sysmon.h"
#include "state.h"
#include "can_logic.h"
#define statePERIOD 100
//static TaskHandle_t sysmoni_task_handle = NULL;
static osThreadId state_task_handle;
static osMessageQDef(state_queue, 16, uint8_t);
static osMessageQId state_queue_handle;

static SyState systate_curr = sReset;
static SyState systate_prev = sReset;

static void prvStateTask( void const * argument );

static void prvStateTask( void const * argument )
{
	(void)argument;
	for (;;) {
		osEvent evt = osMessageGet(state_queue_handle, statePERIOD);
		if (evt.status == osEventMessage) {
			switch((uint8_t)evt.value.signals){
			case sWaitforCan3:
				if (systate_curr == sReset){
					osCan3CreateTask(osPriorityNormal);
					vMonitorSetLED(eAll, eOff);
					vMonitorSetLED(eWait, eBlink);
					systate_prev = systate_curr;
					systate_curr = evt.value.signals;
				}else{

					vStateSet(sError);
				}
				break;
			case sWaitforPC:
				if (systate_curr == sWaitforCan3){
					vMonitorSetLED(eAll, eOff);
					vMonitorSetLED(eRsvd, eBlink);

					systate_prev = systate_curr;
					systate_curr = evt.value.signals;
				}
				break;
			case sError:
				vMonitorSetLED(eAll, eOff);
				vMonitorSetLED(eAlarm, eBlink);
				systate_prev = systate_curr;
				systate_curr = evt.value.signals;
				break;
			}
		}
	}
}

void osStateCreateTask( osPriority priority )
{
	osThreadDef(StateTask, prvStateTask, priority, 0, configMINIMAL_STACK_SIZE);
	state_task_handle = osThreadCreate(osThread(StateTask), NULL);
	state_queue_handle = osMessageCreate(osMessageQ(state_queue), NULL);
}

static void prvStateNotifyTask( SyState st)
{
	osMessagePut(state_queue_handle, (uint8_t)st, 0);
}

void vStateSet( SyState st )
{
	prvStateNotifyTask(st);
}

SyState vStateGet(void){
	return systate_curr;
}
