/*
 * letimer.c
 *
 *  Created on: February 3, 2021
 *      Author: David Wade IV
 */


//***********************************************************************************
// Include files
//***********************************************************************************
#include "app.h"


//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function
//***********************************************************************************

/***************************************************************************//**
 * @brief
 * 	Initializes clocks and gpio
 *
 ******************************************************************************/
void app_peripheral_setup(void){
	cmu_open();
	gpio_open();

}

/***************************************************************************//**
 * @brief
 * 	Indicates if PB0 is pressed or not
 *
 ******************************************************************************/
void samplePushButton_0() {

	RTOS_ERR  err;

	// Only PB0 is pressed
	if(!GPIO_PinInGet(BUTTON_0_port, BUTTON_0_pin) && GPIO_PinInGet(BUTTON_1_port, BUTTON_1_pin)) {

		OSFlagPost(&ButtonInputEventGroup,
				PB0_Pressed,
				OS_OPT_POST_FLAG_SET,
				&err);

	}

	// Both Buttons are pressed
	else if(!GPIO_PinInGet(BUTTON_0_port, BUTTON_0_pin) && !GPIO_PinInGet(BUTTON_1_port, BUTTON_1_pin)) {

		OSFlagPost(&ButtonInputEventGroup,
				Both_Buttons_Pressed,
				OS_OPT_POST_FLAG_SET,
				&err);

	}

	else {

		OSFlagPost(&ButtonInputEventGroup,
				PB0_Not_Pressed,
				OS_OPT_POST_FLAG_SET,
				&err);

	}
}

/***************************************************************************//**
 * @brief
 * 	Indicates if PB1 is pressed or not
 *
 ******************************************************************************/
void samplePushButton_1() {

	RTOS_ERR  err;

	// Only PB1 is pressed
	if(!GPIO_PinInGet(BUTTON_1_port, BUTTON_1_pin) && GPIO_PinInGet(BUTTON_0_port, BUTTON_0_pin)) {

		OSFlagPost(&ButtonInputEventGroup,
				PB1_Pressed,
				OS_OPT_POST_FLAG_SET,
				&err);
	}

	// Both Buttons are pressed
	else if(!GPIO_PinInGet(BUTTON_0_port, BUTTON_0_pin) && !GPIO_PinInGet(BUTTON_1_port, BUTTON_1_pin)) {

		OSFlagPost(&ButtonInputEventGroup,
				Both_Buttons_Pressed,
				OS_OPT_POST_FLAG_SET,
				&err);

	}

	else {

		OSFlagPost(&ButtonInputEventGroup,
				PB1_Not_Pressed,
				OS_OPT_POST_FLAG_SET,
				&err);

	}


}

/***************************************************************************//**
 * @brief
 * 	Indicates where the slider position is
 *
 ******************************************************************************/
void sampleSliderPosition(int* q_val) {

	CAPSENSE_Sense();


	// If touch slider is only in right position
	if((CAPSENSE_getPressed(BUTTON1_CHANNEL) | CAPSENSE_getPressed(2)) && !(CAPSENSE_getPressed(BUTTON0_CHANNEL) | CAPSENSE_getPressed(1))) {

		*q_val = touchSliderPositionRight;


	}

	// If touch slider is only in left position
	else if((CAPSENSE_getPressed(BUTTON0_CHANNEL) | CAPSENSE_getPressed(1)) && !(CAPSENSE_getPressed(BUTTON1_CHANNEL) | CAPSENSE_getPressed(2))) {

		*q_val = touchSliderPositionLeft;

	}

	else {

		*q_val = noSlider;

	}

}

