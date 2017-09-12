/*
 * canprivate.c
 *
 *  Created on: Aug 31, 2017
 *      Author: MJC
 */

#include "main.h"

#include "can_hw.h"
#include "..\module\can_logic.h"
#include <string.h>
/*************** CAN ***************/

uint32_t Can1RxCntr = 0;
uint32_t Can1TxCntr = 0;

uint32_t Can2RxCntr = 0;
uint32_t Can2TxCntr = 0;

uint32_t Can3RxCntr = 0;
uint32_t Can3TxCntr = 0;

static CanTxMsgTypeDef Can1TxMsg;
static CanRxMsgTypeDef Can1RxMsg;

static CanTxMsgTypeDef Can2TxMsg;
static CanRxMsgTypeDef Can2RxMsg;

static CanTxMsgTypeDef Can3TxMsg;
static CanRxMsgTypeDef Can3RxMsg;
HAL_StatusTypeDef CanxStaTx = HAL_ERROR;

void CAN_Init_App(CAN_HandleTypeDef* hcan, uint32_t ExtId)
{
	uint8_t i = 0;

	hcan->pTxMsg->StdId = 0x0;
	hcan->pTxMsg->ExtId = ExtId;//0x1111111;
	hcan->pTxMsg->IDE = CAN_ID_EXT;
	hcan->pTxMsg->RTR = CAN_RTR_DATA;
	hcan->pTxMsg->DLC = 8;//hcan->pRxMsg->DLC;
	for ( i = 0 ; i < 8 ; i++)
	{
		hcan->pRx1Msg->Data[i] = 0;
		hcan->pRxMsg->Data[i] = 0;
		hcan->pTxMsg->Data[i] = 0;
	}
}

HAL_StatusTypeDef CanFilter_Init(CAN_FilterConfTypeDef* pFilter, CAN_HandleTypeDef* hcan)
{
	HAL_StatusTypeDef CfgFltSta = HAL_OK;

	pFilter->FilterIdHigh = 0x0000;
	pFilter->FilterIdLow = 0x0000;//0x0123;
	pFilter->FilterMaskIdHigh = 0x0000;
	pFilter->FilterMaskIdLow = 0x0000;//0x0123;
	pFilter->FilterFIFOAssignment = CAN_FILTER_FIFO0;
	//pFilter->FilterNumber = 0;
	pFilter->FilterNumber = (hcan->Instance == CAN2) ? 14 : 0;
	pFilter->FilterMode = CAN_FILTERMODE_IDMASK;
	pFilter->FilterScale = CAN_FILTERSCALE_32BIT;
	pFilter->FilterActivation = ENABLE;
	//pFilter->BankNumber = 0;
	pFilter->BankNumber = (hcan->Instance == CAN2) ? 14 : 0;

	CfgFltSta = HAL_CAN_ConfigFilter(hcan, pFilter);

	if (CfgFltSta != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	return CfgFltSta;
}

// for test only
void FillTxMsg(CAN_HandleTypeDef* hcan, uint8_t* can_tx, uint8_t length)
{
	memcpy(&hcan->pTxMsg->Data, can_tx, length);
	hcan->pTxMsg->DLC = length;
	HAL_CAN_Transmit_IT(hcan);
	//HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
}

void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan)
{
	HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{
	//CanxStaTx = HAL_CAN_Transmit(hcan, 10);

	switch((uint32_t)(hcan->Instance))	{
	case (uint32_t)CAN1:
			Can1RxCntr++;
	FillTxMsg(hcan, (uint8_t*)&Can1RxCntr,4);
	Can1TxCntr++;
	break;
	case (uint32_t)CAN2:
			Can2RxCntr++;
	FillTxMsg(hcan, (uint8_t*)&Can2RxCntr,4);
	Can2TxCntr++;
	break;
	case (uint32_t)CAN3:
			Can3RxCntr++;
	//FillTxMsg(hcan, (uint8_t*)&Can3RxCntr,4);
	//Can3TxCntr++;
	osMessagePut(can3_queue_handle, Can3RxCntr, 0);
	break;
	default:
		break;
	}
}

/* CAN1 init function */
void MX_CAN1_Init(void)
{

	hcan1.Instance = CAN1;
	hcan1.pTxMsg = &Can1TxMsg;
	hcan1.pRxMsg = &Can1RxMsg;
	hcan1.Init.Prescaler = 10;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.SJW = CAN_SJW_1TQ;
	hcan1.Init.BS1 = CAN_BS1_7TQ;
	hcan1.Init.BS2 = CAN_BS2_2TQ;
	hcan1.Init.TTCM = DISABLE;
	hcan1.Init.ABOM = DISABLE;
	hcan1.Init.AWUM = DISABLE;
	hcan1.Init.NART = DISABLE;
	hcan1.Init.RFLM = DISABLE;
	hcan1.Init.TXFP = DISABLE;
	if (HAL_CAN_Init(&hcan1) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

}

/* CAN2 init function */
void MX_CAN2_Init(void)
{

	hcan2.Instance = CAN2;
	hcan2.pTxMsg = &Can2TxMsg;
	hcan2.pRxMsg = &Can2RxMsg;
	hcan2.Init.Prescaler = 10;
	hcan2.Init.Mode = CAN_MODE_NORMAL;
	hcan2.Init.SJW = CAN_SJW_1TQ;
	hcan2.Init.BS1 = CAN_BS1_7TQ;
	hcan2.Init.BS2 = CAN_BS2_2TQ;
	hcan2.Init.TTCM = DISABLE;
	hcan2.Init.ABOM = DISABLE;
	hcan2.Init.AWUM = DISABLE;
	hcan2.Init.NART = DISABLE;
	hcan2.Init.RFLM = DISABLE;
	hcan2.Init.TXFP = DISABLE;
	if (HAL_CAN_Init(&hcan2) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

}

/* CAN3 init function */
void MX_CAN3_Init(void)
{

	hcan3.Instance = CAN3;
	hcan3.pTxMsg = &Can3TxMsg;
	hcan3.pRxMsg = &Can3RxMsg;
	hcan3.Init.Prescaler = 20;
	hcan3.Init.Mode = CAN_MODE_NORMAL;
	hcan3.Init.SJW = CAN_SJW_1TQ;
	hcan3.Init.BS1 = CAN_BS1_16TQ;
	hcan3.Init.BS2 = CAN_BS2_3TQ;
	hcan3.Init.TTCM = DISABLE;
	hcan3.Init.ABOM = DISABLE;
	hcan3.Init.AWUM = DISABLE;
	hcan3.Init.NART = DISABLE;
	hcan3.Init.RFLM = DISABLE;
	hcan3.Init.TXFP = DISABLE;
	if (HAL_CAN_Init(&hcan3) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

}
