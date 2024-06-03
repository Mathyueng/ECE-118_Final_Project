/*
 * File:   Parallel.c
 * Author: aanbaner
 *
 * Created on May 31, 2024, 8:37 AM
 */

#include "xc.h"
#include "AD.h"
#include "Tank_DriveTrain.h"
#include "Parallel.h"
#include "pwm.h"
#include "ES_Configure.h"
#include "ES_Events.h"

#define PARALLEL_PIN_R1 AD_PORTV3
#define PARALLEL_PIN_R2 AD_PORTV4
#define PARALLEL_PIN_L1 AD_PORTV5
#define PARALLEL_PIN_L2 AD_PORTV6

#define PARALLEL_LOW_THRESHOLD 75
#define PARALLEL_HIGH_THRESHOLD 225

#define MOV_AVG_LENGTH 5

#define EVENTCHECKER_TEST
//#define DEBUG
//#define PARALLEL_TEST

#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
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

void Parallel_Init() {
    AD_AddPins(PARALLEL_PIN_R1 | PARALLEL_PIN_L1 | PARALLEL_PIN_R2 | PARALLEL_PIN_L2);
    prevRsignal = ReadRight();
    prevLsignal = ReadLeft();
    leftWallP = 0;
    rightWallP = 0;
}

void circularInc(uint8_t* index) {
    *index = ++(*index) % MOV_AVG_LENGTH;
}

uint8_t Parallel_CheckEvents(void) {
    
    uint8_t returnVal = FALSE;
    
    uint32_t curRsignal = ReadRight();
    uint32_t curLsignal = ReadLeft();
    
#ifdef DEBUG
    printf("\r\n");
//    if (abs(curRsignal - avgSignalR) <= PARALLEL_THRESHOLD) printf("\r\ncurrent Right: %d", curRsignal);
    printf("\r\ncurrent right: %d", curRsignal);
    printf("\r\nright 1: %d", AD_ReadADPin(PARALLEL_PIN_R1));
    printf("\r\nright 2: %d", AD_ReadADPin(PARALLEL_PIN_R2));
#endif
    
    if (!rightWallP && (curRsignal < PARALLEL_LOW_THRESHOLD)) {
#ifdef DEBUG
        printf("PARALLEL TO THE RIGHT WALL");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = WALL_PARALLEL_R;
        thisEvent.EventParam = (curRsignal + curLsignal) >> 1; // avg
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
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
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
        rightWallP = 0;
        returnVal = TRUE;
#endif
    } else if (!leftWallP && (curLsignal < PARALLEL_LOW_THRESHOLD)) {
#ifdef DEBUG
        printf("PARALLEL TO THE LEFT WALL");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = WALL_PARALLEL_L;
        thisEvent.EventParam = (curRsignal + curLsignal) >> 1; // avg
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
        leftWallP = 1;
        returnVal = TRUE;
    } else if (leftWallP && (curLsignal > PARALLEL_HIGH_THRESHOLD)) {
#ifdef DEBUG
        printf("NO LONGER PARALLEL PARALLEL TO THE LEFT WALL");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = WALL_OFF_L;
        thisEvent.EventParam = (AD_ReadADPin(PARALLEL_PIN_L1) > AD_ReadADPin(PARALLEL_PIN_L2));
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
        leftWallP = 0;
        returnVal = TRUE;
    }
    
    prevRsignal = curRsignal;
    prevLsignal = curLsignal;
    
    return returnVal;
}

#ifdef PARALLEL_TEST
void main (void) {
    BOARD_Init();
    AD_Init();
    PWM_Init();
    DT_Init();
    Parallel_Init();
    
    while (1) {
        if (Parallel_CheckEvents()) {
            printf("\r\nParallel Event"); 
        }
    }
    
}
#endif