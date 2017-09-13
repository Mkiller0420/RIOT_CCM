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

#define STATE_PROI	osPriorityHigh

typedef enum{
	sReset			= 0,
	sWaitforCan3	,
	sWaitforPC		,
	sConfigging	 	,
	sReadytoRun	 	,
	sRunning		,
	sError
}SyState;

typedef struct{
	uint32_t 	can1_id;
	uint32_t 	pc_id;
	uint8_t  	module_num;
	uint8_t  	moudle_type[10];
	bool		can1_is_configed;
	bool		stack_is_configed;
	SyState		systate_curr;
	SyState		systate_prev;

}CCMConfig;
void osStateCreateTask( osPriority priority );
void vStateSet( SyState st );
CCMConfig* vStateGet(void);
#endif /* MODULE_STATE_H_ */
