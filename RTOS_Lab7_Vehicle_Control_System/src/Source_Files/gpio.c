//***********************************************************************************
// Include files
//***********************************************************************************
#include "gpio.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************


//***********************************************************************************
// functions
//***********************************************************************************

/***************************************************************************//**
 * @brief Configure the GPIO
 *****************************************************************************/
void gpio_open(void){

	// Set LED ports to be standard output drive with default off (cleared)
//	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);

//	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);

	GPIO_PinModeSet(BUTTON_0_port, BUTTON_0_pin, gpioModeInputPull, BUTTON_0_default);
	GPIO_IntConfig(BUTTON_0_port, BUTTON_0_pin, true, true, true);

	GPIO_PinModeSet(BUTTON_1_port, BUTTON_1_pin, gpioModeInputPull, BUTTON_1_default);
	GPIO_IntConfig(BUTTON_1_port, BUTTON_1_pin, true, true, true);

	// Enable interrupts for pushbuttons
	NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
	NVIC_EnableIRQ(GPIO_EVEN_IRQn);

	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);

}

/***************************************************************************//**
 * @brief Unified GPIO Interrupt handler (pushbuttons)
 *        PB0 Switches between analog and digital clock modes
 *        PB1 Increments the time by one minute
 *****************************************************************************/
void GPIO_Unified_IRQ(void)
{
  /* Get and clear all pending GPIO interrupts */
  uint32_t interruptMask = GPIO_IntGet();
  GPIO_IntClear(interruptMask);

  /* Act on interrupts */
  if (interruptMask & (1 << BUTTON_0_pin)) {
    /* PB0: Toggle clock mode (analog/digital) */
	  samplePushButton_0();
  }

  if (interruptMask & (1 << BUTTON_1_pin)) {
    /* Increase time by 1 second. */
	  samplePushButton_1();
  }

	RTOS_ERR  err;

	OSSemPost(&Button_Input_Semaphore, OS_OPT_POST_ALL, &err); /* post button input semaphore */


}

/***************************************************************************//**
 * @brief GPIO Interrupt handler for even pins
 ******************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  GPIO_Unified_IRQ();
}

/***************************************************************************//**
 * @brief GPIO Interrupt handler for odd pins
 ******************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  GPIO_Unified_IRQ();
}


