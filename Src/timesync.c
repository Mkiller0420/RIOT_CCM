/// @file
/// @author Timo Kröger
///
/// Implements time sync functionality with timespan counter and GPIOs for
/// event notifications.

#include "timesync.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "can_hw.h"
#include "stm32f4xx_hal.h"

// Private typedefs ------------------------------------------------------------
// Private defines -------------------------------------------------------------
// Private macros --------------------------------------------------------------
// Private function prototypes -------------------------------------------------

static void TimeSyncStartTimer(void);
static bool TimeSyncSeqActive(void);
static void TimeSyncTriggerLine(void);
// Private variables -----------------------------------------------------------

static TIM_HandleTypeDef timer;
static uint32_t seq_num;
static uint32_t next_seq_num;

// Exported variables ----------------------------------------------------------
// Private functions -----------------------------------------------------------

/// Starts the time span counter.
///
/// Updates the sequence number with the next one.
static void TimeSyncStartTimer(void)
{
	HAL_TIM_PWM_Start_IT(&timer, TIMESYNC_TIMER_CHANNEL);
	HAL_TIM_Base_Start_IT(&timer);

	seq_num = next_seq_num;
}

/// Checks if there is an active sequence (timer running).
///
/// @returns True when the timer is running, false otherwise.
static bool TimeSyncSeqActive(void)
{
	return (timer.Instance->CR1 & TIM_CR1_CEN);
}

/// Trigger the time sync line.
///
/// Pulls the time sync line low for a short period.
static void TimeSyncTriggerLine(void)
{
	HAL_GPIO_WritePin(TIMESYNC_OUT_PORT, TIMESYNC_OUT_PIN, GPIO_PIN_RESET);

#ifdef TIME_SYNC_TESTING
	// Call interrupt handler directly.
	TIMESYNC_IN_IRQ_HANDLER();
#endif
}

/// Gets the current time span value.
///
/// @returns The time span in ticks. One tick is equals
///          (1/TIMESYNC_TIMER_CNT_FREQ) seconds.
uint32_t TimeSyncTimeSpan(void)
{
	return timer.Instance->CNT;
}

// Exported functions ----------------------------------------------------------

/// Initializes the time sync hardware.
///
/// Sets up the time span counter, the GPIOs and interrupts.
void TimeSyncInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_OC_InitTypeDef sConfigOC;

	// Configure sync out pin.
	TIMESYNC_OUT_CLK_ENABLE();
	TimeSyncResetLine();  // Default (recessive) state is high level.
	GPIO_InitStruct.Pin = TIMESYNC_OUT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(TIMESYNC_OUT_PORT, &GPIO_InitStruct);

	// Configure sync in pin.
	TIMESYNC_IN_CLK_ENABLE();
	GPIO_InitStruct.Pin = TIMESYNC_IN_PIN;
	// Sync IN is inverted triggering on rising edge here actually means trigger
	// on falling edge on the bus.
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(TIMESYNC_IN_PORT, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(TIMESYNC_IN_IRQ, ISR_PRIO_TIMESYNC, 0);
	HAL_NVIC_EnableIRQ(TIMESYNC_IN_IRQ);
	//HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
	//HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	// Configure timer: Stop it (ISR) when it reaches it max value.
	TIMESYNC_TIMER_CLK_ENABLE();
	timer.Instance = TIMESYNC_TIMER;
	timer.Init.Prescaler = (TIMESYNC_TIMER_CLK_FREQ/TIMESYNC_TIMER_CNT_FREQ)-1;
	timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	timer.Init.Period = TIMESYNC_TIMER_MAX_TIMESPAN;
	timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&timer);

	// Use a capture compare interrupt to reset the line after a short time.
	// Choose PWM mode so that the OCxREF flag (which triggers the interrupt)
	// is reset when overwriting the counter with 0.
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = TIMESYNC_TIMER_PULSE_WIDTH;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&timer, &sConfigOC, TIMESYNC_TIMER_CHANNEL);

	// Configure the timer interrupt.
	HAL_NVIC_SetPriority(TIMESYNC_TIMER_IRQ, ISR_PRIO_TIMESYNC, 0);
	HAL_NVIC_EnableIRQ(TIMESYNC_TIMER_IRQ);
}

/// Notifies all other modules that an event happened and saves the sequence
/// number and time stamp.
///
/// @param[out] ei Pointer to a memory location that receives the event info.
void TimeSyncFireEvent(TimeSyncEventInformation *ei)
{
	if (TimeSyncSeqActive() == false) {
		// The timer is started automatically by the time sync GPIO line
		// interrupt.
		TimeSyncTriggerLine();

		// The sequence number is assigned as soon as the timer starts. If no timer
		// is running and we manually need to asign the next number here.
		ei->seq_num = next_seq_num;
	} else {
		ei->seq_num = seq_num;
	}

	ei->time_span = TimeSyncTimeSpan();
}

/// Sets the next sequence number and resets the counter if requested.
///
/// @param seq Next sequence number.
/// @param reset True if counter shall be reseted.
void TimeSyncSetNextSeq(uint32_t seq, bool reset)
{
	next_seq_num = seq;
	if (reset) {
		TimeSyncResetTimer();
	}
}

/// Resets the time sync line.
void TimeSyncResetLine(void)
{
	HAL_GPIO_WritePin(TIMESYNC_OUT_PORT, TIMESYNC_OUT_PIN, GPIO_PIN_SET);
}

/// Stops and resets the time sync counter.
void TimeSyncResetTimer(void)
{
	HAL_TIM_PWM_Stop_IT(&timer, TIMESYNC_TIMER_CHANNEL);
	HAL_TIM_Base_Stop_IT(&timer);
	timer.Instance->CNT = 0;  // Reset time span counter.

	// When receiving a manual reset oven CANopen there is the possibility that
	// the line is still active. By disabling the timer the line will not reset
	// itself anymore. Reset it manually here.
	TimeSyncResetLine();
}

#ifdef TIME_SYNC_TESTING
bool TimeSyncTest(void)
{
	static GPIO_PinState State;
	TimeSyncEventInformation ei;
	uint32_t can_tx = 0x12345678;
	// Reset counter on plug in and set new seq num;

	//HAL_GPIO_WritePin(LED_Status_Port, LED_Status_Pin, State);
	State = ~State;
	TimeSyncSetNextSeq(5, true);
	TimeSyncFireEvent(&ei);
	FillTxMsg(&hcan1, (uint8_t*)&can_tx, sizeof(can_tx));
	if (ei.seq_num != 5 || ei.time_span != 0) {
		HAL_GPIO_WritePin(LED_Status_Port, LED_Alarm_Pin, GPIO_PIN_SET);
		return false;
	}

	// Set temporary sequence number that is overwritten later.
	TimeSyncSetNextSeq(6, false);

	// Wait a short time.
	HAL_Delay(100);

	TimeSyncFireEvent(&ei);
	// 1% window for time_span counter as this is not an accurate way to test
	// the timing.
	// Sequence number didn't change because max time span not reached.
	//if (ei.seq_num != 5 || ei.time_span < 990 || ei.time_span > 1010) {
	if (ei.time_span < 990 || ei.time_span > 1010){
		HAL_GPIO_WritePin(LED_Status_Port, LED_Ready_Pin, GPIO_PIN_SET);
		return false;
	}

	// Set another sequence number for the next event after max time span.
	TimeSyncSetNextSeq(0xFFFFFFFF, false);

	// Wait for half of max time span.
	HAL_Delay(TIMESYNC_TIMER_MAX_TIMESPAN / TIMESYNC_TIMER_CNT_FREQ * 1000 / 2);

	TimeSyncFireEvent(&ei);
	// Sequence number still not changed.
	if (ei.seq_num != 5) {
		HAL_GPIO_WritePin(LED_Status_Port, LED_Wait_Pin, GPIO_PIN_SET);
		return false;
	}

	// Wait for after the max time span.
	HAL_Delay(TIMESYNC_TIMER_MAX_TIMESPAN / TIMESYNC_TIMER_CNT_FREQ * 1000 / 2);

	TimeSyncFireEvent(&ei);
	// Finally a new sequence number.
	if (ei.seq_num != 0xFFFFFFFF || ei.time_span != 0) {
		HAL_GPIO_WritePin(LED_Status_Port, LED_Rsvd_Pin, GPIO_PIN_SET);
		return false;
	}

	// Now set a new sequence number with a reset.
	TimeSyncSetNextSeq(2, true);

	TimeSyncFireEvent(&ei);
	// Sequence number must be used immediately.
	if (ei.seq_num != 2 || ei.time_span != 0) {
		return false;
	}

	return true;
}
#endif

/// SyncIN GPIO interrupt handler which starts the timer.
//void TIMESYNC_IN_IRQ_HANDLER(void)
/*{
	TimeSyncStartTimer();

	// Clear interrupt pending flag..
	HAL_GPIO_EXTI_IRQHandler(TIMESYNC_IN_PIN);
}*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin==TIMESYNC_IN_PIN){
		TimeSyncStartTimer();
		GPIO_PinState LD3status = HAL_GPIO_ReadPin(GPIOD, LED_Status_Pin);
		if (LD3status){
			vMonitorSetLED(eStatus, eOff);
		}
		else{
			vMonitorSetLED(eStatus, eOn);
		}
	}
}

/// Interrupt handler that resets the timer after TIMESYNC_TIMER_MAX_TIMESPAN
/// or resets the GPIO time sync pin to default.
void TIMESYNC_TIMER_IRQ_HANDLER(void)
{
	// Clear interrupt pending flag and trigger callback (hal.c).
	HAL_TIM_IRQHandler(&timer);
}
