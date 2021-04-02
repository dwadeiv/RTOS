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

#define  MAIN_START_TASK_PRIO              	21u
#define  TASK_STK_SIZE         				512u

#define  SPEED_SETPOINT_TASK_PRIO			2u
#define  VEHICLE_DIRECTION_TASK_PRIO		1u
#define  VEHICLE_MONITOR_TASK_PRIO			3u
#define  LED_OUTPUT_TASK_PRIO				4u
#define  LCD_DISPLAY_TASK_PRIO				5u
#define  IDLE_TASK_PRIO						64u

#define  GENERAL_MAX_SPEED					75
#define  TURNING_MAX_SPEED					45

#define  TIME_DELAY_VALUE					25
#define  TURN_TIME_LIMIT					900	/* Based on Default 1000 Hz OS Tick Rate */

#define  APP_GRAPHICS_GLIB_FONT_WIDTH       (glibContext.font.fontWidth + glibContext.font.charSpacing)
#define  APP_GRAPHICS_GLIB_FONT_HEIGHT      (glibContext.font.fontHeight)
#define  APP_GRAPHICS_CENTER_X              (glibContext.pDisplayGeometry->xSize / 2)
#define  APP_GRAPHICS_CENTER_Y              (glibContext.pDisplayGeometry->ySize / 2)
#define  APP_GRAPHICS_MAX_X                 (glibContext.pDisplayGeometry->xSize - 1)
#define  APP_GRAPHICS_MAX_Y                 (glibContext.pDisplayGeometry->ySize - 1)

#define  APP_GRAPHICS_MIN_X                  0u
#define  APP_GRAPHICS_MIN_Y                  0u
#define  APP_GRAPHICS_MAX_STR_LEN           48u


//***********************************************************************************
// global variables
//***********************************************************************************

																/* Semaphore for Button input */
OS_SEM Button_Input_Semaphore;

																/* Event flag group for vehicle monitoring */
OS_FLAG_GRP Vehicle_Monitor_Event_Group;

																/* Event flag group for LED Output */
OS_FLAG_GRP LED_Output_Event_Group;

																/* Lock for Speed Data */
OS_MUTEX Speed_Setpoint_Lock;

																/* Lock for Direction Data	*/
OS_MUTEX Vehicle_Direction_Lock;

GLIB_Context_t  glibContext;

//struct Node* Button_1_Event_FIFO;
//struct Node* Button_2_Event_FIFO;
struct Vehicle_Speed  speed;
struct Vehicle_Direction direction;





//***********************************************************************************
// function prototypes
//***********************************************************************************

#endif
