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
*/                                                       		/* Start Task Stack.                                    */
static  CPU_STK  MainStartTaskStk[TASK_STK_SIZE];

																/* Start Task TCB.                                      */
static  OS_TCB   MainStartTaskTCB;

static  CPU_STK  ButtonInputTaskStk[TASK_STK_SIZE];
                                                                /* Button Input Task TCB.                               */
static  OS_TCB   ButtonInputTaskTCB;

static  CPU_STK  SliderInputTaskStk[TASK_STK_SIZE];
                                                                /* Slider Input Task TCB.                               */
static  OS_TCB   SliderInputTaskTCB;

static  CPU_STK  LedOutputTaskStk[TASK_STK_SIZE];
                                                                /* LED Output Task TCB.                                 */
static  OS_TCB   LedOutputTaskTCB;

static  CPU_STK  IdleTaskStk[TASK_STK_SIZE];
                                                                /* IDLE Task TCB.                                       */
static  OS_TCB   IdleTaskTCB;




/*
*********************************************************************************************************
*********************************************************************************************************
*                                       LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*********************************************************************************************************
*/

static  void  MainStartTask (void  *p_arg);
static void ButtonInputTask(void *p_arg);
static void SliderInputTask(void *p_arg);
static void LedOutputTask(void *p_arg);
static void IdleTask(void *p_arg);
void TimerCallback(OS_TMR p_tmr, void* not_used);


int  main (void)
{
    RTOS_ERR  err;


    BSP_SystemInit();                                           /* Initialize System.                                   */
    CPU_Init();                                                 /* Initialize CPU.                                      */

    OS_TRACE_INIT();
    OSInit(&err);                                               /* Initialize the Kernel.                               */
                                                                /*   Check error code.                                  */
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


    															/* Create Message Queue */
    OSQCreate((OS_Q *)&Message_Queue,
    		(CPU_CHAR *)"Message Queue",
			(OS_MSG_QTY)4,
			(RTOS_ERR *)&err);

    															/* Create Semaphore */
    OSSemCreate((OS_SEM *) &SliderSemaphore,
    		(CPU_CHAR *)"Slider Semaphore",
			0,
			(RTOS_ERR *)&err);

																/* Create Button Input Event Flag Group */
    OSFlagCreate((OS_FLAG_GRP *) &ButtonInputEventGroup,
    		(CPU_CHAR *)"Button Flags",
			No_Buttons_Pressed,
			(RTOS_ERR *)&err);

    															/* Create Periodic Timer for Slider */
    OSTmrCreate((OS_TMR *) &PeriodicTimer,
    		(CPU_CHAR *)"Periodic Timer",
			0,
			1,
			OS_OPT_TMR_PERIODIC,
			&TimerCallback,
			NULL,
			(RTOS_ERR *)&err);

    OSTmrStart(&PeriodicTimer,
               &err);





    OSTaskCreate(&ButtonInputTaskTCB,                          /* Create the Button Input Task.                               */
                 "Button Input Task",
                  ButtonInputTask,
                  DEF_NULL,
                  BUTTON_INPUT_TASK_PRIO,
                 &ButtonInputTaskStk[0],
                 (TASK_STK_SIZE / 10u),
                  TASK_STK_SIZE,
                  0u,
                  0u,
                  DEF_NULL,
                 (OS_OPT_TASK_STK_CLR),
                 &err);
    OSTaskCreate(&SliderInputTaskTCB,                          /* Create the Slider Input Task.                               */
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
 * 	Button Input Task
 *
 * @details
 *	Pends on Pushbutton Event Flag group to check the status of the Pushbuttons.
 *	It then posts the data to the message queue
 *
 * @param[in] *p_arg
 * 	Task arguments
 *
 ******************************************************************************/
static void ButtonInputTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    int flag_value;
    int q_value = noButton;

    while (DEF_ON) {

    	flag_value = OSFlagPend(&ButtonInputEventGroup,
    					PB0_Pressed + PB1_Pressed + PB0_Not_Pressed + PB1_Not_Pressed + Both_Buttons_Pressed,
						0,
						OS_OPT_PEND_FLAG_SET_ANY + OS_OPT_PEND_FLAG_CONSUME,
						NULL,
						&err);

    	if(flag_value == PB0_Pressed) {

    		q_value = pushButton_0_pressed;

    	}

    	else if(flag_value == PB1_Pressed) {

    		q_value = pushButton_1_pressed;

    	}

    	else if(flag_value == PB0_Not_Pressed) {

    		q_value = pushButton_0_not_pressed;

    	}

    	else if(flag_value == PB1_Not_Pressed) {

    		q_value = pushButton_1_not_pressed;

    	}

    	else if(flag_value == Both_Buttons_Pressed) {

    		q_value = BothButtons;

    	}

    	else {

    		q_value = noButton;

    	}


    	OSQPost(&Message_Queue,
    			&q_value,
    			sizeof(q_value),
    			OS_OPT_POST_FIFO + OS_OPT_POST_ALL,
    			&err);


                                                                /*   Check error code.                                  */
    	APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

/***************************************************************************//**
 * @brief
 * 	Slider Input Task
 *
 * @details
 *	Samples the Slider Position and then posts the data into the message queue.
 *	It then pends on a Semaphore that is released by the Periodic Timer.
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

    int q_value;

    /* Start capacitive sense buttons */
	CAPSENSE_Init();



    while (DEF_ON) {

    	sampleSliderPosition(&q_value);

    	OSQPost(&Message_Queue,
    			&q_value,
    			sizeof(q_value),
				OS_OPT_POST_FIFO + OS_OPT_POST_ALL,
				&err);

    	OSSemPend(&SliderSemaphore,
    			0,
				OS_OPT_PEND_BLOCKING,
				NULL,
				&err);

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

    int* queue_data;
    int* queue_size;

    int pushButton_0_state = 0;
    int pushButton_1_state = 0;

    while (DEF_ON) {

    	queue_data = OSQPend(&Message_Queue,
    				0,
					OS_OPT_PEND_BLOCKING,
					queue_size,
					NULL,
					&err);

    	/* Updating PushButton States */
    	if(*queue_data == pushButton_0_pressed) {

    		pushButton_0_state = 1;

    	}

    	if(*queue_data == pushButton_1_pressed) {

    		pushButton_1_state = 1;

    	}

    	if(*queue_data == pushButton_0_not_pressed) {

    		pushButton_0_state = 0;

    	}

    	if(*queue_data == pushButton_1_not_pressed) {

    		pushButton_1_state = 0;

    	}

    	if(*queue_data == noButton) {

    		pushButton_0_state = 0;
    		pushButton_1_state = 0;

    	}

    	if(*queue_data == BothButtons) {

    		pushButton_0_state = 1;
    		pushButton_1_state = 1;

    	}

    	/* Conditional for LED0 */
    	if((pushButton_0_state && ! pushButton_1_state) || *queue_data == touchSliderPositionLeft){

    		GPIO_PinOutSet(LED0_port, LED0_pin);

    	}
    	else {

    		GPIO_PinOutClear(LED0_port, LED0_pin);

    	}

    	/* Conditional for LED1 */
    	if((pushButton_1_state && ! pushButton_0_state) || *queue_data == touchSliderPositionRight){

    		GPIO_PinOutSet(LED1_port, LED1_pin);

    	}
    	else {

    		GPIO_PinOutClear(LED1_port, LED1_pin);

    	}

                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

/***************************************************************************//**
 * @brief
 * 	Timer Callback function
 *
 * @details
 * 	Posts the Semaphore which will release any Pends
 *
 * @param[in] *p_arg
 * 	Task arguments
 *
 ******************************************************************************/
void TimerCallback(OS_TMR p_tmr, void* not_used) {

	RTOS_ERR  err;

	OSSemPost(&SliderSemaphore, OS_OPT_POST_1, &err); /* post slider semaphore */

}
