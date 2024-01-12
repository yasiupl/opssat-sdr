/** MAX7311 IC Device Library
 * 
 * \file MAX7311.h
 *
 * MAX7311 16-bit bus expander library for I2C device communication.
 * The library contains the device register map and commands for register access.
 * 
 * \project    OPS-SAT Project
 *
 * \copyright  Graz University of Technology
 * \date       15. June 2016
 * \author     Reinhard Zeif, MSc
 * \version    1.0
 *
 *(C) 2018 Graz University of Technology / IKS. All rights reserved.
 * Your use of TU Graz design tools, logic functions and other 
 * software and tools and any output files any of the foregoing 
 * (including device programming or simulation files), and any associated 
 * documentation or information are expressly subject to the terms and 
 * conditions of the applicable license agreement, including, without 
 * limitation, that your use is for the sole purpose of programming the 
 * OPS-SAT processing platform.
 *
 **/
 
#ifndef MAX7311_H_
#define MAX7311_H_

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "sepp_i2c_dev.h"


/* Register Map Definitions for MAX7311 */
#define         MAX7311_REG_P1IN               0x00        // Input Register Port 1
#define         MAX7311_MASK_P1IN_7            0x80        // Input logic levels (if port is set to input) of I/O pin 7
#define         MAX7311_MASK_P1IN_6            0x40        // Input logic levels (if port is set to input) of I/O pin 6
#define         MAX7311_MASK_P1IN_5            0x20        // Input logic levels (if port is set to input) of I/O pin 5
#define         MAX7311_MASK_P1IN_4            0x10        // Input logic levels (if port is set to input) of I/O pin 4
#define         MAX7311_MASK_P1IN_3            0x08        // Input logic levels (if port is set to input) of I/O pin 3
#define         MAX7311_MASK_P1IN_2            0x04        // Input logic levels (if port is set to input) of I/O pin 2
#define         MAX7311_MASK_P1IN_1            0x02        // Input logic levels (if port is set to input) of I/O pin 1
#define         MAX7311_MASK_P1IN_0            0x01        // Input logic levels (if port is set to input) of I/O pin 0
#define         MAX7311_REG_P2IN               0x01        // Input Register Port 2
#define         MAX7311_MASK_P2IN_15           0x80        // Input logic levels (if port is set to input) of I/O pin 15
#define         MAX7311_MASK_P2IN_14           0x40        // Input logic levels (if port is set to input) of I/O pin 14
#define         MAX7311_MASK_P2IN_13           0x20        // Input logic levels (if port is set to input) of I/O pin 13
#define         MAX7311_MASK_P2IN_12           0x10        // Input logic levels (if port is set to input) of I/O pin 12
#define         MAX7311_MASK_P2IN_11           0x08        // Input logic levels (if port is set to input) of I/O pin 11
#define         MAX7311_MASK_P2IN_10           0x04        // Input logic levels (if port is set to input) of I/O pin 10
#define         MAX7311_MASK_P2IN_9            0x02        // Input logic levels (if port is set to input) of I/O pin 9
#define         MAX7311_MASK_P2IN_8            0x01        // Input logic levels (if port is set to input) of I/O pin 8
#define         MAX7311_REG_P1OUT              0x02        // Output Register Port 1
#define         MAX7311_MASK_P1OUT_7           0x80        // Output logic levels (if port is set to output) of I/O pin 7
#define         MAX7311_MASK_P1OUT_6           0x40        // Output logic levels (if port is set to output) of I/O pin 6
#define         MAX7311_MASK_P1OUT_5           0x20        // Output logic levels (if port is set to output) of I/O pin 5
#define         MAX7311_MASK_P1OUT_4           0x10        // Output logic levels (if port is set to output) of I/O pin 4
#define         MAX7311_MASK_P1OUT_3           0x08        // Output logic levels (if port is set to output) of I/O pin 3
#define         MAX7311_MASK_P1OUT_2           0x04        // Output logic levels (if port is set to output) of I/O pin 2
#define         MAX7311_MASK_P1OUT_1           0x02        // Output logic levels (if port is set to output) of I/O pin 1
#define         MAX7311_MASK_P1OUT_0           0x01        // Output logic levels (if port is set to output) of I/O pin 0
#define         MAX7311_REG_P2OUT              0x03        // Output Register Port 2
#define         MAX7311_MASK_P2OUT_15          0x80        // Output logic levels (if port is set to output) of I/O pin 15
#define         MAX7311_MASK_P2OUT_14          0x40        // Output logic levels (if port is set to output) of I/O pin 14
#define         MAX7311_MASK_P2OUT_13          0x20        // Output logic levels (if port is set to output) of I/O pin 13
#define         MAX7311_MASK_P2OUT_12          0x10        // Output logic levels (if port is set to output) of I/O pin 12
#define         MAX7311_MASK_P2OUT_11          0x08        // Output logic levels (if port is set to output) of I/O pin 11
#define         MAX7311_MASK_P2OUT_10          0x04        // Output logic levels (if port is set to output) of I/O pin 10
#define         MAX7311_MASK_P2OUT_9           0x02        // Output logic levels (if port is set to output) of I/O pin 9
#define         MAX7311_MASK_P2OUT_8           0x01        // Output logic levels (if port is set to output) of I/O pin 8
#define         MAX7311_REG_P1POLINV           0x04        // Polarity Inversion Register Port 1
#define         MAX7311_MASK_P1POLINV_7        0x80        // Polarity inversion (if port is set to input) of I/O pin 7,1 = invert, 0 = no inversion
#define         MAX7311_MASK_P1POLINV_6        0x40        // Polarity inversion (if port is set to input) of I/O pin 6, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P1POLINV_5        0x20        // Polarity inversion (if port is set to input) of I/O pin 5, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P1POLINV_4        0x10        // Polarity inversion (if port is set to input) of I/O pin 4, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P1POLINV_3        0x08        // Polarity inversion (if port is set to input) of I/O pin 3, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P1POLINV_2        0x04        // Polarity inversion (if port is set to input) of I/O pin 2, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P1POLINV_1        0x02        // Polarity inversion (if port is set to input) of I/O pin 1, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P1POLINV_0        0x01        // Polarity inversion (if port is set to input) of I/O pin 0, 1 = invert, 0 = no inversion
#define         MAX7311_REG_P2POLINV           0x05        // Polarity Inversion Register Port 2
#define         MAX7311_MASK_P2POLINV_15       0x80        // Polarity inversion (if port is set to input) of I/O pin 15, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P2POLINV_14       0x40        // Polarity inversion (if port is set to input) of I/O pin 14, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P2POLINV_13       0x20        // Polarity inversion (if port is set to input) of I/O pin 13, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P2POLINV_12       0x10        // Polarity inversion (if port is set to input) of I/O pin 12, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P2POLINV_11       0x08        // Polarity inversion (if port is set to input) of I/O pin 11, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P2POLINV_10       0x04        // Polarity inversion (if port is set to input) of I/O pin 10, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P2POLINV_9        0x02        // Polarity inversion (if port is set to input) of I/O pin 9, 1 = invert, 0 = no inversion
#define         MAX7311_MASK_P2POLINV_8        0x01        // Polarity inversion (if port is set to input) of I/O pin 8, 1 = invert, 0 = no inversion
#define         MAX7311_REG_P1CFG              0x06        // Configuration Register Port 1
#define         MAX7311_MASK_P1CFG_7           0x80        // Port 1 Configuration Register I/O pin 7, 1 = input, 0 = output
#define         MAX7311_MASK_P1CFG_6           0x40        // Port 1 Configuration Register I/O pin 6, 1 = input, 0 = output
#define         MAX7311_MASK_P1CFG_5           0x20        // Port 1 Configuration Register I/O pin 5, 1 = input, 0 = output
#define         MAX7311_MASK_P1CFG_4           0x10        // Port 1 Configuration Register I/O pin 4, 1 = input, 0 = output
#define         MAX7311_MASK_P1CFG_3           0x08        // Port 1 Configuration Register I/O pin 3, 1 = input, 0 = output
#define         MAX7311_MASK_P1CFG_2           0x04        // Port 1 Configuration Register I/O pin 2, 1 = input, 0 = output
#define         MAX7311_MASK_P1CFG_1           0x02        // Port 1 Configuration Register I/O pin 1, 1 = input, 0 = output
#define         MAX7311_MASK_P1CFG_0           0x01        // Port 1 Configuration Register I/O pin 0, 1 = input, 0 = output
#define         MAX7311_REG_P2CFG              0x07        // Configuration Register Port 2
#define         MAX7311_MASK_P2CFG_15          0x80        // Port 1 Configuration Register I/O pin 15, 1 = input, 0 = output
#define         MAX7311_MASK_P2CFG_14          0x40        // Port 1 Configuration Register I/O pin 14, 1 = input, 0 = output
#define         MAX7311_MASK_P2CFG_13          0x20        // Port 1 Configuration Register I/O pin 13, 1 = input, 0 = output
#define         MAX7311_MASK_P2CFG_12          0x10        // Port 1 Configuration Register I/O pin 12, 1 = input, 0 = output
#define         MAX7311_MASK_P2CFG_11          0x08        // Port 1 Configuration Register I/O pin 11, 1 = input, 0 = output
#define         MAX7311_MASK_P2CFG_10          0x04        // Port 1 Configuration Register I/O pin 10, 1 = input, 0 = output
#define         MAX7311_MASK_P2CFG_9           0x02        // Port 1 Configuration Register I/O pin 9, 1 = input, 0 = output
#define         MAX7311_MASK_P2CFG_8           0x01        // Port 1 Configuration Register I/O pin 8, 1 = input, 0 = output
#define         MAX7311_REG_TIMEOUT            0x08        // Timeout Register

/** MAX7311 device class.
* 	Contains members for device address and general information. 
*	  Set and Get functions for device address, description and interface type.
* 	Methods to generate correct message sequences for write and read commands.
*/

class MAX7311 : public SEPP_I2C_DEV
{
	private:

		// SEPP_I2C_DEV *i2c_dev_;       // i2c device file handler
		unsigned int cfg_pin_masks_[16];
		unsigned int out_pin_masks_[16];
		unsigned int in_pin_masks_[16];
		unsigned int polinv_pin_masks_[16];
		
	public:
		MAX7311();
		MAX7311(unsigned int i2c_address, std::string i2c_device_name);
		virtual ~MAX7311();

		/* public Methods for Device Configuration */
		void Connect(unsigned int i2c_address, std::string i2c_device_name);
		void Disconnect();	
		void PrintDeviceInfo();                               //!< Print device summary / info
		
		void WriteRegisterWord(unsigned int reg_addr, unsigned int reg_value);
		unsigned int ReadRegisterWord(unsigned int reg_addr);
   
		/* ----- Custom user functions for device ----- */
		 
		void Set_IOPort_Direction(unsigned int direction_mask);
		unsigned int Get_IOPort_Direction();
		void Set_IOPort_PolarityInversion(unsigned int inversion_mask);
		unsigned int Get_IOPort_PolarityInversion();
		void Set_IOPort_OutputValue(unsigned int output_mask);
		unsigned int Get_IOPort_OutputValue();
		//void Set_IOPortInputValue(unsigned int output_mask);
		unsigned int Get_IOPort_InputValue();

    void Set_IOPin_Direction(unsigned int pin_nr, unsigned int pin_direction);
    unsigned int Get_IOPin_Direction(unsigned int pin_nr);
    void Set_IOPin_PolarityInversion(unsigned int pin_nr, unsigned int pin_inversion);
    unsigned int Get_IOPin_PolarityInversion(unsigned int pin_nr);
    void Set_IOPin_OutputValue(unsigned int pin_nr, unsigned int pin_value);
		unsigned int Get_IOPin_OutputValue(unsigned int pin_nr);
		//void Set_IOPinInputValue(unsigned int pin_nr, unsigned int pin_value);
    unsigned int Get_IOPin_InputValue(unsigned int pin_nr);


};

/** MAX7311 exception class.
* 	This exception type is returned in case of an exception in the MAX7311 device class.
*/
class MAX7311_Exception : public std::exception
{
  private:
	std::string e_;
	
  public:
	MAX7311_Exception(std::string e) : e_(e) {}
	~MAX7311_Exception() throw () {}
	const char* what() const throw() { return e_.c_str(); }
};

#endif
