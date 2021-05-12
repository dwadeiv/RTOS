/***************************************************************************//**
 * @file main.c
 * @brief This is the main for the Inverted Pendulum Game
 *******************************************************************************
 *
 *  Created on: April 8, 2021
 *      Author: David Wade IV
 *
 ******************************************************************************/

#include "main.h"

/*
*********************************************************************************************************
*********************************************************************************************************
*                                        LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*********************************************************************************************************
*/                                                       		/* Start Task Stack. */
static  CPU_STK  MainStartTaskStk[TASK_STK_SIZE];

																/* Start Task TCB. */
static  OS_TCB   MainStartTaskTCB;

static  CPU_STK  GainSetpointTaskStk[TASK_STK_SIZE];
                                                                /* Gain Setpoint Task TCB. */
static  OS_TCB   GainSetpointTaskTCB;

static  CPU_STK  LedPWMTaskStk[TASK_STK_SIZE];
                                                                /* LED0 PWM Task TCB. */
static  OS_TCB   LedPWMTaskTCB;

static  CPU_STK  SliderInputTaskStk[TASK_STK_SIZE];
                                                                /* Slider Input Task TCB. */
static  OS_TCB   SliderInputTaskTCB;

static  CPU_STK  PositionUpdateTaskStk[TASK_STK_SIZE];
                                                                /* Position Update Task TCB. */
static  OS_TCB   PositionUpdateTaskTCB;

static  CPU_STK  LcdDisplayTaskStk[TASK_STK_SIZE];
                                                                /* LCD Display Task TCB. */
static  OS_TCB   LcdDisplayTaskTCB;

static  CPU_STK  IdleTaskStk[TASK_STK_SIZE];
                                                                /* IDLE Task TCB. */
static  OS_TCB   IdleTaskTCB;




/*
*********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

static void MainStartTask (void  *p_arg);
static void GainSetpointTask(void *p_arg);
static void LedPWMTask(void *p_arg);
static void SliderInputTask(void *p_arg);
static void PositionUpdateTask(void *p_arg);
static void LcdDisplayTask(void *p_arg);
static void IdleTask(void *p_arg);

static void configurationInit();

static void AppGraphics_Init(GLIB_Context_t *pContext);
static void AppGraphics_DisplayScreen(GLIB_Context_t *pContext, struct Position_Data* current_position_data);


int  main (void)
{
    RTOS_ERR  err;


    BSP_SystemInit();                                           /* Initialize System. */
    CPU_Init();                                                 /* Initialize CPU. */

    OS_TRACE_INIT();
    OSInit(&err);                                               /* Initialize the Kernel. 	*/
                                                                /*   Check error code. 		*/
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);



    /* Call application program to open / initialize all required peripheral */
    app_peripheral_setup();


    OSTaskCreate(&MainStartTaskTCB,                          /* Create the Start Task.                               */
                 "Main Start Task",
                  MainStartTask,
                  DEF_NULL,
                  MAIN_START_TASK_PRIO,
                 &MainStartTaskStk[0],
                 (TASK_STK_SIZE / 10u),
                  TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);
                                                                /*   Check error code.                                  */
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);


    OSStart(&err);                                              /* Start the kernel.                                    */
                                                                /*   Check error code.                                  */
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);


    return (1);
}

/***************************************************************************//**
 * @brief
 * 	Start Task that creates ITC tools and Creates All the tasks
 *
 ******************************************************************************/
static void MainStartTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    Common_Init(&err);                                          /* Call common module initialization example.           */
    APP_RTOS_ASSERT_CRITICAL(err.Code == RTOS_ERR_NONE, ;);

    BSP_OS_Init();                                              /* Initialize the BSP. It is expected that the BSP ...  */
                                                                /* ... will register all the hardware controller to ... */
                                                                /* ... the platform manager at this moment.             */


																/* Create Position Data Message Queue */
	OSQCreate((OS_Q *)&Position_Data_Message_Queue,
			(CPU_CHAR *)"Position Data Message Queue",
			(OS_MSG_QTY)4,
			(RTOS_ERR *)&err);

																/* Create Ball Position Mutex */
	OSMutexCreate(&Ball_Position_Lock,
			(CPU_CHAR *)"Ball Position Mutex",
			&err);

    															/* Create Button Input Semaphore */
    OSSemCreate((OS_SEM *) &Button_Input_Semaphore,
    		(CPU_CHAR *)"Button Input Semaphore",
			0,
			(RTOS_ERR *)&err);

    															/* Initialize Button Event FIFO Structure */
    queueInit();

    															/* Configure the game */
    configurationInit();




    OSTaskCreate(&GainSetpointTaskTCB,                          /* Create the Gain Setpoint Task. */
                 "Gain Setpoint Task",
                  GainSetpointTask,
                  DEF_NULL,
                  GAIN_SETPOINT_TASK_PRIO,
                 &GainSetpointTaskStk[0],
                 (TASK_STK_SIZE / 10u),
                  TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);

    OSTaskCreate(&LedPWMTaskTCB,                          		/* Create the LED0 PWM Task. */
                 "LED0 PWM Task",
				 LedPWMTask,
				 DEF_NULL,
				 LED0_PWM_TASK_PRIO,
                 &LedPWMTaskStk[0],
				 (TASK_STK_SIZE / 10u),
				 TASK_STK_SIZE,
				 0u,
				 0u,
				 DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);

    OSTaskCreate(&SliderInputTaskTCB,                 		/* Create the Slider Input Task. */
                 "Slider Input Task",
				 SliderInputTask,
                  DEF_NULL,
                  SLIDER_INPUT_TASK_PRIO,
                 &SliderInputTaskStk[0],
                 (TASK_STK_SIZE / 10u),
                  TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);

    OSTaskCreate(&LcdDisplayTaskTCB,                          /* Create the LCD Display Task.                               */
                 "Lcd Display Task",
                  LcdDisplayTask,
                  DEF_NULL,
                  LCD_DISPLAY_TASK_PRIO,
                 &LcdDisplayTaskStk[0],
                 (TASK_STK_SIZE / 10u),
                  TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);

    OSTaskCreate(&PositionUpdateTaskTCB,                  		/* Create the Position Update Task.                               */
                 "Position Update Task",
				 PositionUpdateTask,
                  DEF_NULL,
                  POSITION_UPDATE_TASK_PRIO,
                 &PositionUpdateTaskStk[0],
                 (TASK_STK_SIZE / 10u),
                  TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);

	OSTaskCreate(&IdleTaskTCB,                          	/* Create the LED Output Task.                               */
			  "Idle Task",
			   IdleTask,
			   DEF_NULL,
			   IDLE_TASK_PRIO,
			  &IdleTaskStk[0],
			  (TASK_STK_SIZE / 10u),
			   TASK_STK_SIZE,
			   0u,
			   0u,
			   DEF_NULL,
			  (OS_OPT_TASK_STK_CLR),
			  &err);

    // Delete the start task
    OSTaskDel(&MainStartTaskTCB, &err);

}

/***************************************************************************//**
 * @brief
 * 	Initialize configuration data for the game
 *
 ******************************************************************************/
static void configurationInit() {

	config_data.ball_mass = 3;
	config_data.graphics_max_x = 100;
	config_data.graphics_min_x = 0;
	config_data.graphics_max_y = 100;
	config_data.graphics_min_y = 0;
	config_data.gravity = 0.98; 							/* Dialed down gravity by a factor of 10 to make the game playable */
	config_data.max_gain = 10;
	config_data.post_length = 40;
	config_data.project_variant = Inverted_Pendulum;

															/* Initialize Gain Data */
	current_gain = 1;

	current_score = 0;

															/* Initialize Slider Value */
	slider_value = Neutral;

															/* Initialize Position Data */
	ball_position.current_cart_position = 50;
	ball_position.current_theta = 0;
	ball_position.current_theta_first_derivative = 0;
	ball_position.current_theta_second_derivative = 0;
	ball_position.current_ball_x_position = 50;
	ball_position.current_ball_y_position = 40;
	ball_position.current_cart_velocity = 0;


}

/***************************************************************************//**
 * @brief
 * 	Idle tasks for energy efficiency
 *
 ******************************************************************************/
static void IdleTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif




    while (DEF_ON) {

    	EMU_EnterEM1();
                                                                /* Delay Start Task execution for                       */
        OSTimeDly( 25,                                        /*   25 OS Ticks                                      */
                   OS_OPT_TIME_DLY,                             /*   from now.                                          */
                  &err);
                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

/***************************************************************************//**
 * @brief
 * 	This is the Gain Setpoint task.
 *
 * @details
 * 	This task takes in the button input data from a FIFO structure and sets the current gain.
 * 	It is blocked by a semaphore that is released by the Button ISR.
 *
 * @param[in] *p_arg
 * 	Task arguments
 *
 ******************************************************************************/
static void GainSetpointTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    int new_gain_data;

    while (DEF_ON) {

    	// Wait for a button state to change
    	OSSemPend(&Button_Input_Semaphore,
    			0,
				OS_OPT_PEND_BLOCKING,
				NULL,
				&err);

    	// grab next gain data
    	new_gain_data = peek();

    	// Pop data from queue
    	pop();

    	// Updating the Gain Data
    	if(new_gain_data == PB0_Pressed && current_gain < config_data.max_gain) {

    		current_gain += 1;

    	}
    	if(new_gain_data == PB1_Pressed && current_gain > 1) {

    		current_gain -= 1;

    	}

                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

/***************************************************************************//**
 * @brief
 * This is the LED0 PWM task
 *
 * @details
 * 	This task takes looks at the current gain and then determines a PWM for LED0
 *
 * @param[in] *p_arg
 * 	Task arguments
 *
 ******************************************************************************/
static void LedPWMTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    int duty_cycle;
    int off_time;

    while (DEF_ON) {

    	duty_cycle = current_gain;
    	off_time = 10 - duty_cycle;

    	GPIO_PinOutSet(LED0_port, LED0_pin);

    	OSTimeDlyHMSM(0, 0, (CPU_INT16U)duty_cycle, 0, OS_OPT_TIME_DLY, &err);


    	GPIO_PinOutClear(LED0_port, LED0_pin);

    	OSTimeDlyHMSM(0, 0, (CPU_INT16U)off_time, 0, OS_OPT_TIME_DLY, &err);

                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

/***************************************************************************//**
 * @brief
 * This is the slider input task
 *
 * @details
 * 	This task takes in the capsense slideoff_timer position and then sets the slider_value
 * 	accordingly
 *
 * @param[in] *p_arg
 * 	Task arguments
 *
 ******************************************************************************/
static void SliderInputTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    /* Start capacitive sense buttons */
	CAPSENSE_Init();

    while (DEF_ON) {

    														/* Delay Vehicle Direction Task execution for 	*/
		OSTimeDly( TIME_DELAY_VALUE,                                      /*   25 OS Ticks								*/
				OS_OPT_TIME_DLY,                            /*   from now.									*/
				&err);

    	sampleSliderPosition(&slider_value);

                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

/***************************************************************************//**
 * @brief
 * 	This is the position update task.
 *
 * @details
 * 	This task will use rough physics to calculate the ball and cart position
 * 	applying Hamiltonian's equations. It will then update the position data which will then be sent
 * 	to a message queue for the LCD Display Task
 *
 * @param[in] *p_arg
 * 	Task arguments
 *
 ******************************************************************************/
static void PositionUpdateTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    double delta_t = PHYS_TAU;
    int local_slider_value;
    double cart_acceleration;

    while (DEF_ON) {

    	OSTimeDlyHMSM(0, 0, 0, (CPU_INT32U)delta_t, OS_OPT_TIME_PERIODIC, &err);

    	current_score ++;

    	// Make acceleration proportional to the gain
    	cart_acceleration = (double)current_gain / 1000 * delta_t;

    	local_slider_value = slider_value;

    	// Update theta variables for the Lagrangian
    	if(local_slider_value > 0) {

    		ball_position.current_cart_velocity = ball_position.current_cart_velocity + cart_acceleration;
    		ball_position.current_theta_second_derivative = ((config_data.gravity * cos(ball_position.current_theta) + (cart_acceleration * sin(ball_position.current_theta))) / config_data.post_length);

    	}

    	else if(local_slider_value < 0) {

    		ball_position.current_cart_velocity = ball_position.current_cart_velocity - cart_acceleration;
    		ball_position.current_theta_second_derivative = -((config_data.gravity * cos(ball_position.current_theta) + (cart_acceleration * sin(ball_position.current_theta))) / config_data.post_length);

    	}

    	else if(local_slider_value == 0) {

    		ball_position.current_cart_velocity = ball_position.current_cart_velocity;
    		ball_position.current_theta_second_derivative = ball_position.current_theta_second_derivative;

    	}

    	ball_position.current_theta_first_derivative = ball_position.current_theta_first_derivative + ball_position.current_theta_second_derivative * (delta_t / 1000);
    	ball_position.current_theta = ball_position.current_theta + ball_position.current_theta_first_derivative * (delta_t / 1000);

    	// Update cart position
    	ball_position.current_cart_position = ball_position.current_cart_position + ball_position.current_cart_velocity * (delta_t / 1000);

    	// Update Ball Position
    	ball_position.current_ball_x_position = ball_position.current_cart_position + (config_data.post_length * cos(ball_position.current_theta + PI/2));
    	ball_position.current_ball_y_position = config_data.post_length * sin(ball_position.current_theta + PI/2);



    	OSQPost(&Position_Data_Message_Queue,
    			&ball_position,
    			sizeof(ball_position),
				OS_OPT_POST_FIFO + OS_OPT_POST_ALL,
				&err);

                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}


/***************************************************************************//**
 * @brief
 *	This is the LCD Display Task.
 * @details
 *	It is in charge of taking in the psoition data
 *	which is passed from the message queue, and then displays it on the LCD screen
 *
 * @param[in] *p_arg
 * 	Task arguments
 *
 ******************************************************************************/
static void LcdDisplayTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    struct Position_Data* current_position_data;
    int data_size = sizeof(struct Position_Data*);

    AppGraphics_Init(&glibContext);                             /* Initialize the screen */


    while (DEF_ON) {


    	current_position_data = OSQPend(&Position_Data_Message_Queue,
									0,
									OS_OPT_PEND_BLOCKING,
									data_size,
									NULL,
									&err);


    	GLIB_clear(&glibContext);                               /* Clear the screen */

    	AppGraphics_DisplayScreen(&glibContext, current_position_data);

    	DMD_updateDisplay();                                    /* Update the screen

                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

/*
*********************************************************************************************************
*                                          AppGraphics_Init()
*
* Description : Initalize the graphics
*
* Argument(s) : pContext:   Pointer to the glib context structure
*
* Return(s)   : None.
*
* Notes       : None.
*********************************************************************************************************
*/

void AppGraphics_Init(GLIB_Context_t *pContext)
{
    EMSTATUS status;


    status = DMD_init(0);                                   /* Initialize the DMD module for the DISPLAY device driver  */
    if (DMD_OK != status) {
      while (1) {}
    }

    status = GLIB_contextInit(&glibContext);
    if (GLIB_OK != status) {
      while (1) {}
    }
}

/*
*********************************************************************************************************
*                                          AppGraphics_DisplayScreen()
*
* Description :
*
* Argument(s) : p_arg   Argument passed from task creation. Unused, in this case.
*
* Return(s)   : None.
*
* Notes       : None.
*********************************************************************************************************
*/

void AppGraphics_DisplayScreen(GLIB_Context_t *pContext, struct Position_Data* current_position_data)
{

	RTOS_ERR  err;

	char str1[12] = {0};
	char str2[12] = {0};
	char str3[12] = {0};
	char str4[12] = {0};


    GLIB_Rectangle_t rectBackground = { 0, 0, APP_GRAPHICS_MAX_X, APP_GRAPHICS_MAX_Y };

    int graphics_x1 = (current_position_data->current_ball_x_position * 128) / config_data.graphics_max_x;
    int graphics_x2 = (current_position_data->current_cart_position * 128) / config_data.graphics_max_x;
    int graphics_y1 = (current_position_data->current_ball_y_position * 128) / config_data.graphics_max_y;
    int graphics_y2 = 10;


    pContext->backgroundColor = Black;
    pContext->foregroundColor = Black;
    GLIB_drawRectFilled(pContext, &rectBackground);

    pContext->foregroundColor = White;

    if(graphics_x2 <= APP_GRAPHICS_MIN_X || graphics_x2 >= APP_GRAPHICS_MAX_X || graphics_y1 <= 10) {

    	strcpy(str3, "GAME OVER");
        snprintf(str4, sizeof(str4), "Score: %d ", current_score);
        GLIB_setFont(pContext, (GLIB_Font_t *)&GLIB_FontNormal8x8);
        GLIB_drawString(pContext,
    					   str3,
    					   strlen(str3),
    					   APP_GRAPHICS_CENTER_X - ((APP_GRAPHICS_GLIB_FONT_WIDTH * strlen(str3)) / 2),
    					   50,
    					   0);
        GLIB_drawString(pContext,
    					   str4,
    					   strlen(str4),
    					   APP_GRAPHICS_CENTER_X - ((APP_GRAPHICS_GLIB_FONT_WIDTH * strlen(str4)) / 2),
    					   60,
    					   0);
        GPIO_PinOutSet(LED1_port, LED1_pin);
        GPIO_PinOutClear(LED0_port, LED0_pin);
        OSSchedLock(&err);

    }


    strcpy(str1, "Inverted Pendulum");
    strcpy(str2, "Game");

    GLIB_setFont(pContext, (GLIB_Font_t *)&GLIB_FontNarrow6x8);
    GLIB_drawString(pContext,
					   str1,
					   strlen(str1),
					   APP_GRAPHICS_CENTER_X - ((APP_GRAPHICS_GLIB_FONT_WIDTH * strlen(str1)) / 2),
					   10,
					   0);
    GLIB_drawString(pContext,
					   str2,
					   strlen(str2),
					   APP_GRAPHICS_CENTER_X - ((APP_GRAPHICS_GLIB_FONT_WIDTH * strlen(str2)) / 2),
					   20,
					   0);

    GLIB_Rectangle_t cart = { graphics_x2 + 10, 128-10, graphics_x2 - 10, APP_GRAPHICS_MAX_Y };
    GLIB_drawRectFilled(pContext, &cart);
    GLIB_drawCircleFilled(pContext, graphics_x1, (128 - graphics_y1), 5);


    GLIB_setFont(pContext, (GLIB_Font_t *)&GLIB_FontNormal8x8);
    GLIB_drawLine(pContext, graphics_x1, (128 - graphics_y1), graphics_x2, (128 - graphics_y2));

}
