#ifndef APP_HG
#define APP_HG

//***********************************************************************************
// Include files
//***********************************************************************************
#include "em_cmu.h"
#include "cmu.h"
#include "gpio.h"
#include "capsense.h"
#include "main.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************
typedef enum {

	pushButton_0_pressed,
	pushButton_0_not_pressed,
	pushButton_1_pressed,
	pushButton_1_not_pressed,
	noButton,
	BothButtons,
	touchSliderPositionLeft,
	touchSliderPositionRight,
	noSlider

}Message_Queue_Data;

typedef enum {

	PB0_Pressed = 0x1,
	PB0_Not_Pressed = 0x2,
	PB1_Pressed = 0x4,
	PB1_Not_Pressed = 0x8,
	No_Buttons_Pressed = 0x10,
	Both_Buttons_Pressed = 0x20

}e_PB_Flags;


//***********************************************************************************
// function prototypes
//***********************************************************************************
void app_peripheral_setup(void);
void samplePushButton_0(void);
void samplePushButton_1(void);
void sampleSliderPosition(int* q_val);

#endif
