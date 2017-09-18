/*
 * ccm_roit.h
 *
 *  Created on: Sep 12, 2017
 *      Author: MJC
 */

#ifndef MODULE_STATE_H_
#define MODULE_STATE_H_
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include <stdbool.h>

#define STATE_PROI		osPriorityHigh
#define MAX_STACK_NUM	10
typedef enum{
	sReset			= 0 ,
	sCan3WaitConfig		,	//power on and wait for CCM config CAN1 ID though CAN3
	sCCMWaitConfig		,	//CAN1 is configed by though CAN3 and wait for PC
							//to config the CCM about the stack
	sMODWaitConfig		,	//CCM is configed by PC, wait for PC to config the stack
	sReadytoRun	 		,	//all modules are configed, the stack is ready to run
	sRunning			,	//the stack is running
	sError
}SyState;

typedef struct{
	uint32_t 	can1_id;
	uint32_t 	pc_id;
	uint8_t  	moudle_type[MAX_STACK_NUM];
	bool		can1_is_configed;
	bool 		ccm_is_configed;
	bool		stack_is_configed;
	SyState		systate_curr;
	SyState		systate_prev;

}CCMConfig;
void osStateCreateTask( osPriority priority );
void vStateSet( SyState st );
CCMConfig* vStateGet(void);
void CCMConfigeration(uint8_t coil_id, const char *data, uint32_t length);
#endif /* MODULE_STATE_H_ */
