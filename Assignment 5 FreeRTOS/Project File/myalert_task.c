/*
 * myalert_task.c
 *
 *  Created on: 10 Apr 2019
 *      Author: TanmayC
 */

#include "myalert_task.h"

logger_data_t data_txrx;
extern QueueHandle_t myqueuehandle;

void myalert_task(void *pvParameters)
{
    uint32_t ulNotified_val;

    for (;;)
    {
        xTaskNotifyWait(0x01, 0x00, &ulNotified_val, portMAX_DELAY);
//
//        if( (ulNotified_val & 0x01 ) != 0 )
//        {
//            UARTprintf("Notification Not Checked!\n\n");
            data_txrx.task_id = ALERT_TASK;
            TickType_t time = (xTaskGetTickCount()/portTICK_PERIOD_MS);
            data_txrx.timestamp = time;
      if(xQueueSend(myqueuehandle, &data_txrx , 100))
      {

      }
      else
      {
              UARTprintf("Not able to send");
      }

    }

}



