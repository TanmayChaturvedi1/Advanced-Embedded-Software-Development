/*
 * mylogger_task.c
 *
 *  Created on: 8 Apr 2019
 *      Author: TanmayC
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"
#include "mylogger_task.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

extern QueueHandle_t myqueuehandle;
logger_data_t data_txrx;

void mylogger_task(void *pvParameters)
{
    char temp_str[30];
    for (;;)
    {

       if(xQueueReceive(myqueuehandle, &data_txrx , 100))
       {


           if(data_txrx.task_id == TEMP_TASK)
           {
               UARTprintf("\n\r[%d ms]     ", data_txrx.timestamp);
               sprintf(temp_str, "Temp value = %d.%d\n\r",(uint8_t)data_txrx.temperature, (uint8_t)(data_txrx.temperature * 100) );
               UARTprintf("%s\n\r", temp_str);
           }
           else if (data_txrx.task_id == LED_TASK)
           {
               UARTprintf("[%d ms]     ", data_txrx.timestamp);
               UARTprintf("Toggle Count = %d", data_txrx.led_toggle_count);
               UARTprintf("     Name = %s\n\r", data_txrx.name);
           }
           else if(data_txrx.task_id == ALERT_TASK)
           {
               UARTprintf("\n\n\n[%d ms]     ", data_txrx.timestamp);
               UARTprintf("*****Temperature Beyond Threshold!*****          ");
               sprintf(temp_str, "Temp value = %d.%d\n\r",(uint8_t)data_txrx.temperature, (uint8_t)(data_txrx.temperature * 100) );
               UARTprintf("%s\n\r", temp_str);
           }

       }
       else
           UARTprintf("Not able to Receive");

    }
}




