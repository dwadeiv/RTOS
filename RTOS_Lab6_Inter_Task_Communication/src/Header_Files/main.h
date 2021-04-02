#ifndef MAIN_HG
#define MAIN_HG

//***********************************************************************************
// Include files
//***********************************************************************************

#include <stdint.h>
#include <stdbool.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"

#include "app.h"
#include "brd_config.h"
#include  <bsp_os.h>
#include  "bsp.h"

#include  <cpu/include/cpu.h>
#include  <common/include/common.h>
#include  <kernel/include/os.h>
#include  <kernel/include/os_trace.h>

#include  <common/include/lib_def.h>
#include  <common/include/rtos_utils.h>
#include  <common/include/toolchains.h>

//***********************************************************************************
// defined files
//***********************************************************************************

#define  MAIN_START_TASK_PRIO              	21u
#define  TASK_STK_SIZE         				512u

#define  BUTTON_INPUT_TASK_PRIO             1u
#define  SLIDER_INPUT_TASK_PRIO             2u
#define  LED_OUTPUT_TASK_PRIO              	3u
#define  IDLE_TASK_PRIO              	64u


//***********************************************************************************
// global variables
//***********************************************************************************
																/* Message Queue                                        */
OS_Q	 Message_Queue;

																/* Semaphore for Slider input                           */
OS_SEM SliderSemaphore;

																/* Event flag group for button inputs                   */
OS_FLAG_GRP ButtonInputEventGroup;


																/* Periodic Timer                                       */
OS_TMR PeriodicTimer;

//***********************************************************************************
// function prototypes
//***********************************************************************************

#endif
