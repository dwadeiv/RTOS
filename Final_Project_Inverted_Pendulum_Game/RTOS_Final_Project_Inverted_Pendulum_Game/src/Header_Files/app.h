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

struct Configuration_Data{

	double gravity;
	double ball_mass;
	double post_length;
	int graphics_max_x;
	int graphics_min_x;
	int graphics_max_y;
	int graphics_min_y;
	int project_variant;
	int max_force;
	int max_gain;

};

typedef enum {

	Inverted_Pendulum,
	Inverted_Pendulum_with_Kicker,
	Pogo_Stick,
	Pogo_Stick_with_Flywheel


}e_Project_Variants;

typedef enum {

	PB0_Pressed,
	PB0_Not_Pressed,
	PB1_Pressed,
	PB1_Not_Pressed,
	No_Buttons_Pressed,
	Both_Buttons_Pressed

}e_PB_States;

typedef enum {

	No_Button_Event,
	Button0_Event,
	Button1_Event

}e_PB_Events;

typedef enum {

	Left = -20,
	Hard_Left = -40,
	Right = 20,
	Hard_Right = 40,
	Neutral= 0

}e_Force_States;

struct Position_Data {

	double current_ball_x_position;
	double current_ball_y_position;
	double current_cart_velocity;
	double current_theta;
	double current_theta_first_derivative;
	double current_theta_second_derivative;
	double current_cart_position;

};

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
