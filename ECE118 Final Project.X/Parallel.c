/*
 * File:   Parallel.c
 * Author: aanbaner
 *
 * Created on May 31, 2024, 8:37 AM
 */
#include "EventChecker.h"
#include "TopHSM.h"

#include "xc.h"
#include <stdio.h>
#include "Parallel.h"
#include "pwm.h"
#include "ES_Configure.h"
#include "ES_Events.h"
#include "ES_Timers.h"
#include "serial.h"
#include "IO_Ports.h"
#include "AD.h"

#define PARALLEL_PIN_R1 AD_PORTV3
#define PARALLEL_PIN_R2 AD_PORTV4
#define PARALLEL_PIN_L1 AD_PORTV5
#define PARALLEL_PIN_L2 AD_PORTV6

#define PARALLEL_LOW_THRESHOLD 75
#define PARALLEL_HIGH_THRESHOLD 225

#define MOV_AVG_LENGTH 5

//#define ParallelMain
//#define DEBUG

#ifdef ParallelMain
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};
#endif
static uint32_t prevRsignal = 0;
static uint32_t prevLsignal = 0;
static uint8_t leftWallP = 0;
static uint8_t rightWallP = 0;

uint32_t ReadRight() {
    return abs(AD_ReadADPin(PARALLEL_PIN_R1) - AD_ReadADPin(PARALLEL_PIN_R2));
}

uint32_t ReadLeft() {
    return abs(AD_ReadADPin(PARALLEL_PIN_L1) - AD_ReadADPin(PARALLEL_PIN_L2));
}

void Parallel_Init(void) {
    AD_AddPins(PARALLEL_PIN_R1 | PARALLEL_PIN_L1 | PARALLEL_PIN_R2 | PARALLEL_PIN_L2);
    prevRsignal = ReadRight();
    prevLsignal = ReadLeft();
    leftWallP = 0;
    rightWallP = 0;
}

uint8_t Parallel_CheckEvents(void) {

    uint8_t returnVal = FALSE;

    uint32_t curRsignal = ReadRight();
    uint32_t curLsignal = ReadLeft();
#ifdef DEBUG
    printf("curLsignal: %d\r\n", curLsignal);
#endif

#ifdef DEBUG
    //    printf("\r\n");
    //    if (abs(curRsignal - avgSignalR) <= PARALLEL_THRESHOLD) printf("\r\ncurrent Right: %d", curRsignal);
    //    printf("\r\ncurrent right: %d", curRsignal);
    //    printf("\r\nright 1: %d", AD_ReadADPin(PARALLEL_PIN_R1));
    //    printf("\r\nright 2: %d", AD_ReadADPin(PARALLEL_PIN_R2));
    //    printf("\r\nleft 1: %d", AD_ReadADPin(PARALLEL_PIN_L1));
    //    printf("\r\nleft 2: %d", AD_ReadADPin(PARALLEL_PIN_L2));    
#endif

    if (!rightWallP && (curRsignal < PARALLEL_LOW_THRESHOLD)) {
#ifdef DEBUG
        //        printf("PARALLEL TO THE RIGHT WALL");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = WALL_PARALLEL_R;
        thisEvent.EventParam = (curRsignal + curLsignal) >> 1; // avg
#ifndef ParallelMain           // keep this as is for test harness
        PostTopHSM(thisEvent);
#else
        SaveEvent(thisEvent);
#endif 
        rightWallP = 1;
        returnVal = TRUE;
    } else if (rightWallP && (curRsignal > PARALLEL_HIGH_THRESHOLD)) {
#ifdef DEBUG
        printf("NO LONGER PARALLEL TO THE RIGHT WALL");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = WALL_OFF_R;
        thisEvent.EventParam = (AD_ReadADPin(PARALLEL_PIN_R1) > AD_ReadADPin(PARALLEL_PIN_R2));
#ifndef ParallelMain           // keep this as is for test harness
        PostTopHSM(thisEvent);
#else
        SaveEvent(thisEvent);
#endif 
        rightWallP = 0;
        returnVal = TRUE;

    } else if (!leftWallP && (curLsignal < PARALLEL_LOW_THRESHOLD)) {
#ifdef DEBUG
        printf("\r\nPARALLEL TO THE LEFT WALL\r\n");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = WALL_PARALLEL_L;
        thisEvent.EventParam = (curRsignal + curLsignal) >> 1; // avg
#ifndef ParallelMain           // keep this as is for test harness
        PostTopHSM(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
        leftWallP = 1;
        returnVal = TRUE;
#ifdef DEBUG
        printf("\r\nleftWallP = %d\r\n", leftWallP);
#endif        
    } else if (leftWallP && (curLsignal > PARALLEL_HIGH_THRESHOLD)) {
#ifdef DEBUG
        printf("NO LONGER PARALLEL TO THE LEFT WALL");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = WALL_OFF_L;
        thisEvent.EventParam = (AD_ReadADPin(PARALLEL_PIN_L1) > AD_ReadADPin(PARALLEL_PIN_L2));
#ifndef ParallelMain           // keep this as is for test harness
        PostTopHSM(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
        leftWallP = 0;
        returnVal = TRUE;
#ifdef DEBUG
        printf("\r\nleftWallP = %d\r\n", leftWallP);
#endif            
    }

    prevRsignal = curRsignal;
    prevLsignal = curLsignal;

    return returnVal;
}

#ifdef ParallelMain

void PrintEvent(void);

void main(void) {
    BOARD_Init();
    /* user initialization code goes here */
    ES_Timer_Init();
    ES_Initialize();
    printf("\r\nTimer initialized");
    Parallel_Init();
    // Do not alter anything below this line
    int i;

    printf("\r\nEvent checking test harness for %s", __FILE__);

    while (1) {
        if (Parallel_CheckEvents()) {
            PrintEvent();
        }
#ifdef DEBUG
        printf("\r\nCheck");
        PrintEvent();
#endif

        if (IsTransmitEmpty()) {
#ifdef DEBUG
            printf("\r\nEmptyCheck");
#endif
            for (i = 0; i< sizeof (EventList) >> 2; i++) {
                if (EventList[i]() == TRUE) {
                    PrintEvent();
                    break;
                }

            }
        }
    }
}

void PrintEvent(void) {
    printf("\r\nFunc: %s\tEvent: %s\tParam: 0x%X", eventName,
            EventNames[storedEvent.EventType], storedEvent.EventParam);
}
#endif