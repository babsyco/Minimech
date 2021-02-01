#ifndef MM_BLUETOOTH_H
#define MM_BLUETOOTH_H

#include <stdint.h>
#include <MM_stm8s.h>

/**********************************************************************************
 * @File     MM_bt_hc06.h
 * @AUthor   Daniel Babekuhl
 * @Date     7th June 2020
 * @Brief    This file contains the main software for the MiniMech robot.
 *********************************************************************************
 * This library is for connecting the MiniMech software to the MiniMech 
 * Smartphone app via the HC-06 Bluetooth module. It requires the use MM_lib.h 
 * library. It defines and declares the standard bluetooth functions used in
 * MiniMech software.
 *****************************************************************************
 */

uint8_t MM_BT_init(void);
uint8_t MM_BT_getPhrase(void);
void MM_BT_getXYZ(void);

#endif


