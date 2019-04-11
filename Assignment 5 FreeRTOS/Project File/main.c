/* Assignment 5:
 * Course: Advanced Embedded Software Development
 *
 * main.c
 * Author:  Tanmay Chaturvedi
 *
 * Based on Demo Application by Andy Kobyljanec (https://github.com/akobyl/TM4C129_FreeRTOS_Demo)
 *
 * Device: EK-TM4C1294XL.  TivaWare driverlib sourcecode is included.
 *
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/inc/hw_memmap.h"
// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "main.h"

#include "myled_task.h"
#include "mytemp_task.h"
#include "mylogger_task.h"
#include "myi2c.h"
#include "myalert_task.h"


QueueHandle_t myqueuehandle = 0;

//System Clock handle
uint32_t output_clock_rate_hz;

BaseType_t xAlertHandle;
TaskHandle_t xAlertTaskHandle = NULL;


// Main function
return_type_t main(void)
{
    BaseType_t  xReturned;

    myqueuehandle = xQueueCreate(10, sizeof(logger_data_t));
    // Initialize system clock to 120 MHz

    output_clock_rate_hz = ROM_SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    //Initialize I2C0 module
    myi2cinit();

    // Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);

    // Set up the UART which is connected to the virtual COM port
    UARTStdioConfig(0, 57600, SYSTEM_CLOCK);

   xReturned = xTaskCreate(myled_task, (const portCHAR *)"LED Task",
                        configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    if(xReturned != pdPASS)
    {
        UARTprintf("LED TASK FAILED");
        return LED_TASK_CREATE_FAILED;
    }

    xReturned = xTaskCreate(mytemp_task, (const portCHAR *)"Temp Task",
                        configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    if(xReturned != pdPASS)
    {
        UARTprintf("TEMP TASK FAILED");
        return TEMP_TASK_CREATE_FAILED;
    }

    xReturned = xTaskCreate(mylogger_task, (const portCHAR *)"Logger Task",
                        configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    if(xReturned != pdPASS)
    {
        UARTprintf("LOGGER TASK FAILED");
        return LOGGER_TASK_CREATE_FAILED;
    }

    xAlertHandle = xTaskCreate(myalert_task,(const portCHAR *)"Alert Task",
                               configMINIMAL_STACK_SIZE, NULL, 1, &xAlertTaskHandle);

    if(xAlertHandle != pdPASS)
    {
        UARTprintf("ALERT TASK FAILED");
        return ALERT_TASK_CREATE_FAILED;
    }



    vTaskStartScheduler();
    return PROGRAM_SUCCESS;
}


/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
