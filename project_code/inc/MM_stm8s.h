#ifndef MM_STM8S_H
#define MM_STM8S_H

#include <stdint.h>
#include <stm8s.h>
#include <MM_lib.h>

/*********************************************************************************
 * @File     MM_stm8s.h
 * @AUthor   Daniel Babekuhl
 * @Date     7th June 2020
 * @Brief    This file contains the main software for the MiniMech robot.
 *********************************************************************************
 * This file contains functions used by the MiniMiech software 
 * to inferface with the SPM8S Standard Peripheral Library. 
 * It is designed for use with the SPM8S208CB microcontroller.
 **********************************************************************************/

void MM_MCU_init(void);
void MM_MCU_delay(__IO uint32_t ms);
void MM_MCU_sendByte(unsigned char byte, char* module);
char MM_MCU_recvByte(char * module);
void MM_MCU_setLED(MM_led MM_LED_COLOUR, MM_led_state MM_STATE);
void MM_MCU_setMotor(MM_motor MM_MOTOR, MM_motor_state MM_STATE);

#endif