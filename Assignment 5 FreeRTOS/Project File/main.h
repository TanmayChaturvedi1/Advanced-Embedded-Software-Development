/*
 * main.h
 *
 *  Created on: Mar 28, 2015
 *      Author: akobyljanec
 */

#ifndef MAIN_H_
#define MAIN_H_


#include <stdint.h>

#include "portmacro.h"

// System clock rate, 120 MHz
#define SYSTEM_CLOCK    (120000000U)
#define TEMP_TH         (29.0)  //for Extra Credits, Temperature Alert Mechanism

/***************************************************************************************
*                   ENUMS
****************************************************************************************/
typedef enum
{
    TASK_CREATE_SUCCESS = 0,
    TASK_CREATE_FAILED,
    TEMP_TASK_CREATE_FAILED,
    LED_TASK_CREATE_FAILED,
    LOGGER_TASK_CREATE_FAILED,
    ALERT_TASK_CREATE_FAILED,
    PROGRAM_SUCCESS
}return_type_t;

typedef enum
{
    TEMP_TASK = 1,
    LED_TASK,
    ALERT_TASK

}task_id_t;



typedef struct logger_data
{
    float       temperature;
    uint32_t    led_toggle_count;
    char        name[30];
    TickType_t  timestamp;
    task_id_t   task_id;
}logger_data_t;



#endif /* MAIN_H_ */
