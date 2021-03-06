/*
 * can_logic.c
 *
 *  Created on: Aug 31, 2017
 *      Author: MJC
 */
#include "can_logic.h"
#include "..\base\can_hw.h"
#include "state.h"
HAL_StatusTypeDef Can1StaFlt = HAL_ERROR;
HAL_StatusTypeDef Can2StaFlt = HAL_ERROR;
HAL_StatusTypeDef Can3StaFlt = HAL_ERROR;



HAL_StatusTypeDef Can2StaRx = HAL_ERROR;
HAL_StatusTypeDef Can2StaTx = HAL_ERROR;

CAN_FilterConfTypeDef Can1Filter;
CAN_FilterConfTypeDef Can2Filter;
CAN_FilterConfTypeDef Can3Filter;
///Can1
static osThreadId can1_start_handle;

///CAN2
static osThreadId can2_start_handle;

///CAN3
static osThreadId can3_start_handle;

static osMessageQDef(can1_queue, 16, uint32_t);

static osMessageQDef(can2_queue, 16, uint32_t);

static osMessageQDef(can3_queue, 16, uint32_t);

#define CAN3_ID				0x33333333
#define	CAN3_CONFIG_HEAD1	0x5010
#define CAN3_CONFIG_HEAD2	0x22

//Can1
static void prvCan1Task( void const * argument )
{
	//(void)argument;
	MX_CAN1_Init();
	Can1StaFlt = CanFilter_Init(&Can1Filter, &hcan1);
	CAN_Init_App(&hcan1, (uint32_t)argument);
	HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);

	for(;;)
	{
		//uint32_t can_tx = 0xABCDEF54;
		//FillTxMsg(&hcan1, (uint8_t*)&can_tx, 4);
		osEvent evt = osMessageGet(can1_queue_handle, osWaitForever);
		if (evt.status == osEventMessage) {
			AnysCanId* id = (AnysCanId*)evt.value.signals;
			switch(id->func_id){
			case 0:
				if (id->module_id==0){
					if(id->module_type!=0
							|| vStateGet()->systate_curr != sCCMWaitConfig
							|| vStateGet()->ccm_is_configed){
						//vStateSet(sError);
					}else{
						CCMConfigeration(id->coil_id, (const char*) &hcan1.pRxMsg->Data, hcan1.pRxMsg->DLC);
					}

				}
				//CCMConfigeration(id->coil_id, (const char*) &hcan1.pRxMsg->Data, hcan1.pRxMsg->DLC);

			}
			//FillTxMsg(&hcan1, (uint8_t*)&evt.value.signals,4);
		}
		//osDelay(1);
	}
}
void osCan1CreateTask( osPriority priority , uint32_t can_id)
{
	if (can1_start_handle==NULL && can1_queue_handle==NULL){
		osThreadDef(Can1Task, prvCan1Task, priority, 0, configMINIMAL_STACK_SIZE);
		can1_start_handle = osThreadCreate(osThread(Can1Task), (uint32_t*) can_id);
		can1_queue_handle = osMessageCreate(osMessageQ(can1_queue), NULL);
	}
}

//Can2
static void prvCan2Task( void const * argument )
{
	(void)argument;
	MX_CAN2_Init();
	Can2StaFlt = CanFilter_Init(&Can2Filter, &hcan2);
	CAN_Init_App(&hcan2, 0x2222222);
	HAL_CAN_Receive_IT(&hcan2, CAN_FIFO0);
	for(;;)
	{
		osEvent evt = osMessageGet(can2_queue_handle, osWaitForever);
		if (evt.status == osEventMessage) {

		}
	}

}

void osCan2CreateTask( osPriority priority , uint32_t can_id)
{
	osThreadDef(Can2Task, prvCan2Task, priority, 0, configMINIMAL_STACK_SIZE);
	can2_start_handle = osThreadCreate(osThread(Can2Task), &can_id);
	can2_queue_handle = osMessageCreate(osMessageQ(can2_queue), NULL);
}

//Can3
static void prvCan3Task( void const * argument )
{
	(void)argument;
	MX_CAN3_Init();
	Can3StaFlt = CanFilter_Init(&Can3Filter, &hcan3);
	CAN_Init_App(&hcan3, 0x3333333);
	HAL_CAN_Receive_IT(&hcan3, CAN_FIFO0);
	for(;;)
	{
		osEvent evt = osMessageGet(can3_queue_handle, osWaitForever);
		if (evt.status == osEventMessage) {
			if (vStateGet()->systate_curr==sCan3WaitConfig){
				if ((hcan3.pRxMsg->Data[0]<<8)
						+hcan3.pRxMsg->Data[1]==CAN3_CONFIG_HEAD1){

					//osCan1CreateTask(osPriorityHigh, *(uint32_t*)&hcan3.pRxMsg->Data[1]);
					//osCan1CreateTask(CAN1_PROI, 0x12345678);
					vStateSet(sCCMWaitConfig);
				}
			}
		}
	}
}

void osCan3CreateTask( osPriority priority )
{
	osThreadDef(Can3Task, prvCan3Task, priority, 0, configMINIMAL_STACK_SIZE);
	can3_start_handle = osThreadCreate(osThread(Can3Task), NULL);
	can3_queue_handle = osMessageCreate(osMessageQ(can3_queue), NULL);
}

