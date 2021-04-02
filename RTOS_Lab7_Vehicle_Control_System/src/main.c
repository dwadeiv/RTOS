/***************************************************************************//**
 * @file
 * @brief Simple LED Blink Demo for SLSTK3402A
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
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

static  CPU_STK  SpeedSetpointTaskStk[TASK_STK_SIZE];
                                                                /* Speed Setpoint Task TCB. */
static  OS_TCB   SpeedSetpointTaskTCB;

static  CPU_STK  VehicleDirectionTaskStk[TASK_STK_SIZE];
                                                                /* Vehicle Direction Task TCB. */
static  OS_TCB   VehicleDirectionTaskTCB;

static  CPU_STK  VehicleMonitorTaskStk[TASK_STK_SIZE];
                                                                /* Vehicle Monitor Task TCB. */
static  OS_TCB   VehicleMonitorTaskTCB;

static  CPU_STK  LedOutputTaskStk[TASK_STK_SIZE];
                                                                /* LED Output Task TCB. */
static  OS_TCB   LedOutputTaskTCB;

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

static  void  MainStartTask (void  *p_arg);
static void SpeedSetpointTask(void *p_arg);
static void VehicleDirectionTask(void *p_arg);
static void VehicleMonitorTask(void *p_arg);
static void LedOutputTask(void *p_arg);
static void LcdDisplayTask(void *p_arg);
static void IdleTask(void *p_arg);

static void AppGraphics_Init(GLIB_Context_t *pContext);
static void AppGraphics_DisplayScreen(GLIB_Context_t *pContext, int current_speed, int current_direction);


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


																/* Create Speed Setpoint Mutex */
    OSMutexCreate(&Speed_Setpoint_Lock,
    		(CPU_CHAR *)"Speed Data Mutex",
			&err);

    															/* Create Vehicle Direction Mutex */
    OSMutexCreate(&Vehicle_Direction_Lock,
    		(CPU_CHAR *)"Vehicle Direction Data Mutex",
			&err);
    															/* Create Button Input Semaphore */
    OSSemCreate((OS_SEM *) &Button_Input_Semaphore,
    		(CPU_CHAR *)"Button Input Semaphore",
			0,
			(RTOS_ERR *)&err);

																/* Create Vehicle Monitoring Event Flag Group */
    OSFlagCreate((OS_FLAG_GRP *) &Vehicle_Monitor_Event_Group,
    		(CPU_CHAR *)"Vehicle Monitoring Flags",
			Vehicle_None_Flag,
			(RTOS_ERR *)&err);

																/* Create LED Output Event Flag Group */
    OSFlagCreate((OS_FLAG_GRP *) &LED_Output_Event_Group,
			(CPU_CHAR *)"LED Output Flags",
			Both_LED_Off,
			(RTOS_ERR *)&err);

    															/* Initialize Buttone Event FIFO Structure */
    queueInit();

    															/* Initialize Vehicle Speed Data Structure */
    speed.current_speed = 0;
    speed.increment_count = 0;
    speed.decrement_count = 0;

																/* Initialize Vehicle Direction Data Structure */
    direction.current_direction = Vehicle_Neutral;
	direction.current_direction_time_held = 0;
	direction.left_turns_number = 0;
	direction.right_turns_number = 0;


    OSTaskCreate(&SpeedSetpointTaskTCB,                          /* Create the Speed Setpoint Task. */
                 "Speed Setpoint Task",
                  SpeedSetpointTask,
                  DEF_NULL,
                  SPEED_SETPOINT_TASK_PRIO,
                 &SpeedSetpointTaskStk[0],
                 (TASK_STK_SIZE / 10u),
                  TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);

    OSTaskCreate(&VehicleDirectionTaskTCB,                          /* Create the Vehicle Direction Task. */
                 "Vehicle Direction Task",
                  VehicleDirectionTask,
                  DEF_NULL,
                  VEHICLE_DIRECTION_TASK_PRIO,
                 &VehicleDirectionTaskStk[0],
                 (TASK_STK_SIZE / 10u),
                  TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);

    OSTaskCreate(&VehicleMonitorTaskTCB,                          /* Create the Vehicle Monitor Task.                               */
                 "Vehicle Monitor Task",
                  VehicleMonitorTask,
                  DEF_NULL,
                  VEHICLE_MONITOR_TASK_PRIO,
                 &VehicleMonitorTaskStk[0],
                 (TASK_STK_SIZE / 10u),
                  TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);

    OSTaskCreate(&LedOutputTaskTCB,                          /* Create the LED Output Task.                               */
                 "Led Output Task",
                  LedOutputTask,
                  DEF_NULL,
                  LED_OUTPUT_TASK_PRIO,
                 &LedOutputTaskStk[0],
                 (TASK_STK_SIZE / 10u),
                  TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);

    OSTaskCreate(&LcdDisplayTaskTCB,                          /* Create the LED Output Task.                               */
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
 *
 * @details
 *
 * @param[in] *p_arg
 * 	Task arguments
 *
 ******************************************************************************/
static void SpeedSetpointTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    int new_speed_data;

    while (DEF_ON) {

    	// Wait for a button state to change
    	OSSemPend(&Button_Input_Semaphore,
    			0,
				OS_OPT_PEND_BLOCKING,
				NULL,
				&err);

    	// Lock Speed Set Point Data
    	OSMutexPend(&Speed_Setpoint_Lock,
    			0,
				OS_OPT_PEND_BLOCKING,
				NULL,
				&err);


    	// grab next speed data
    	new_speed_data = peek();

    	// Pop data from queue
    	pop();

    	// Updating the Speed Data
    	if(new_speed_data == PB0_Pressed) {

    		speed.current_speed += 5;
    		speed.increment_count ++;

    	}
    	if(new_speed_data == PB1_Pressed && speed.current_speed >= 5) {

    		speed.current_speed -= 5;
    		speed.decrement_count ++;

    	}

    	// Unlock Speed Set Point Data
    	OSMutexPost(&Speed_Setpoint_Lock,
				OS_OPT_POST_NONE,
				&err);

    	// Flag that the vehicle is speeding up or slowing down
    	if(new_speed_data != Both_Buttons_Pressed && new_speed_data != No_Buttons_Pressed) {

    		OSFlagPost(&Vehicle_Monitor_Event_Group,
    				Vehicle_Speed_Flag,
    				OS_OPT_POST_FLAG_SET,
    				&err);

    	}

    	// Flag that the vehicle is not speeding up or slowing down
    	else {

    		OSFlagPost(&Vehicle_Monitor_Event_Group,
    				Vehicle_None_Flag,
    				OS_OPT_POST_FLAG_SET,
    				&err);

    	}

                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}



/***************************************************************************//**
 * @brief
 *
 * @details
 *
 * @param[in] *p_arg
 * 	Task arguments
 *
 ******************************************************************************/
static void VehicleDirectionTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    int slider_value;

    /* Start capacitive sense buttons */
	CAPSENSE_Init();



    while (DEF_ON) {


    														/* Delay Vehicle Direction Task execution for 	*/
		OSTimeDly( TIME_DELAY_VALUE,                                      /*   25 OS Ticks								*/
				OS_OPT_TIME_DLY,                            /*   from now.									*/
				&err);

    	int old_slider_value = slider_value;

    	sampleSliderPosition(&slider_value);

    	// Lock Vehicle Direction Data
    	OSMutexPend(&Vehicle_Direction_Lock,
    			0,
				OS_OPT_PEND_BLOCKING,
				NULL,
				&err);

    	// Direction is hard right and was hard right before
    	if(slider_value == Vehicle_Hard_Right && old_slider_value == Vehicle_Hard_Right) {

    		direction.current_direction = Vehicle_Hard_Right;
    		direction.current_direction_time_held += TIME_DELAY_VALUE;
    		direction.right_turns_number ++;

    	}

    	// New direction is hard right
    	else if(slider_value == Vehicle_Hard_Right) {

    		direction.current_direction = Vehicle_Hard_Right;
    		direction.current_direction_time_held = 0;
    		direction.right_turns_number ++;

    	}

    	// Direction is right and was right before
    	else if(slider_value == Vehicle_Right && old_slider_value == Vehicle_Right) {

    		direction.current_direction = Vehicle_Right;
    		direction.current_direction_time_held += TIME_DELAY_VALUE;
    		direction.right_turns_number ++;

    	}

    	// New direction is right
    	else if(slider_value == Vehicle_Right) {

    		direction.current_direction = Vehicle_Right;
    		direction.current_direction_time_held = 0;
    		direction.right_turns_number ++;

    	}

    	// Direction is hard left and was hard left before
    	else if(slider_value == Vehicle_Hard_Left && old_slider_value == Vehicle_Hard_Left) {

    		direction.current_direction = Vehicle_Hard_Left;
    		direction.current_direction_time_held += TIME_DELAY_VALUE;
    		direction.left_turns_number ++;

    	}

    	// New direction is hard left
    	else if(slider_value == Vehicle_Hard_Left) {

    		direction.current_direction = Vehicle_Hard_Left;
    		direction.current_direction_time_held = 0;
    		direction.left_turns_number ++;

    	}

    	// Direction is left and was left before
    	else if(slider_value == Vehicle_Left && old_slider_value == Vehicle_Left) {

    		direction.current_direction = Vehicle_Left;
    		direction.current_direction_time_held += TIME_DELAY_VALUE;
    		direction.left_turns_number ++;

    	}

    	// New direction is left
    	else if(slider_value == Vehicle_Left) {

    		direction.current_direction = Vehicle_Left;
    		direction.current_direction_time_held = 0;
    		direction.left_turns_number ++;

    	}

    	// Vehicle is not making a turn
    	else {

    		direction.current_direction = Vehicle_Neutral;
    		direction.current_direction_time_held = 0;

    	}

    	// Unlock Vehicle Direction Data
    	OSMutexPost(&Vehicle_Direction_Lock,
				OS_OPT_POST_NONE,
				&err);

    	// Flag that the vehicle is turning
    	if(slider_value != Vehicle_Neutral) {

    		OSFlagPost(&Vehicle_Monitor_Event_Group,
    				Vehicle_Direction_Flag,
    				OS_OPT_POST_FLAG_SET,
    				&err);

    	}

    	// Flag that the vehicle is not turning
    	else {

    		OSFlagPost(&Vehicle_Monitor_Event_Group,
    				Vehicle_None_Flag,
    				OS_OPT_POST_FLAG_SET,
    				&err);

    	}

                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

/***************************************************************************//**
 * @brief
 *
 * @details
 *
 * @param[in] *p_arg
 * 	Task arguments
 *
 ******************************************************************************/
static void VehicleMonitorTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    int led_0_state = 0;
    int led_1_state = 0;
    int vehicle_turning = 0;
    int current_speed = 0;
    int current_direction_time_held = 0;
    int flag_value;


    while (DEF_ON) {

    	flag_value = OSFlagPend(&Vehicle_Monitor_Event_Group,
    					Vehicle_Direction_Flag + Vehicle_Speed_Flag + Vehicle_None_Flag,
						0,
						OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME,
						NULL,
						&err);

    	// No change to vehicle direction or speed
    	if(flag_value == Vehicle_None_Flag) {

    		vehicle_turning = 0;
    		led_1_state = 0;

    	}

    	// Change in Vehicle Direction
    	else if(flag_value == Vehicle_Direction_Flag) {

        	// Lock Vehicle Direction Data
        	OSMutexPend(&Vehicle_Direction_Lock,
        			0,
    				OS_OPT_PEND_BLOCKING,
    				NULL,
    				&err);

			current_direction_time_held = direction.current_direction_time_held;

        	// Unlock Vehicle Direction Data
        	OSMutexPost(&Vehicle_Direction_Lock,
    				OS_OPT_POST_NONE,
    				&err);

        	// State if vehicle is turning
			vehicle_turning = 1;



        	// Checking for turn time violation
        	if(current_direction_time_held > TURN_TIME_LIMIT) {

        		led_1_state = 1;

        	}

    	}

    	// Change in Vehicle Speed
    	else if(flag_value == Vehicle_Speed_Flag) {

        	// Lock Speed Set Point Data
        	OSMutexPend(&Speed_Setpoint_Lock,
        			0,
    				OS_OPT_PEND_BLOCKING,
    				NULL,
    				&err);

        	// Storing current speed locally
        	current_speed = speed.current_speed;

        	// Unlock Speed Set Point Data
        	OSMutexPost(&Speed_Setpoint_Lock,
    				OS_OPT_POST_NONE,
    				&err);

        	if(current_speed > GENERAL_MAX_SPEED) {

        		led_0_state = 1;

        	}

        	else if(current_speed > TURNING_MAX_SPEED && vehicle_turning) {

        		led_0_state = 1;

        	}

        	else {

        		led_0_state = 0;

        	}


    	}

    	// This is to force LED0 off if the vehicle stops turning and speed is in range 45 < speed < 75
    	if(current_speed > TURNING_MAX_SPEED && !vehicle_turning && current_speed < GENERAL_MAX_SPEED) {

    		led_0_state = 0;

    	}

    	// Otherwise vehicle is still having speed violation
    	else if(current_speed > TURNING_MAX_SPEED && vehicle_turning) {

    		led_0_state = 1;

    	}

    	if(led_0_state && led_1_state) {

    		OSFlagPost(&LED_Output_Event_Group,
    				Both_LED_On,
    				OS_OPT_POST_FLAG_SET,
    				&err);

    	}

    	else if(led_0_state) {

    		OSFlagPost(&LED_Output_Event_Group,
    				LED0_Only_On,
    				OS_OPT_POST_FLAG_SET,
    				&err);

    	}

    	else if(led_1_state) {

    		OSFlagPost(&LED_Output_Event_Group,
    				LED1_Only_On,
    				OS_OPT_POST_FLAG_SET,
    				&err);

    	}

    	else {

    		OSFlagPost(&LED_Output_Event_Group,
    				Both_LED_Off,
    				OS_OPT_POST_FLAG_SET,
    				&err);

    	}



                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

/***************************************************************************//**
 * @brief
 * 	LED Output Task
 *
 * @details
 * 	Takes in the next piece of data in the message queue. It then updates the pushbutton
 * 	states if the datat is pushbutton related. After, it does the control of the LEDs
 * 	using the current pushbutton states and/or the touchslider datat information
 *
 * @param[in] *p_arg
 * 	Task arguments
 *
 ******************************************************************************/
static void LedOutputTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    int flag_value;

    while (DEF_ON) {

    	flag_value = OSFlagPend(&LED_Output_Event_Group,
    					LED0_Only_On + LED1_Only_On + Both_LED_On + Both_LED_Off,
						0,
						OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME,
						NULL,
						&err);

    	if(flag_value == LED0_Only_On) {

    		GPIO_PinOutClear(LED1_port, LED1_pin);
    		GPIO_PinOutSet(LED0_port, LED0_pin);

    	}

    	else if(flag_value == LED1_Only_On) {

    		GPIO_PinOutClear(LED0_port, LED0_pin);
    		GPIO_PinOutSet(LED1_port, LED1_pin);

    	}

    	else if(flag_value == Both_LED_On) {

    		GPIO_PinOutSet(LED0_port, LED0_pin);
    		GPIO_PinOutSet(LED1_port, LED1_pin);

    	}

    	else if(flag_value == Both_LED_Off){

    		GPIO_PinOutClear(LED0_port, LED0_pin);
//    		GPIO_PinOutSet(LED0_port, LED0_pin);
    		GPIO_PinOutClear(LED1_port, LED1_pin);

    	}

                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

/***************************************************************************//**
 * @brief
 *
 * @details
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

    int current_speed = 0;
    int current_direction;

    AppGraphics_Init(&glibContext);                             /* Initialize the screen */


    while (DEF_ON) {


    	// Lock Vehicle Direction Data
    	OSMutexPend(&Vehicle_Direction_Lock,
    			0,
				OS_OPT_PEND_BLOCKING,
				NULL,
				&err);

    	current_direction = direction.current_direction;

    	// Unlock Vehicle Direction Data
    	OSMutexPost(&Vehicle_Direction_Lock,
				OS_OPT_POST_NONE,
				&err);



    	// Lock Speed Set Point Data
    	OSMutexPend(&Speed_Setpoint_Lock,
    			0,
				OS_OPT_PEND_BLOCKING,
				NULL,
				&err);

    	current_speed = speed.current_speed;

    	// Unlock Vehicle Direction Data
    	OSMutexPost(&Speed_Setpoint_Lock,
				OS_OPT_POST_NONE,
				&err);


    	GLIB_clear(&glibContext);                               /* Clear the screen */

    	AppGraphics_DisplayScreen(&glibContext, current_speed, current_direction);

    	DMD_updateDisplay();                                    /* Update the screen


																/* Delay LCD Display Task execution for 	*/
    	OSTimeDly( TIME_DELAY_VALUE,                            /*   25 OS Ticks							*/
    			OS_OPT_TIME_DLY,                                /*   from now.								*/
				&err);

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

void AppGraphics_DisplayScreen(GLIB_Context_t *pContext, int current_speed, int current_direction)
{

	char str1[12] = {0};
	char str2[20] = {0};
	char str3[20] = {0};

    GLIB_Rectangle_t rectBackground = { 0, 0, APP_GRAPHICS_MAX_X, APP_GRAPHICS_MAX_Y };


    pContext->backgroundColor = White;
    pContext->foregroundColor = White;
    GLIB_drawRectFilled(pContext, &rectBackground);

    pContext->foregroundColor = Black;

    strcpy(str1, "Current Vehicle State");

    GLIB_setFont(pContext, (GLIB_Font_t *)&GLIB_FontNarrow6x8);
    GLIB_drawString(pContext,
					   str1,
					   strlen(str1),
					   APP_GRAPHICS_CENTER_X - ((APP_GRAPHICS_GLIB_FONT_WIDTH * strlen(str1)) / 2),
					   10,
					   0);


    snprintf(str2, sizeof(str2), "Speed: %d mph", current_speed);
    GLIB_drawString(&glibContext, str2, strlen(str2), 0, 40, false);

    switch(current_direction) {

		case(Vehicle_Left):{

			strcpy(str3, "Direction: Left");
			break;

		}

		case(Vehicle_Hard_Left):{

			strcpy(str3, "Direction: Hard Left");
			break;

		}

		case(Vehicle_Right):{

			strcpy(str3, "Direction: Right");
			break;

		}

		case(Vehicle_Hard_Right):{

			strcpy(str3, "Direction: Hard Right");
			break;

		}

		case(Vehicle_Neutral):{

			strcpy(str3, "Direction: Neutral");
			break;

		}

		default: {

			EFM_ASSERT(false);
			break;

		}

    }

    GLIB_drawString(&glibContext, str3, strlen(str3), 0, 70, false);
}
