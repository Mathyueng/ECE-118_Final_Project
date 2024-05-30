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
#include "ES_Events.h"

//#define PING_MAIN
#define PING_BASIC_TEST
#define DEBUG
#define EVENTCHECKER_TEST

#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

#ifdef DEBUG
#include <stdio.h>
#endif

#ifdef PING_TEST
#include <stdio.h>
#endif

#define NOPS_FOR_10US 1020
#define PING_10US
#define PING_1MS

#define D_OVER_EW_RATIO 148 // D/EW, adjusted for taking in ms instead of us
#define PING_SENSOR_PORT PORTW
#define TRIGGER_PIN PIN3
#define ECHO_PIN PIN4
#define PING_TIMER 

#define TIMER_TICKS_PER_uS 40

typedef enum {
    IDLE,
    TRIGGER,
    WAITING,
    ECHO
} PingState;

static PingState state;
static uint16_t start_time;

// private functions

int16_t ReadEcho(void) {
    return (IO_PortsReadPort(PING_SENSOR_PORT) & ECHO_PIN);
}

void SetTrigger(uint8_t in) {
    if (in) IO_PortsWritePort(PING_SENSOR_PORT, IO_PortsReadPort(PING_SENSOR_PORT) | TRIGGER_PIN);
    else IO_PortsWritePort(PING_SENSOR_PORT, IO_PortsReadPort(PING_SENSOR_PORT) & ~TRIGGER_PIN);
}

// public functions

void Ping_Init(void) {
    state = IDLE;
    IO_PortsSetPortOutputs(PING_SENSOR_PORT, TRIGGER_PIN);
    IO_PortsSetPortInputs(PING_SENSOR_PORT, ECHO_PIN);
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
uint8_t Ping_StateMachine(void) { // function for Problem 6, part c
    static uint32_t start_time_in_uS = 0;
    uint32_t FRT_in_uS = (ES_Timer_GetTime() * 1000) + (TMR1 / TIMER_TICKS_PER_uS);
    uint8_t returnVal = FALSE;
    switch (state) {
        case IDLE:
            if (FRT_in_uS - start_time_in_uS >= 60000) {
                SetTrigger(1); // set output pin TRIGGER to 1
                start_time_in_uS = FRT_in_uS;
                state = TRIGGER;
            }
            break;
        case TRIGGER:
            if (FRT_in_uS - start_time_in_uS >= 10) {
                SetTrigger(0);
                state = WAITING;
            }
            break;
        case WAITING:
            if (ReadEcho()) {
                start_time_in_uS = FRT_in_uS;
                state = ECHO;
            }
            break;
        case ECHO:
            if (!ReadEcho()) {
                ES_Event thisEvent;
                thisEvent.EventType = PING;
                thisEvent.EventParam = (FRT_in_uS - start_time_in_uS) / D_OVER_EW_RATIO;
#ifdef DEBUG
                printf("\r\nFinal distance in inches: %d", thisEvent.EventParam);
#endif
                if (thisEvent.EventParam <= 10) {
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//                    PostGenericService(thisEvent);
#else
                    SaveEvent(thisEvent);
#endif   
                returnVal = TRUE;
                }
                state = IDLE;
            }
            break;
    }
    return returnVal;
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
