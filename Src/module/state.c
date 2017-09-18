/*
 * ccm_roit.c
 *
 *  Created on: Sep 12, 2017
 *      Author: MJC
 */
#include "..\base\sysmon.h"
#include "state.h"
#include "can_logic.h"
#include "..\base\can_hw.h"
#define statePERIOD 100
//static TaskHandle_t sysmoni_task_handle = NULL;

static osThreadId state_task_handle;
static osMessageQDef(state_queue, 16, uint8_t);
static osMessageQId state_queue_handle;

static CCMConfig ccm;

static void prvStateTask( void const * argument );
static void InitState(){
	ccm.moudle_type[0]=0;
	for (uint8_t i=1;i<MAX_STACK_NUM;i++){
		ccm.moudle_type[i] = 0xFF;
	}
}
static void prvStateTask( void const * argument )
{
	(void)argument;
	InitState();
	for (;;) {
		osEvent evt = osMessageGet(state_queue_handle, statePERIOD);
		if (evt.status == osEventMessage) {
			switch((uint8_t)evt.value.signals){
			case sCan3WaitConfig:
				if (ccm.systate_curr == sReset){
					osCan3CreateTask(CAN3_PROI);
					vMonitorSetLED(eAll, eOff);
					vMonitorSetLED(eWait, eBlink);
					ccm.systate_prev = ccm.systate_curr;
					ccm.systate_curr = evt.value.signals;
				}else{

					//vStateSet(sError);
				}
				break;
			case sCCMWaitConfig:
				if (ccm.systate_curr == sCan3WaitConfig){
					vMonitorSetLED(eAll, eOff);
					vMonitorSetLED(eRsvd, eBlink);
					uint32_t stack_id = hcan3.pRxMsg->Data[2];
					uint32_t can_id_init = 0 | stack_id<<STAK_ID_LOCA;
					osCan1CreateTask(CAN1_PROI, can_id_init);
					ccm.systate_prev = ccm.systate_curr;
					ccm.systate_curr = evt.value.signals;
				}
				break;
			case sMODWaitConfig:
				if (ccm.systate_curr == sCCMWaitConfig){
					vMonitorSetLED(eAll, eOff);
					vMonitorSetLED(eRedy, eBlink);
					ccm.systate_prev = ccm.systate_curr;
					ccm.systate_curr = evt.value.signals;
				}
				break;
			case sError:
				vMonitorSetLED(eAll, eOff);
				vMonitorSetLED(eAlarm, eBlink);
				ccm.systate_prev = ccm.systate_curr;
				ccm.systate_curr = evt.value.signals;
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
	if(st<0||st>sError){
		//st = sError;
	}
	osMessagePut(state_queue_handle, (uint8_t)st, 0);
}

void vStateSet( SyState st )
{
	prvStateNotifyTask(st);
}

CCMConfig* vStateGet(void){
	return &ccm;
}

void CCMConfigeration(uint8_t coil_id, const char *data, uint32_t length){
	uint8_t module_num;
	uint8_t module_type;
	static uint8_t i,j;
	if(ccm.systate_curr==sCCMWaitConfig && coil_id == 0x00){
		for (i=0;i<length;i++){
			module_num 	= (uint8_t) data[i]>>4;
			module_type = (uint8_t) data[i]&0xF;
			if (module_num>9|| module_num<1){
				vStateSet(sError);
				break;
			}
			if (module_type == 0x0F){
				for (j=module_num;j<MAX_STACK_NUM;j++){
					ccm.moudle_type[j] = 0x0F;
				}
				break;
			}
			else{
				ccm.moudle_type[module_num] = module_type;
			}

		}
	}else{
		vStateSet(sError);
	}
	for (j=0;j<MAX_STACK_NUM;){
		if(ccm.moudle_type[j]>0x0F) {break;}
		j++;
	}
	if (j>=MAX_STACK_NUM){
		vStateSet(sMODWaitConfig);
		ccm.ccm_is_configed = true;
	}
}
