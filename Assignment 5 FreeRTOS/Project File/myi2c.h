/*
 * myi2c.h
 *
 *  Created on: 9 Apr 2019
 *      Author: TanmayC
 */

#ifndef MYI2C_H_
#define MYI2C_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#define SLAVE_ADDR  (0x48)
#define DEVICE_REG  (0x00)


void myi2cinit(void);
uint32_t myi2creceive(uint8_t slave_addr, uint8_t reg);


#endif /* MYI2C_H_ */
