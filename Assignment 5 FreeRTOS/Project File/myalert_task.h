/*
 * myalert_task.h
 *
 *  Created on: 10 Apr 2019
 *      Author: TanmayC
 */

#ifndef MYALERT_TASK_H_
#define MYALERT_TASK_H_


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

void myalert_task(void *pvParameters);




#endif /* MYALERT_TASK_H_ */
