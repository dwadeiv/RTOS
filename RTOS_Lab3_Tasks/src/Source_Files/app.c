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
static bool pushButton_0_pressed;
static bool pushButton_1_pressed;
static bool touchSliderPositionLeft;
static bool touchSliderPositionRight;

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
	if(!GPIO_PinInGet(BUTTON_0_port, BUTTON_0_pin)) {

		pushButton_0_pressed = true;

	}

	else {

		pushButton_0_pressed = false;

	}
}

/***************************************************************************//**
 * @brief
 * 	Indicates if PB1 is pressed or not
 *
 ******************************************************************************/
void samplePushButton_1() {

	// Only PB1 is pressed
	if(!GPIO_PinInGet(BUTTON_1_port, BUTTON_1_pin)) {

		pushButton_1_pressed = true;

	}

	else {

		pushButton_1_pressed = false;

	}


}

/***************************************************************************//**
 * @brief
 * 	Indicates where the slider position is
 *
 ******************************************************************************/
void sampleSliderPosition() {

	CAPSENSE_Sense();

	// If touch slider is only in right position
	if((CAPSENSE_getPressed(BUTTON1_CHANNEL) | CAPSENSE_getPressed(2)) && !(CAPSENSE_getPressed(BUTTON0_CHANNEL) | CAPSENSE_getPressed(1))) {

		touchSliderPositionRight = true;
		touchSliderPositionLeft = false;

	}

	// If touch slider is only in left position
	else if((CAPSENSE_getPressed(BUTTON0_CHANNEL) | CAPSENSE_getPressed(1)) && !(CAPSENSE_getPressed(BUTTON1_CHANNEL) | CAPSENSE_getPressed(2))) {

		touchSliderPositionLeft = true;
		touchSliderPositionRight = false;

	}

	else {

		touchSliderPositionRight = false;
		touchSliderPositionLeft = false;

	}

}

/***************************************************************************//**
 * @brief
 * 	Adjusts the LEDs based on slider and pushbutton values
 *
 ******************************************************************************/
void adjustLED() {

	// Both LEDs are on
	if ((touchSliderPositionRight & pushButton_0_pressed) | (touchSliderPositionLeft & pushButton_1_pressed)) {

		GPIO_PinOutSet(LED1_port, LED1_pin);
		GPIO_PinOutSet(LED0_port, LED0_pin);

	}

	// LED0 is on LED1 is off
	else if((pushButton_0_pressed & !pushButton_1_pressed) | (touchSliderPositionLeft & pushButton_0_pressed & pushButton_1_pressed) | (touchSliderPositionLeft & pushButton_0_pressed) | touchSliderPositionLeft) {

		GPIO_PinOutSet(LED0_port, LED0_pin);
		GPIO_PinOutClear(LED1_port, LED1_pin);

	}

	// LED1 is on LED0 is off
	else if((pushButton_1_pressed & !pushButton_0_pressed) | (touchSliderPositionRight & pushButton_1_pressed & pushButton_0_pressed) | (touchSliderPositionRight & pushButton_1_pressed) | touchSliderPositionRight) {

		GPIO_PinOutSet(LED1_port, LED1_pin);
		GPIO_PinOutClear(LED0_port, LED0_pin);

	}

	// No LEDs are on
	else {

		GPIO_PinOutClear(LED0_port, LED0_pin);
		GPIO_PinOutClear(LED1_port, LED1_pin);

	}

}
