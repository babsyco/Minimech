#ifndef MM_LIB_H
#define MM_LIB_H

#include <stdint.h>

/**********************************************************************************
 * @File     MM_lib.h
 * @AUthor   Daniel Babekuhl
 * @Date     7th June 2020
 * @Brief    This file contains the main software for the MiniMech robot.
 **********************************************************************************
 * This library provides functions and variables for the MiniMech
 * main software and hardware modules to interface and function.
 **********************************************************************************/

// controller signal states
typedef enum {
    MM_STATIC,
    MM_FORWARD,
    MM_LEFT,
    MM_RIGHT,
    MM_SWITCH,
} MM_controller_state;

// Main controller signal state variable (set by external device, 
// ie bluetooth)
extern MM_controller_state MM_CONTROL;

// motors
typedef enum {
    MM_MOTOR_L,
    MM_MOTOR_R
} MM_motor;

// states of motors
typedef enum {
    MM_MOTOR_OFF,
    MM_MOTOR_ON
} MM_motor_state;

// LEDs
typedef enum {
    MM_LED_GREEN,
    MM_LED_BLUE,
    MM_LED_ORANGE,
    MM_LED_RED,
} MM_led;

//LED states 
typedef enum {
    MM_LED_OFF,
    MM_LED_ON,
} MM_led_state;


// Store phrases to be sent to text-to-speech module. 249 max chars + null byte.
extern char MM_PHRASES[10][250];
// phrase array index
extern uint8_t MM_PHR_INDEX;
// number of phrases entered
extern uint8_t MM_NUM_PHRASES;

// pass a random phrase from MM_PHRASES to 
// text to speech module
void MM_say_rand_phrase(void);

#endif