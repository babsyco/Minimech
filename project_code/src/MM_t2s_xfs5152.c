#include <MM_stm8s.h>
#include <MM_t2s_xfs5152.h>
#include <string.h>


/**********************************************************************************
 * @File     MM_t2s_xfs5152.c
 * @AUthor   Daniel Babekuhl
 * @Date     7th June 2020
 * @Brief    This file contains a library for the XFS5152 text-to-speech module 
 *           to interface with the MiniMech main software.
 * ********************************************************************************
 * Summary of Module Operation:
 * 
 * Command format (send order of bytes):
 *  0xFD -> start of command
 *  0xXX 0xXX -> size of command (ie, all bytes that follow)
 *  0xXX -> command. 0x01 means talk, 0x02 means stop talking, 
 *          0x21 means get current status (will return 0x4E for busy,
 *          or 0x4F for idle/finished talking).
 *  0xXX -> encoding. Best for english is GB2312 = 0x00
 * data -> string to be spoken. best to include "[g2]" at the beginning to aid with
 *         english translation. 
 * 
 * Note: when determining length of command, can use (strlen("<phrase>") + 6).
 * The + 6 is for command byte, encoding byte, "[g2]" (without null terminator)
 * 
 * Unofficial tutorial found at: https://www.youtube.com/watch?v=kuBG0U6X7Jw
 **********************************************************************************/

/*
 * Variables for use in this library. SDCC compiler requires these be declared 
 * outside functions.
 */
static char retval = '\0';
static char c = '\0';
static uint8_t com_len = 0;
static uint8_t i = 0;

/*
 * Initialise module by checking status and confirming it is idle.
 */
void MM_T2S_init(void){
    
    retval = '\0';
    while (retval != 0x4F) {
        // get current status
    MM_MCU_sendByte(0xFD, "T2S"); // start command
    MM_MCU_sendByte(0x02, "T2S"); // size of command
    MM_MCU_sendByte(0x01, "T2S");
    MM_MCU_sendByte(0x21, "T2S"); //command: get current status

    retval = MM_MCU_recvByte("T2S"); // 0x4F means idle
    }
}

/*
 * Send a phrase to module. Max length is 249 chars (not including null)
 * but this is controlled in bluetooth library function MM_BT_getPhrase().
 * Phrase sent is MM_PHRASES[MM_PHR_INDEX], declared in MM_lib.c and determined
 * in MM_main.c
 */
void MM_T2S_sendPhrase(void){
    // + 6 because of command byte, encoding byte, and "[g2]" 
    // (without null terminator). Will have a max value of 255 total.
    com_len = (unsigned char)(strlen(MM_PHRASES[MM_PHR_INDEX]) + 6);
    
    // message header
    MM_MCU_sendByte(0xFD, "T2S"); // start command
    MM_MCU_sendByte(0x00, "T2S"); // size of command byte 1
    MM_MCU_sendByte(com_len, "T2S"); // size of command byte 2
    MM_MCU_sendByte(0x01, "T2S"); // command: talk
    MM_MCU_sendByte('[', "T2S"); // English format aid
    MM_MCU_sendByte('g', "T2S");
    MM_MCU_sendByte('2', "T2S");
    MM_MCU_sendByte(']', "T2S");
    
    // send phrase
    c = MM_PHRASES[MM_PHR_INDEX][0];
    i = 0;
    while (c != '\0') {
        MM_MCU_sendByte(MM_PHRASES[MM_PHR_INDEX][i], "T2S");
        i++;
    }
}

// get status from T2S module. Returns 1 if busy, 0 if idle   
uint8_t MM_T2S_getStatus(void) {
    
    retval = 0x00;
    // send get status command
    MM_MCU_sendByte(0xFD, "T2S"); // start command
    MM_MCU_sendByte(0x00, "T2S"); // size of command byte 1
    MM_MCU_sendByte(0x01, "T2S"); // size of command byte 2
    MM_MCU_sendByte(0x21, "T2S"); // command: get status

    if(MM_MCU_recvByte("T2S") == 0x4F) {
        return 0;
    }
    else return 1;  
}

/*
 * Stop phrase and await confirmation that module has returned to idle
 * state.
 */
void MM_T2S_stopPhrase(void){
    // wait until speech finishes
    retval = '\0';
    while(retval != '0x4F') {
        MM_MCU_sendByte(0xFD, "T2S"); // start command
        MM_MCU_sendByte(0x00, "T2S"); // size of command byte 1
        MM_MCU_sendByte(0x01, "T2S"); // size of command byte 2
        MM_MCU_sendByte(0x02, "T2S"); // command: stop talking

        retval = MM_MCU_recvByte("T2S"); //0x4F means idle
    }
}