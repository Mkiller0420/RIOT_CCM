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
//Protocol of CAN1
//for 29bit CANID, mask and location
#define FRAM_ID_MASK		0x18000000
#define FRAM_ID_LOCA		27
#define STAK_ID_MASK		0x07F00000
#define	STAK_ID_LOCA		20
#define FUNC_ID_MASK		0x000F0000
#define FUNC_ID_LOCA		16
#define	MODL_ID_MASK		0x0000F000
#define	MODL_ID_LOCA		12
#define MODL_TY_MASK		0x00000F00
#define MODL_TY_LOCA		8
#define CHAN_ID_MASK		0x000000F0
#define CHAN_ID_LOCA		4
#define COIL_ID_MASK		0x0000000F
#define	COIL_ID_LOCA		0

typedef struct{
	uint8_t 	frame_id;
	uint16_t 	stack_id;
	uint8_t 	func_id;
	uint8_t 	module_id;
	uint8_t		module_type;
	uint8_t 	channel_id;
	uint8_t		coil_id;
}AnysCanId;

void CAN_Init_App(CAN_HandleTypeDef* hcan, uint32_t ExtId);
void MX_CAN1_Init(void);
void MX_CAN2_Init(void);
void MX_CAN3_Init(void);
//HAL_StatusTypeDef CanFilter_Init(void);
HAL_StatusTypeDef CanFilter_Init(CAN_FilterConfTypeDef* pFilter, CAN_HandleTypeDef* hcan);
void FillTxMsg(CAN_HandleTypeDef* hcan, uint8_t* can_tx, uint8_t length);
//void FillTxMsg(CAN_HandleTypeDef* hcan, uint32_t RxCntr, uint32_t TxCntr);
#endif /* CAN_HW_H_ */
