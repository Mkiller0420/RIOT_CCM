/*
 * can_logic.h
 *
 *  Created on: Aug 31, 2017
 *      Author: MJC
 */

#ifndef CAN_LOGIC_H_
#define CAN_LOGIC_H_

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
//// CAN test
//void FillTxMsg(CAN_HandleTypeDef* hcan, uint32_t RxCntr, uint32_t TxCntr);
/// CAN

void osCan1CreateTask( osPriority priority );
void osCan2CreateTask( osPriority priority );
void osCan3CreateTask( osPriority priority );

osMessageQId can1_queue_handle;
osMessageQId can2_queue_handle;
osMessageQId can3_queue_handle;

#endif /* CAN_LOGIC_H_ */
