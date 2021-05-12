#ifndef MAIN_HG
#define MAIN_HG

//***********************************************************************************
// Include files
//***********************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"

#include "app.h"
#include "fifo_queue.h"
#include "brd_config.h"
#include  <bsp_os.h>
#include  "bsp.h"
#include  <display.h>
#include  "dmd.h"
#include  "glib.h"

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

#define  MAIN_START_TASK_PRIO              	9u
#define  TASK_STK_SIZE         				512u

#define  POSITION_UPDATE_TASK_PRIO			2u
#define  LCD_DISPLAY_TASK_PRIO				3u
#define  SLIDER_INPUT_TASK_PRIO				5u
#define  GAIN_SETPOINT_TASK_PRIO			6u
#define  GAME_RUNNING_TASK_PRIO				7u
#define  LED0_PWM_TASK_PRIO					3u
#define  IDLE_TASK_PRIO						64u

#define  TIME_DELAY_VALUE					25

#define  APP_GRAPHICS_GLIB_FONT_WIDTH       (glibContext.font.fontWidth + glibContext.font.charSpacing)
#define  APP_GRAPHICS_GLIB_FONT_HEIGHT      (glibContext.font.fontHeight)
#define  APP_GRAPHICS_CENTER_X              (glibContext.pDisplayGeometry->xSize / 2)
#define  APP_GRAPHICS_CENTER_Y              (glibContext.pDisplayGeometry->ySize / 2)
#define  APP_GRAPHICS_MAX_X                 (glibContext.pDisplayGeometry->xSize - 1)
#define  APP_GRAPHICS_MAX_Y                 (glibContext.pDisplayGeometry->ySize - 1)

#define  APP_GRAPHICS_MIN_X                  0u
#define  APP_GRAPHICS_MIN_Y                  0u
#define  APP_GRAPHICS_MAX_STR_LEN           48u

#define PHYS_TAU							100
#define PI									3.14


//***********************************************************************************
// global variables
//***********************************************************************************

																/* Semaphore for Button input */
OS_SEM Button_Input_Semaphore;


																/* Message Queue For passing Position Data to Display Task */
OS_Q Position_Data_Message_Queue;

																/* Lock for Position Data	*/
OS_MUTEX Ball_Position_Lock;

GLIB_Context_t  glibContext;

struct Configuration_Data config_data;

int current_gain;

int current_score;

int slider_value;

struct Position_Data ball_position;





//***********************************************************************************
// function prototypes
//***********************************************************************************

#endif
