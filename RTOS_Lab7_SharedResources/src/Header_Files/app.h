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
#include "fifo_queue.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************
//typedef enum {
//
//	pushButton_0_pressed,
//	pushButton_0_not_pressed,
//	pushButton_1_pressed,
//	pushButton_1_not_pressed,
//	noButton,
//	BothButtons,
//	touchSliderPositionLeft,
//	touchSliderPositionRight,
//	noSlider
//
//}Message_Queue_Data;

typedef enum {

	PB0_Pressed,
	PB0_Not_Pressed,
	PB1_Pressed,
	PB1_Not_Pressed,
	No_Buttons_Pressed,
	Both_Buttons_Pressed

}e_PB_States;

struct Vehicle_Speed {

	int current_speed;
	int increment_count;
	int decrement_count;

};

typedef enum {

	Vehicle_Left,
	Vehicle_Hard_Left,
	Vehicle_Right,
	Vehicle_Hard_Right,
	Vehicle_Neutral

}e_Vehicle_Directions;

struct Vehicle_Direction {

	int current_direction;
	int current_direction_time_held;
	int left_turns_number;
	int right_turns_number;

};

typedef enum {

	Vehicle_Direction_Flag = 0x1,
	Vehicle_Speed_Flag = 0x2,
	Vehicle_None_Flag = 0x4

}e_Vehicle_Monitor_Flags;

typedef enum {

	LED0_Only_On = 0x1,
	LED1_Only_On = 0x2,
	Both_LED_On = 0x4,
	Both_LED_Off = 0x8

}e_LED_Output_Flags;


//***********************************************************************************
// function prototypes
//***********************************************************************************
void app_peripheral_setup(void);
void samplePushButton_0(void);
void samplePushButton_1(void);
void sampleSliderPosition(int* q_val);

#endif
