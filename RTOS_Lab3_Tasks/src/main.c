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
*/                                                       /* Start Task Stack.                                    */
static  CPU_STK  MainStartTaskStk[TASK_STK_SIZE];
                                                                /* Start Task TCB.                                      */
static  OS_TCB   MainStartTaskTCB;

static  CPU_STK  ButtonInputTaskStk[TASK_STK_SIZE];
                                                                /* Start Task TCB.                                      */
static  OS_TCB   ButtonInputTaskTCB;

static  CPU_STK  SliderInputTaskStk[TASK_STK_SIZE];
                                                                /* Start Task TCB.                                      */
static  OS_TCB   SliderInputTaskTCB;

static  CPU_STK  LedOutputTaskStk[TASK_STK_SIZE];
                                                                /* Start Task TCB.                                      */
static  OS_TCB   LedOutputTaskTCB;

static  CPU_STK  IdleTaskStk[TASK_STK_SIZE];
                                                                /* Start Task TCB.                                      */
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

	OSTaskCreate(&IdleTaskTCB,                          /* Create the LED Output Task.                               */
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

static void ButtonInputTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif




    while (DEF_ON) {
  	  samplePushButton_1();
  	  samplePushButton_0();
                                                                /* Delay Start Task execution for                       */
        OSTimeDly( 25,                                        /*   25 OS Ticks                                      */
                   OS_OPT_TIME_DLY,                             /*   from now.                                          */
                  &err);
                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

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

    	sampleSliderPosition();
                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}

static void LedOutputTask(void *p_arg)
{

    RTOS_ERR  err;


    PP_UNUSED_PARAM(p_arg);                                     /* Prevent compiler warning.                            */

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();                                /* Initialize interrupts disabled measurement.          */
#endif

    while (DEF_ON) {

    	adjustLED();
                                                                /* Delay Start Task execution for                       */
        OSTimeDly( 25,                                        /*   1000 OS Ticks                                      */
                   OS_OPT_TIME_DLY,                             /*   from now.                                          */
                  &err);
                                                                /*   Check error code.                                  */
        APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), ;);
    }

}
