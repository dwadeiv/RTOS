#ifndef GPIO_HG
#define GPIO_HG

//***********************************************************************************
// Include files
//***********************************************************************************
#include "em_gpio.h"

//***********************************************************************************
// defined files
//***********************************************************************************

// LED 0 pin is
#define	LED0_port			gpioPortF
#define LED0_pin			04u
#define LED0_default		false 	// Default false (0) = off, true (1) = on
// LED 1 pin is
#define LED1_port			gpioPortF
#define LED1_pin			05u
#define LED1_default		false	// Default false (0) = off, true (1) = on
// PUSH BUTTON 0
#define BUTTON_0_port		gpioPortF
#define BUTTON_0_pin		06u
#define BUTTON_0_default	true
// PUSH BUTTON 1
#define BUTTON_1_port		gpioPortF
#define BUTTON_1_pin		07u
#define BUTTON_1_default	true

//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void gpio_open(void);



#endif
