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


	// Only PB0 is pressed
	if(!GPIO_PinInGet(BUTTON_0_port, BUTTON_0_pin) && GPIO_PinInGet(BUTTON_1_port, BUTTON_1_pin)) {

		push(PB0_Pressed);

	}

	// Both Buttons are pressed
	else if(!GPIO_PinInGet(BUTTON_0_port, BUTTON_0_pin) && !GPIO_PinInGet(BUTTON_1_port, BUTTON_1_pin)) {

		push(Both_Buttons_Pressed);

	}

	else {

		push(PB0_Not_Pressed);

	}
}

/***************************************************************************//**
 * @brief
 * 	Indicates if PB1 is pressed or not
 *
 ******************************************************************************/
void samplePushButton_1() {


	// Only PB1 is pressed
	if(!GPIO_PinInGet(BUTTON_1_port, BUTTON_1_pin) && GPIO_PinInGet(BUTTON_0_port, BUTTON_0_pin)) {

		push(PB1_Pressed);

	}

	// Both Buttons are pressed
	else if(!GPIO_PinInGet(BUTTON_0_port, BUTTON_0_pin) && !GPIO_PinInGet(BUTTON_1_port, BUTTON_1_pin)) {

		push(Both_Buttons_Pressed);

	}

	else {

		push(PB1_Not_Pressed);

	}


}

/***************************************************************************//**
 * @brief
 * 	Indicates where the slider position is
 *
 ******************************************************************************/
void sampleSliderPosition(int* q_val) {

	CAPSENSE_Sense();


//	// If touch slider is only in right position
//	if((CAPSENSE_getPressed(BUTTON1_CHANNEL) | CAPSENSE_getPressed(2)) && !(CAPSENSE_getPressed(BUTTON0_CHANNEL) | CAPSENSE_getPressed(1))) {
//
//		*q_val = touchSliderPositionRight;
//
//
//	}
//
//	// If touch slider is only in left position
//	else if((CAPSENSE_getPressed(BUTTON0_CHANNEL) | CAPSENSE_getPressed(1)) && !(CAPSENSE_getPressed(BUTTON1_CHANNEL) | CAPSENSE_getPressed(2))) {
//
//		*q_val = touchSliderPositionLeft;
//
//	}
//
//	else {
//
//		*q_val = noSlider;
//
//	}

	// If touch slider is in far right position
	if(CAPSENSE_getPressed(BUTTON1_CHANNEL)) {

		*q_val = Vehicle_Hard_Right;


	}

	// If touch slider is in right position
	else if(CAPSENSE_getPressed(2)) {

		*q_val = Vehicle_Right;
	}

	// If touch slider is in far left position
	else if(CAPSENSE_getPressed(BUTTON0_CHANNEL)) {

		*q_val = Vehicle_Hard_Left;

	}

	// If touch slider is in left position
	else if(CAPSENSE_getPressed(1)) {

		*q_val = Vehicle_Left;
	}

	else {

		*q_val = Vehicle_Neutral;

	}

}

