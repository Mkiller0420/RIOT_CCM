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

typedef enum
{
	sReset			= 0,
	sWaitforCan3	,
	sWaitforPC		,
	sConfigging	 	,
	sReadytoRun	 	,
	sRunning		,
	sError
}SyState;

void osStateCreateTask( osPriority priority );
void vStateSet( SyState st );
SyState vStateGet(void);
#endif /* MODULE_STATE_H_ */
