/**********************************************************************************
 * @File     MM_main.c
 * @AUthor   Daniel Babekuhl
 * @Date     7th June 2020
 * @Brief    This file contains the main software for the MiniMech robot.
 * ********************************************************************************
 * 
 * This file contains the main software for the MiniMech robot.
 * It is hardware-independent, but requires:
 * 
 *  1) an MCU to run on, with at least 6 GPIO pins and 2 UARTS.
 *  2) a bluetooth module (connected via UART)
 *  3) a text-to-speech (T2S) converter (connected via UART)
 *  4) control over 4 LEDs (via 4/6 GPIO pins)
 *  5) control over 2 motors (via 2/6 GPIO pins)
 * 
 *  UART specifications: BAUD = 9600, 8-N-1 format
 * 
 * The software contained in this file requires hardware functions that must 
 * be supplied by purpose-built MiniMech libraries for each module. Each of 
 * these will also require the MM_lib.h library, and it is assumed each 
 * hardware module library will have access to the functions below from the 
 * the all other hardware module libraries. The required functions are:
 * 
 *  MCU functions:
 *       // Configure clock, GPIOs, UARTS, anything else required by
 *       // MCU to operate. 
 *       void MM_MCU_init(void);
 * 
 *      // Standard Delay, max value required: 1000ms
 *      void MM_MCU_delay(__IO uint32_t ms);
 * 
 *      // send a single byte over UART. module = "BT" or "T2S"
 *      void MM_MCU_sendByte(char byte, char * module);
 * 
 *      // recieve a single byte from UART (stored at byte). 
 *      // module = "BT" or "T2S"
 *      char MM_MCU_recvByte(char* module);
 * 
 *      // turn LED on or off (arg types declared in MM_lib.h)
 *      void MM_MCU_setLED(MM_led MM_LED_COLOUR, MM_led_state MM_STATE);
 *      
 *      // turn motor on or off (arg types declared in MM_lib.h)
 *      void MM_MCU_motor(MM_motor MM_MOTOR, MM_motor_state MM_STATE);
 * 
 * Bluetooth functions:
 *      // initialise and connect with bluetooth module. Return 1 on success.
 *      uint8_t MM_BT_init(void);
 * 
 *      // Get a phrase from bluetooth module, store it in 
 *      // MM_PHRASES[MM_PHR_INDEX]. Max return length = 249 chars. Returns 
 *      // 1 on success. Returns 0 if there are no more phrases to get.
 *      uint8_t MM_BT_getPhrase(void);
 * 
 *      // recieve XYZ values via bluetooth and update MM_CONTROL variable
 *      // accordingly. XYZ values are sent from MiniMech phone app in a 
 *      // single byte with the following format:
 *      //                      MSB  00000XYZ  LSB
 *      // Control flow:
 *      // if (Z == 1) MM_CONTROL = MM_SWITCH; 
 *      // else if (Y == 1) MM_CONTORL = MM_FORWARD;
 *      // else if (X == 1) MM_CONTORL = MM_LEFT;
 *      // else if (y == 1) MM_CONTORL =MM_RIGHT;
 *      void MM_BT_getXYZ();
 * 
 * Text-to-Speech functions:
 *      // initialise and connect to module. Return 1 on success.
 *      uint8_t MM_T2S_init();
 * 
 *      // send phrase to module, wait till phrase is finished
 *      void MM_T2S_sendPhrase(char* phrase);
 * 
 *      // get status from T2S module. Returns 1 if busy, 0 if idle   
 *      uint8_t MM_T2S_getStatus(void);
 *
 *      // stop saying phrase
 *       void MM_T2S_stopPhrase(void);
 * 
 ********************************************************************************* 
 * This software is originally designed for:
 *      MCU: STM8S205CB
 *      Bluetooth module: HC-06
 *      Text-to-speech module: XFS5152
 * 
 *********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
// MiniMech standard library:
#include <MM_lib.h>
// MiniMech Hardware libraries:
#include <MM_bt_hc06.h>
#include <MM_stm8s.h>
#include <MM_t2s_xfs5152.h>

// flag to enable phrase to be sent once per SPEAK state
uint8_t MM_speak_flag = 0;


void MM_state_machine(void);
// Required for compiler. Defined in MM_lib.c
//extern char MM_PHRASES[10][281];

// Various states of FSM
typedef enum {
    STARTUP,
    PHRASE,
    STEER,
    MOVE,
    SPEAK,
} _state;

// Main state variable of machine
_state STATE = STARTUP;

// FSM for MiniMech
void MM_state_machine(void) {
    
    switch (STATE) {
        // configure MCU, init bluetooth and T2S modules
        case STARTUP :
            // set control to static (only used in STARTUP state)
            MM_CONTROL = MM_STATIC;
            // initalise MCU
            MM_MCU_init();
            // state LED config
            MM_MCU_setLED(MM_LED_RED, MM_LED_ON);
            // initialise BT and T2S modules
            MM_BT_init();
            MM_T2S_init();
            // exit state
            STATE = PHRASE;
            // state LED deconfig
            MM_MCU_setLED(MM_LED_RED, MM_LED_OFF);
            break;
        case PHRASE :
            // state LED config
            MM_MCU_setLED(MM_LED_RED, MM_LED_ON);
            MM_MCU_setLED(MM_LED_ORANGE, MM_LED_ON);
            // get phrases
            MM_PHR_INDEX = 0;
            while(((MM_PHR_INDEX < 10) && MM_BT_getPhrase())) {
                MM_PHR_INDEX++;
                MM_NUM_PHRASES++;
            }
            // state LED deconfig
            MM_MCU_setLED(MM_LED_RED, MM_LED_OFF);
            MM_MCU_setLED(MM_LED_ORANGE, MM_LED_OFF);
            STATE = STEER;
            break;
        case STEER :
            // switch states if necessary
            if (MM_CONTROL == MM_SWITCH) {
                STATE = SPEAK;
                MM_MCU_setLED(MM_LED_ORANGE, MM_LED_OFF);
                break;
            }
            if (MM_CONTROL == MM_FORWARD) {
                STATE = MOVE;
                MM_MCU_setLED(MM_LED_ORANGE, MM_LED_OFF);
                break;
            }
            // steer mode actions
            // state LED config
            MM_MCU_setLED(MM_LED_ORANGE, MM_LED_ON);
            // steer left
            if (MM_CONTROL == MM_LEFT) {
                MM_MCU_setMotor(MM_MOTOR_L, MM_MOTOR_OFF);
                MM_MCU_setMotor(MM_MOTOR_R, MM_MOTOR_ON);    
            }
            // steer right
            if (MM_CONTROL == MM_RIGHT) {
                MM_MCU_setMotor(MM_MOTOR_L, MM_MOTOR_ON);
                MM_MCU_setMotor(MM_MOTOR_R, MM_MOTOR_OFF);    
            }
            break;
        case MOVE :
            // switch states if necessary
            if (MM_CONTROL == MM_SWITCH) {
                STATE = SPEAK;
                MM_MCU_setLED(MM_LED_GREEN, MM_LED_OFF);
                break;
            }
            if ((MM_CONTROL == MM_LEFT) || (MM_CONTROL == MM_RIGHT)) {
                STATE = STEER;
                MM_MCU_setLED(MM_LED_GREEN, MM_LED_OFF);
                break;
            }        
            // move mode actions
            // state LED config
            MM_MCU_setLED(MM_LED_GREEN, MM_LED_ON);
            // move forward
            MM_MCU_setMotor(MM_MOTOR_L, MM_MOTOR_ON);
            MM_MCU_setMotor(MM_MOTOR_R, MM_MOTOR_ON);    
            break;    
        case SPEAK :
            // ensure switch setting is not triggered by same movement
            MM_MCU_delay(500);
            // switch states if necessary
            if (MM_CONTROL == MM_SWITCH) {
                MM_T2S_stopPhrase();
                STATE = STEER;
                //reset flag on exit
                MM_speak_flag = 0;
                MM_MCU_setLED(MM_LED_BLUE, MM_LED_OFF);
                break;
            }
            // speak mode actions
            MM_MCU_setLED(MM_LED_BLUE, MM_LED_ON);
            // tell text to speech module to say a random phrase
            MM_MCU_delay(500);
            // prevent phrase being resent until next time
            // SPEAK state is entered
            if (MM_speak_flag == 0) {
                MM_T2S_sendPhrase();
                MM_speak_flag = 1;
            }
            // checks if phrase has finished
            if(!MM_T2S_getStatus()) {
                STATE = STEER;
                //reset flag on exit
                MM_speak_flag = 0;
            }
            MM_MCU_setLED(MM_LED_BLUE, MM_LED_OFF);
            break;    
    }

}

int main() {
    
    while(1) {
        MM_state_machine();
        //get bluetooth value that sets MM_CONTROL value
        MM_BT_getXYZ();
    } 
    return 0;
}
 