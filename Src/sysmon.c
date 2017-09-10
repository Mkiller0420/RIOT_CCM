////////////////////////////////////////////////////////////////////////////////
/**
 * MSM Module Firmware
 * Copyright(c) 2015 ABB AG, PPHV-GES, Germany
 *
 * @file	sysMoni.c
 *
 * @author	Martin Legleiter
 *
 * @brief	Defines the monitoring interface for the system and the event LED.
 */
////////////////////////////////////////////////////////////////////////////////

/* Includes ------------------------------------------------------------------*/

#include "sysmon.h"

#include <assert.h>
#include <stdbool.h>

#include "FreeRTOS.h"


#define LEDNUM		5
/// Defines the length of one monitor period in ms.
#define monitorPERIOD 100

/// Defines how long a LED will stay off for a flicker event
///
/// ( monitorMAX_FLICKER_PERIODS_SYSTEM * Timer Period = Stay off time ).
#define monitorMAX_FLICKER_PERIODS 2

/// Defines how long a LED will stay off when blinking (= 500ms).
#define monitorBLINK_PERIOD_OFF 5

/// Defines how long a LED will stay on when blinking (= 500ms).
#define monitorBLINK_PERIOD_ON  5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//static TaskHandle_t sysmoni_task_handle = NULL;
static osThreadId sysmoni_task_handle;
static osMessageQDef(led_queue, 16, uint8_t);
static osMessageQId led_queue_handle;
/* Private function prototypes -----------------------------------------------*/

static void prvMonitorTask( void const * argument );
//static void prvMonitorNotifyTask( LEDType_t type, LEDColor_t color, LEDFreq_t freq );
static LEDConfig led[5]={
		{LED_Status_Port,	LED_Status_Pin,	eStatus,	eOff, 	0, 0},
		{LED_Alarm_Port,	LED_Alarm_Pin,	eAlarm,		eOff, 	0 ,0},
		{LED_Wait_Port,		LED_Wait_Pin,	eWait, 		eOff,   0, 0},
		{LED_Ready_Port,	LED_Ready_Pin, 	eRedy, 		eOff, 	0, 0},
		{LED_Rsvd_Port,		LED_Rsvd_Pin, 	eRsvd, 		eOff, 	0, 0},
};
/* Private functions ---------------------------------------------------------*/

static void prvMonitorTask( void const * argument )
{
	(void)argument;

	/*xLEDDriverInit();

	LEDColor_t xStatusLedColor = eOff;
	LEDFreq_t xStatusLedFreq = eSteady;
	uint32_t xStatusLedFlickerTicks = 0;  // Turn of LED while not zero.
	                                      // Decrement with each tick.

	LEDColor_t xSignalLedColor = eOff;
	LEDFreq_t xSignalLedFreq = eSteady;
	uint32_t xSignalLedFlickerTicks = 0;  // Turn of LED while not zero.
	                                      // Decrement with each tick.*/
	//MonitorInit();
	uint32_t period_tick = 0;

	for (;;) {
		//uint32_t num;
		/*BaseType_t ret = xTaskNotifyWait(0,               // Clear no flags on entry.
				0xFFFFFFFF,      // Clear all flags on exit.
				&num, // Store notification value.
				pdMS_TO_TICKS(monitorPERIOD));*/
		//osEvent evt = osSignalWait(0, monitorPERIOD);
		osEvent evt = osMessageGet(led_queue_handle, monitorPERIOD);
		// Received a notification: Update color or frequency.
		if (evt.status == osEventMessage) {
			/*if (led_config.fields.type == eStatus) {
				// New value received. Update variables.
				if (led_config.fields.freq == eFlicker) {
					xStatusLedFlickerTicks = monitorMAX_FLICKER_PERIODS;
				} else {
					xStatusLedFreq = led_config.fields.freq;
					xStatusLedColor = led_config.fields.color;
				}
			} else if (led_config.fields.type == eSignal) {
				// New value received. Update variables.
				if (led_config.fields.freq == eFlicker) {
					xSignalLedFlickerTicks = monitorMAX_FLICKER_PERIODS;
				} else {
					xSignalLedFreq = led_config.fields.freq;
					xSignalLedColor = led_config.fields.color;
				}
			} else {
				assert(false);
			}*/
			uint8_t led_num=(uint8_t)evt.value.signals;
			assert(0<=led_num&&led_num<=LEDNUM);
			if (led_num<LEDNUM){
				if (led[led_num].state==eOn){
					HAL_GPIO_WritePin(led[led_num].LED_Port, led[led_num].LED_Pin, GPIO_PIN_SET);
				}else if(led[led_num].state==eOff){
					HAL_GPIO_WritePin(led[led_num].LED_Port, led[led_num].LED_Pin, GPIO_PIN_RESET);
				}
			}else if(led_num==LEDNUM){
				for (uint8_t i=0;i<LEDNUM-1;i++){
					if (led[i].state==eOn){
						HAL_GPIO_WritePin(led[i].LED_Port, led[i].LED_Pin, GPIO_PIN_SET);
					}else if(led[i].state==eOff){
						HAL_GPIO_WritePin(led[i].LED_Port, led[i].LED_Pin, GPIO_PIN_RESET);
					}
				}
			}
		}

		// Enable LED be default...
		/*LEDColor_t status_color = xStatusLedColor;

		// ... but turn it of if flickering ...
		if (xStatusLedFlickerTicks > 0) {
			status_color = eOff;
			--xStatusLedFlickerTicks;
		// ... or blinking.
		} else if ((xStatusLedFreq == eBlink) &&
		           (period_tick < monitorBLINK_PERIOD_OFF)) {
			status_color = eOff;
		}

		// Enable LED be default...
		LEDColor_t signal_color = xSignalLedColor;

		// ... but turn it of if flickering ...
		if (xSignalLedFlickerTicks > 0) {
			signal_color = eOff;
			--xSignalLedFlickerTicks;
		// ... or blinking.
		} else if ((xSignalLedFreq == eBlink) &&
		           (period_tick < monitorBLINK_PERIOD_OFF)) {
			signal_color = eOff;
		}

		xLEDSet(eStatus, status_color);
		xLEDSet(eSignal, signal_color);*/
		for (uint8_t i=0;i<LEDNUM;i++){
			if (led[i].state==eOn){
				HAL_GPIO_WritePin(led[i].LED_Port, led[i].LED_Pin, GPIO_PIN_SET);
			}else if(led[i].state==eOff){
				HAL_GPIO_WritePin(led[i].LED_Port, led[i].LED_Pin, GPIO_PIN_RESET);
			}else if(led[i].state==eBlink){
				if (period_tick < monitorBLINK_PERIOD_OFF){
					HAL_GPIO_WritePin(led[i].LED_Port, led[i].LED_Pin, GPIO_PIN_SET);
				}else{
					HAL_GPIO_WritePin(led[i].LED_Port, led[i].LED_Pin, GPIO_PIN_RESET);
				}

			}
		}
		if (period_tick < (monitorBLINK_PERIOD_OFF + monitorBLINK_PERIOD_ON)) {
			period_tick++;
		} else {
			period_tick = 0;
		}

	}
}
/* Exported functions ------------------------------------------------------- */

/// Create the system monitor RTOS task.
///
/// This includes the LED driver, the creation of the System Monitor Task and
/// the System Monitor timer (HAL).
void osMonitorCreateTask( osPriority priority )
{
	osThreadDef(MonitorTask, prvMonitorTask, priority, 0, configMINIMAL_STACK_SIZE);
	sysmoni_task_handle = osThreadCreate(osThread(MonitorTask), NULL);
	led_queue_handle = osMessageCreate(osMessageQ(led_queue), NULL);
	/*xTaskCreate(&prvMonitorTask,            // Task function
			"Monitor",                  // Task name
			configMINIMAL_STACK_SIZE,   // Stack size in words
			NULL,                       // No user data
			priority,                   // Priority
			&sysmoni_task_handle);      // Task handle*/
}
static void prvMonitorNotifyTask( LEDNum num, LEDState state)
{
	assert(0<=num&&num<=LEDNUM);
	assert(1<=state&&state<=3);
	if (num<LEDNUM){
		led[num].state=state;
	}else if(num==LEDNUM){
		for (uint8_t i=0;i<LEDNUM;i++){
			led[i].state=state;
		}
	}
	//xTaskNotify(sysmoni_task_handle, num, eSetValueWithOverwrite);
	//uint32_t signals = osSignalSet (sysmoni_task_handle, num);
	osMessagePut(led_queue_handle, (uint8_t)num, 0);
}

/// Sets the Status LED to a new state.
///
/// @param color Color of the LED
/// @param freq Blinking frequency
void vMonitorSetLED( LEDNum num, LEDState state )
{
	prvMonitorNotifyTask( num, state);
}

LEDConfig* vMonitorGetState(LEDNum num)
{
	assert(0<=num&&num<LEDNUM);
	return &led[num];
}


void MonitorInit(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	HAL_GPIO_WritePin(GPIOD, LED_Status_Pin|LED_Alarm_Pin|
			LED_Wait_Pin|LED_Ready_Pin|LED_Rsvd_Pin, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LED_Status_Pin|LED_Alarm_Pin|
			LED_Wait_Pin|LED_Ready_Pin|LED_Rsvd_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
