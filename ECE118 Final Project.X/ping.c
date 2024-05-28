/*
 * File:   ping.c
 * Author: aarushbanerjee
 *
 * Created on May 6, 2024, 6:42 PM
 */


#include "xc.h"
#include "ping.h"
#include "BOARD.h"
#include "IO_Ports.h"
#include "ES_Timers.h"
#include "ES_Configure.h"

//#define PING_MAIN
#define PING_BASIC_TEST

#ifdef PING_TEST
#include <stdio.h>
#endif

#define NOPS_FOR_10US 1020
#define PING_10US
#define PING_1MS

#define D_OVER_EW_RATIO 1000 / 148 // D/EW, adjusted for taking in ms instead of us
#define PING_SENSOR_PORT PORTY
#define TRIGGER_PIN PIN3
#define ECHO_PIN PIN4
#define PING_TIMER 

#define TIMER_uS_PER_TICK 25

typedef enum {
    IDLE,
    TRIGGER,
    WAITING,
    ECHO
} PingState;

static PingState state;
static unsigned int FreeRunningTimer;
static uint16_t start_time;

void PingInit(void) {
    state = IDLE;
    IO_PortsSetPortOutputs(PING_SENSOR_PORT, TRIGGER_PIN);
    IO_PortsSetPortInputs(PING_SENSOR_PORT, ECHO_PIN);
}

int16_t ReadEcho(void) {
    return (IO_PortsReadPort(PING_SENSOR_PORT) & ECHO_PIN);
}

void SetTrigger(uint8_t in) {
    if (in) IO_PortsWritePort(PING_SENSOR_PORT, IO_PortsReadPort(PING_SENSOR_PORT) | TRIGGER_PIN);
    else IO_PortsWritePort(PING_SENSOR_PORT, IO_PortsReadPort(PING_SENSOR_PORT) & ~TRIGGER_PIN);
}

/* 
 * @definition:
 * called by ES_TIMEOUT events for respective PING_TIMER
 * this function triggers the ping sensor if inactive. 
 * if active, it keeps count until called when ECHO goes low
 * @params:
 * no parameters
 * @return:
 * this function will return 1 when no value is available
 * when it has a distance it has detected from the ping sensor, it will return that distance in increments of 6-7 inches
 */
int16_t PingAndReportDistance(void) { // function for Problem 6, part c
    switch (state) {
        case IDLE:
            SetTrigger(1); // set output pin TRIGGER to 1
            start_time = FreeRunningTimer;
            state = TRIGGER;
            break;
        case TRIGGER:
            if (FreeRunningTimer - start_time >= 100) {
                SetTrigger(0);
                state = WAITING;
            }
            break;
        case WAITING:
            if (ReadEcho()) {
                start_time = FreeRunningTimer;
                state = ECHO;
            }
            break;
        case ECHO:
            if (!ReadEcho()) {
                state = IDLE;
                return (FreeRunningTimer - start_time) * (D_OVER_EW_RATIO / (1000 / TIMER_uS_PER_TICK));
            }
            break;
    }
    return 1;
}


#ifdef PING_MAIN

int main(void) {
    BOARD_Init();
    PingInit();
    printf("\r\nPing sensor test harness for %s", __FILE__);
    
#ifdef PING_BASIC_TEST
#define DELAY(x) for (int i=0;i<(400000*x);i++) asm("nop");
    while (1) {
        printf("\r\nnew cycle");
        SetTrigger(1);
        DELAY(1);
        SetTrigger(0);
        while (!ReadEcho()) printf("\r\nnot yet");
        int counter = 0;
        printf("\r\necho high");
        while (ReadEcho()) counter++;
        printf("\r\necho low: %d cycles", counter);
        DELAY(5);
    }
#endif
    
}
#endif
