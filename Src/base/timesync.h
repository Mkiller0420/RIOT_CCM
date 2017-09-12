/// @file
/// @author Timo Kröger
///
/// Implements time sync functionality with time span counter and GPIOs for
/// event notifications.

#ifndef TIMESYNC_H_
#define TIMESYNC_H_
//#define TIME_SYNC_TESTING
#include <stdbool.h>
#include <stdint.h>
#include "sysmon.h"
//#include "config.h"

// Exported constants ----------------------------------------------------------

// GPIO hardware definitions for time sync output.
#define TIMESYNC_OUT_PIN          GPIO_PIN_14
#define TIMESYNC_OUT_PORT         GPIOD
#define TIMESYNC_OUT_CLK_ENABLE() __GPIOD_CLK_ENABLE()

// GPIO hardware definitions for time sync input.
#define TIMESYNC_IN_PIN           GPIO_PIN_13
#define TIMESYNC_IN_PORT          GPIOD
#define TIMESYNC_IN_CLK_ENABLE()  __GPIOD_CLK_ENABLE()
#define TIMESYNC_IN_IRQ           EXTI15_10_IRQn
#define TIMESYNC_IN_IRQ_HANDLER   EXTI15_10_IRQHandler
// Timer hardware definitions.
#define TIMESYNC_TIMER              TIM13
#define TIMESYNC_TIMER_CLK_ENABLE() __TIM13_CLK_ENABLE()
#define TIMESYNC_TIMER_IRQ          TIM8_UP_TIM13_IRQn
#define TIMESYNC_TIMER_IRQ_HANDLER  TIM8_UP_TIM13_IRQHandler
#define TIMESYNC_TIMER_CLK_FREQ     100000000ul // AHB1 clk frequency in Hz
#define ISR_PRIO_TIMESYNC           5U
// Timer capture compare channel to reset the time sync line after some ticks.
#define TIMESYNC_TIMER_CHANNEL      TIM_CHANNEL_1

/// Frequency the time span counter is incremented with in Hz.
#define TIMESYNC_TIMER_CNT_FREQ     10000ul

/// Length of the event pulse on the time sync line (= 1ms).
#define TIMESYNC_TIMER_PULSE_WIDTH  (TIMESYNC_TIMER_CNT_FREQ / 1000)

/// Maximum time span count (= 3s). ATTENTION: 16bit value do not overflow.
#define TIMESYNC_TIMER_MAX_TIMESPAN (TIMESYNC_TIMER_CNT_FREQ * 3)

// Exported types --------------------------------------------------------------

/// Holds all information to determine when an event happened.
typedef struct {
	uint32_t seq_num;    ///< Number of sequence the event occurred in.
	uint32_t time_span;  ///< Number of ticks since the first event.
	                     ///< 0 if this is the first event in a sequence.
} TimeSyncEventInformation;

// Exported variables ----------------------------------------------------------
// Exported functions ----------------------------------------------------------

void TimeSyncInit(void);
void TimeSyncFireEvent(TimeSyncEventInformation *ei);
void TimeSyncSetNextSeq(uint32_t seq, bool reset);
void TimeSyncResetTimer(void);
void TimeSyncResetLine(void);
uint32_t TimeSyncTimeSpan(void);
#ifdef TIME_SYNC_TESTING
bool TimeSyncTest(void);
#endif

void TIMESYNC_IN_IRQ_HANDLER(void);
void TIMESYNC_TIMER_IRQ_HANDLER(void);

#endif  // TIMESYNC_H_
