#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <MM_lib.h>
#include <stdint.h>
#include <MM_bt_hc06.h>
#include <MM_stm8s.h>

/*********************************************************************************
 * @File     MM_bt_hc06.h
 * @AUthor   Daniel Babekuhl
 * @Date     7th June 2020
 * @Brief    This file contains the main software for the MiniMech robot.
 *********************************************************************************
 * This library is for connecting the MiniMech software to the MiniMech 
 * Smartphone app via the HC-06 Bluetooth module. It requires the use MM_lib.h 
 * library. It defines and declares the standard bluetooth functions used in
 * MiniMech software.
 *********************************************************************************/

/*
 * Connect to app via bluetooth. Returns 1 on success.
 */
uint8_t MM_BT_init(void) {

    // Use AT command to signal module is connected
    char buf [2];
    while ((buf[0] != 'O') || (buf[1] != 'K')) {
        // send AT
        MM_MCU_sendByte('A', "BT");
        MM_MCU_sendByte('T', "BT");
        // wait for OK
        buf[0] = MM_MCU_recvByte("BT");
        buf[1] = MM_MCU_recvByte("BT");
    }
    return 1;
}

/*
 * Get phrase from app via bluetooth. Returns 1 if a phrase has been acquired,
 * 0 when last phrase has been recieved, signaled by a phrase of "\0" from app.
 * 0 is then returned. Maximum phrase length = 280 chars.
 */
 uint8_t MM_BT_getPhrase(void) {

     uint16_t char_idx = 0;
     // get 1st char
     char buf = MM_MCU_recvByte("BT");
     
    // check to see if this is signal for last phrase
    if (buf == '\0') {
        return 0;
    }

    // put string into MM_PHRASES buffer
    while ((char_idx < 249) && (buf != '\0')) {
         MM_PHRASES[MM_PHR_INDEX][char_idx] = buf;
         buf = MM_MCU_recvByte("BT");
         char_idx++;
    }
    //add null byte to string in MM_PHRASES buffer
    MM_PHRASES[MM_PHR_INDEX][char_idx] = '\0';
    return 1;
 }
 
 /**
  * Acquires X, Y, Z values form accelerometer from app via bluetooth. 
  * Data sent as a single char in the following format:
  *   X-axis = left/right = 3rd LSB (1 = left, 0 = right)
  *   Y-axis = forward/steer = 2rd LSB (1 = forward, 0 = steer)
  *   Z-axis = jerk downwards = LSB (1 = jerk, 0 = not jerked)
  * This data is used to set MM_CONTROL (located in MM_lib.h)
  */
 void MM_BT_getXYZ(void) {
    // get data
    char data = MM_MCU_recvByte("BT");
    // extract XYZ values
    uint8_t x_val = data && (1 << 2);
    uint8_t y_val = data && (1 << 1);
    uint8_t z_val = data && 1;

    // set MM_CONTROL (order is important)
    // switch in/out of speak mode
    if (z_val == 1) {
        MM_CONTROL = MM_SWITCH;
    }
    // move forward
    else if (y_val == 1) {
        MM_CONTROL = MM_FORWARD;
    }
    // steer left
    else if (x_val == 1) {
        MM_CONTROL = MM_LEFT;
    }
    // steer right
    else {
        MM_CONTROL = MM_RIGHT;
    }
 }

