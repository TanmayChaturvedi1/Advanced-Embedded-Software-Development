/*
 * myi2c.c
 *
 *  Created on: 9 Apr 2019
 *      Author: TanmayC
 *
 *      Reference: https://www.digikey.com/eewiki/display/microcontroller/I2C+Communication+with+the+TI+Tiva+TM4C123GXL
 */

#include "myi2c.h"
#include <stdint.h>

extern uint32_t output_clock_rate_hz;

void myi2cinit(void)
{
    //enable module 0 of I2C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //Perform a software reset on the I2C periph
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    //enable GPIO peripheral that contains I2C 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);

    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);

    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    //Initializes the I2C Master block.
    I2CMasterInitExpClk(I2C0_BASE, output_clock_rate_hz, false);

    //clear I2C FIFOs
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;

}


uint32_t myi2creceive(uint8_t slave_addr, uint8_t reg)
{
    uint32_t msb, lsb, temperature;
    //"false" arg means write operation.
    //Specify that we are setting up register address in the slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //Specify the register to be read
    I2CMasterDataPut(I2C0_BASE, reg);

    //Controls the state of the I2C Master
    //Send control byte and register address byte to slave address
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    //poll for busy flag and wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    //Send control bit to specify read operation from the reg. "read" is "true"
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);

    //Controls the state of the I2C Master
    //Send control byte (for read) and register address byte to slave address
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //poll for busy flag and wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    msb = I2CMasterDataGet(I2C0_BASE);

    //Controls the state of the I2C Master
    //Finish reading the second byte of the data
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);

    //poll for busy flag and wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    lsb = I2CMasterDataGet(I2C0_BASE);

    temperature = (msb << 8) | lsb ;
    temperature >>= 4;

    return(temperature);

}

