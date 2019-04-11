/*
 * mytemp_task.c
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
#include "mylogger_task.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "myi2c.h"


logger_data_t data_txrx;
extern QueueHandle_t myqueuehandle;
extern BaseType_t xAlertHandle;
extern TaskHandle_t xAlertTaskHandle;

void mytemp_task(void *pvParameters)
{
    uint32_t temperature_value;
    float final_temperature;

    for (;;)
    {
        temperature_value = myi2creceive(SLAVE_ADDR, DEVICE_REG);
        //UARTprintf("dojjne\n");
        if(temperature_value & 0x800)    //if Signed bit (MSB) is high, means negative temperature
        {
//            uint32_t new_temp = ~(temperature_value) + 1;
//            final_temperature = -1 * new_temp * 0.0625;
        }
        else
        {
            final_temperature = temperature_value * 0.0625;
        }

        data_txrx.temperature = final_temperature;

        if(data_txrx.temperature >= TEMP_TH)
        {
            xTaskNotify(xAlertTaskHandle, 0x01, eSetBits);
            if(xQueueSend(myqueuehandle, &data_txrx , 100))
            {

            }
            else
            {
                UARTprintf("Not able to send");
            }
        }
        else
        {
            data_txrx.task_id = TEMP_TASK;
            TickType_t time = (xTaskGetTickCount()*portTICK_PERIOD_MS);
            data_txrx.timestamp = time;
            if(xQueueSend(myqueuehandle, &data_txrx , 100))
            {

            }
            else
            {
                UARTprintf("Not able to send");
            }
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);

    }
}



