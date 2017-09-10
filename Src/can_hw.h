/*
 * canprivate.h
 *
 *  Created on: Aug 31, 2017
 *      Author: MJC
 */

#ifndef CAN_HW_H_
#define CAN_HW_H_

#include "stm32f4xx_hal.h"

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;
CAN_HandleTypeDef hcan3;
/*************** CAN ***************/


void CAN_Init_App(CAN_HandleTypeDef* hcan, uint32_t ExtId);
void MX_CAN1_Init(void);
void MX_CAN2_Init(void);
void MX_CAN3_Init(void);
//HAL_StatusTypeDef CanFilter_Init(void);
HAL_StatusTypeDef CanFilter_Init(CAN_FilterConfTypeDef* pFilter, CAN_HandleTypeDef* hcan);
void FillTxMsg(CAN_HandleTypeDef* hcan, uint8_t* can_tx, uint8_t length);
//void FillTxMsg(CAN_HandleTypeDef* hcan, uint32_t RxCntr, uint32_t TxCntr);
#endif /* CAN_HW_H_ */
