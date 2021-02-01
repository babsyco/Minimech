#include <stdint.h>
#include <stm8s.h>
#include <string.h>
#include <MM_lib.h>

/**********************************************************************************
 * @File     MM_stm8s.c
 * @AUthor   Daniel Babekuhl
 * @Date     7th June 2020
 * @Brief    This file contains library for the STM8S208CB microcontroller
 *           to run the software for the MiniMech robot.
 * ********************************************************************************
 *
 * This file contains a library for use of STM8S205CB microcontroller to run
 * MiniMech software. Configured as follows:
 * 
 *  UART1: bluetooth module
 *  UART3: text-to-speech (T2S) module
 *  Orange LED:     PA3
 *  Red LED:        PA2
 *  Green LED:      PA1
 *  Blue LED:       PA0
 *  Left motor:     PB1
 *  Right motor:    PB0
 * 
 ***********************************************************************************/

/*
 * Configure clock, GPIOs, UARTS chip on startup
 */
void MM_MCU_init(void) { 
    // configure clock
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);  
    // Set PA5 as Output open-drain high-impedance level for UART1_Tx
    GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST);

    // INITIALISE GPIO PINS
    // Initialise four LED pins:
    // PA3 = Orange, PA2 = Red, PA1 = Green, PA0 = Blue
    GPIO_Init(GPIOA, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0,
        GPIO_MODE_OUT_PP_LOW_FAST);

    // Initialise two motor pins:
    // PB1 = Left, PB0 = right
    GPIO_Init(GPIOB, GPIO_PIN_1 | GPIO_PIN_0,
        GPIO_MODE_OUT_PP_LOW_FAST);

    // INITIALISE UARTs
    // UART1: bluetooth module
    UART1_DeInit();
    UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
                UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
    // Enable UART1 Half Duplex Mode
    UART1_HalfDuplexCmd(ENABLE);

    // UART3: T2S module
    UART3_DeInit();
    UART3_Init((uint32_t)9600, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO,
                UART3_MODE_TXRX_ENABLE);
}

/*
 * Standard delay function. Uses Timer1. 
 * Max delay = 1000ms.
 */
uint16_t count_val = 0;
void MM_MCU_delay(__IO uint32_t ms) {
    // configure timer 1, 1000 ticks per second
    TIM1_TimeBaseInit(16000, TIM1_COUNTERMODE_UP, 16000, 1);
    // enable timer
    TIM1_Cmd(ENABLE);
    
    count_val = TIM1_GetCounter();
    while (TIM1_GetCounter() != ((count_val + ms) % 1000)){}
}

/*
 * Send a single byte to a module via UART.
 * 'module' argmument must be either "BT" or "T2S"
 */
void MM_MCU_sendByte(char byte, char * module) {
    // send byte to BT module
    if(strcmp(module, "BT")) {
        // Wait until end of transmit
        while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET){}
        // Write one byte in the UART1 Transmit Data Register
        UART1_SendData8(byte);

    }
    else if(strcmp(module, "T2S")) {
        // Wait until end of transmit
        while (UART3_GetFlagStatus(UART1_FLAG_TXE) == RESET){}
        // Write one byte in the UART3 Transmit Data Register
        UART3_SendData8(byte);
    }
}

/*
 * Recieve a single byte from a module via UART.
 * 'module' argmument must be either "BT" or "T2S"
 */
char MM_MCU_recvByte(char * module) {
    // send byte to BT module
    if(strcmp(module, "BT")) {
        // Wait until byte is entirely received by UART1
        while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET){}
        /* Store the received byte in the RxBuffer1 */
        return UART1_ReceiveData8();

    }
    else if(strcmp(module, "T2S")) {
        // Wait until byte is entirely recieved by UART3
        while (UART3_GetFlagStatus(UART3_FLAG_RXNE) == RESET){}
        // Write one byte in the UART1 Transmit Data Register
        return UART3_ReceiveData8();
    }
    // should never reach here
    return '\0';
}

/*
 * Configure an LED. argument types are declared in MM_lib.h
 */
void MM_MCU_setLED(MM_led MM_LED_COLOUR, MM_led_state MM_STATE){
    // Orange LED
    if (MM_LED_COLOUR == MM_LED_ORANGE) {
        if (MM_STATE == MM_LED_ON) {
            GPIO_WriteHigh(GPIOA, GPIO_PIN_3);
        } else GPIO_WriteLow(GPIOA, GPIO_PIN_3);
    }
    //Red LED
    if (MM_LED_COLOUR == MM_LED_RED) {
        if (MM_STATE == MM_LED_ON) {
            GPIO_WriteHigh(GPIOA, GPIO_PIN_2);
        } else GPIO_WriteLow(GPIOA, GPIO_PIN_2);
    }
    //Green LED
    if (MM_LED_COLOUR == MM_LED_GREEN) {
        if (MM_STATE == MM_LED_ON) {
            GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
        } else GPIO_WriteLow(GPIOA, GPIO_PIN_1);
    }
    //Green LED
    if (MM_LED_COLOUR == MM_LED_BLUE) {
        if (MM_STATE == MM_LED_ON) {
            GPIO_WriteHigh(GPIOA, GPIO_PIN_0);
        } else GPIO_WriteLow(GPIOA, GPIO_PIN_0);
    }
}

/*
 * Turn motors on/off. argument types are declared in MM_lib.h
 */
void MM_MCU_setMotor(MM_motor MM_MOTOR, MM_motor_state MM_STATE){
    // Left Motor
    if (MM_MOTOR == MM_MOTOR_L) {
        if (MM_STATE == MM_MOTOR_ON) {
            GPIO_WriteHigh(GPIOB, GPIO_PIN_1);
        } else GPIO_WriteLow(GPIOB, GPIO_PIN_1);
    }
    // Right Motor
    if (MM_MOTOR == MM_MOTOR_R) {
        if (MM_STATE == MM_MOTOR_ON) {
            GPIO_WriteHigh(GPIOB, GPIO_PIN_0);
        } else GPIO_WriteLow(GPIOB, GPIO_PIN_0);
    }
}
