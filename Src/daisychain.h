/// @file
/// @author Timo Kröger
///
/// Defines the MSM Daisy Chain mechanism. This includes the GPIO initialization
/// for the Reset In and Reset Next pins and a mechanism to hold the module in
/// reset state.

#ifndef DAISYCHAIN_H_
#define DAISYCHAIN_H_
#define CCM
#include <stdbool.h>

/* Exported constants --------------------------------------------------------*/

// GPIO configuration for the daisy chain pins.
#define DAISYCHAIN_GPIO               GPIOA
#define DAISYCHAIN_GPIO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()
#ifndef CCM
#define DAISYCHAIN_GPIO_PIN_RESET_IN  GPIO_PIN_9   ///< GPIO pin for Reset In.
#endif
#define DAISYCHAIN_GPIO_PIN_RESET_OUT GPIO_PIN_10  ///< GPIO pin for Reset Out.


/* Exported types ------------------------------------------------------------*/
/* Exported variables ------------------------------------------------------- */
/* Exported functions ------------------------------------------------------- */

void DaisychainInit(void);

void DaisychainSet(bool state);
#ifndef CCM
void DaisychainCheckReset(void);
void DaisychainWait(void);
#endif
#endif  // DAISYCHAIN_H_
