#include <stdio.h>
#include <stdlib.h>
#include "MM_lib.h"
#include "MM_bt_hc06.h"
// library for text-to-speech module
#include "MM_t2s_xfs5152.h"

/**********************************************************************************
 * @File     MM_lib.c
 * @AUthor   Daniel Babekuhl
 * @Date     7th June 2020
 * @Brief    This file contains the main software for the MiniMech robot.
 **********************************************************************************
 * This library provides functions and variables for the MiniMech
 * main software and hardware modules to interface and function.
 **********************************************************************************/


// Define variables from header file
MM_controller_state MM_CONTROL = MM_STATIC;
// contains phrases to be spoken by T2S converter
char MM_PHRASES[10][250];
// phrase array index
uint8_t MM_PHR_INDEX = 0;
// number of phrases entered
uint8_t MM_NUM_PHRASES = 0;


/*
 * Pass a random phrase from MM_PHRASES to text to speech module.
 */
void MM_say_rand_phrase(void) {
    MM_PHR_INDEX = rand() % MM_NUM_PHRASES;
    MM_T2S_sendPhrase();
}