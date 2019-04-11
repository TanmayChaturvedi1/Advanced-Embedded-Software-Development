/*
 * myled_task.c
 *
 *  Created on: 8 Apr 2019
 *      Author: TanmayC
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"
#include "driverlib/gpio.h"
#include "driverlib/inc/hw_memmap.h"
#include "mylogger_task.h"
#include "myled_task.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

logger_data_t data_txrx;
extern QueueHandle_t myqueuehandle;


void myled_task(void *pvParameters)
{
    uint32_t toggle_count = 0;
    for (;;)
    {
       myled_toggle();
       toggle_count++;
       data_txrx.led_toggle_count = toggle_count;
       data_txrx.task_id = LED_TASK;
       strcpy(data_txrx.name, "Tanmay Chaturvedi");
       TickType_t time = (xTaskGetTickCount()/portTICK_PERIOD_MS);
       data_txrx.timestamp = time;
       if(xQueueSend(myqueuehandle, &data_txrx , 20))
       {
           //UARTprintf("Data sent = %d\n\r", data_txrx.led_toggle_count);
       }
       else
           UARTprintf("Not able to send");

    }
}


void myled_toggle(void)
{
    GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, CLP_D1_PIN);
    GPIOPinWrite(CLP_D2_PORT, CLP_D2_PIN, CLP_D2_PIN);
    vTaskDelay(50/portTICK_PERIOD_MS);

    // Turn off LED 1,2
    GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, 0);
    GPIOPinWrite(CLP_D2_PORT, CLP_D2_PIN, 0);
    vTaskDelay(50/portTICK_PERIOD_MS);
}
