/// @file
/// @author Timo Kröger
///
/// Defines the MSM Daisy Chain mechanism. This includes the GPIO initialization
/// for the Reset In and Reset Next pins and a mechanism to hold the module in
/// reset state.

#include "daisychain.h"

#include "stm32f4xx_hal.h"

/* Private typedefs ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables ------------------------------------------------------- */
/* Private functions ---------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/// Initializes daisy chain GPIOs (Reset In and Reset Out Pins).
void DaisychainInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	DAISYCHAIN_GPIO_CLK_ENABLE();
#ifndef CCM
	// Reset In
	GPIO_InitStruct.Pin = DAISYCHAIN_GPIO_PIN_RESET_IN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(DAISYCHAIN_GPIO, &GPIO_InitStruct);
#endif
	// Reset Out
	HAL_GPIO_WritePin(DAISYCHAIN_GPIO, DAISYCHAIN_GPIO_PIN_RESET_OUT, 0);
	GPIO_InitStruct.Pin = DAISYCHAIN_GPIO_PIN_RESET_OUT;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(DAISYCHAIN_GPIO, &GPIO_InitStruct);
}

/// Waits for a pulse on the Reset In pin.
#ifndef CCM
void DaisychainWait(void)
{
	// Wait for bus to be pulled low (pin high).
	while (HAL_GPIO_ReadPin(DAISYCHAIN_GPIO, DAISYCHAIN_GPIO_PIN_RESET_IN) == GPIO_PIN_RESET);

	// Wait for bus to be pulled high (pin low).
	while (HAL_GPIO_ReadPin(DAISYCHAIN_GPIO, DAISYCHAIN_GPIO_PIN_RESET_IN) == GPIO_PIN_SET);
}

/// Checks if the reset line has changed. May reset the controller.
void DaisychainCheckReset(void)
{
	if (HAL_GPIO_ReadPin(DAISYCHAIN_GPIO, DAISYCHAIN_GPIO_PIN_RESET_IN)) {
		// Wait for bus to be pulled high (pin low).
		while (HAL_GPIO_ReadPin(DAISYCHAIN_GPIO, DAISYCHAIN_GPIO_PIN_RESET_IN) == GPIO_PIN_SET);
		NVIC_SystemReset();
	}
}
#endif
/// Changes the Reset Out pin state.
///
/// @param state true to set the active state (low level on the bus)
///              false to set the passive state (high level on the bus)
void DaisychainSet(bool state)
{
	HAL_GPIO_WritePin(DAISYCHAIN_GPIO, DAISYCHAIN_GPIO_PIN_RESET_OUT, state);
}


