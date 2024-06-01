/*
 * File:   Parallel.c
 * Author: aanbaner
 *
 * Created on May 31, 2024, 8:37 AM
 */

#include "xc.h"
#include "AD.h"
#include "Tank_DriveTrain.h"
#include "pwm.h"
#include "ES_Configure.h"
#include "ES_Events.h"

#define PARALLEL_PIN_R AD_PORTV3
#define PARALLEL_PIN_L AD_PORTV4
#define PARALLEL_THRESHOLD 75

#define MOV_AVG_LENGTH 5

#define EVENTCHECKER_TEST
#define DEBUG
#define PARALLEL_TEST

#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

typedef struct {
    uint16_t array[MOV_AVG_LENGTH];
    uint8_t head;
    uint8_t tail;
} buffer;

static uint32_t avgSignalR = 0;
static uint32_t avgSignalL = 0;
static buffer readingsR;
static buffer readingsL;

void Parallel_Init() {
    AD_AddPins(PARALLEL_PIN_R | PARALLEL_PIN_L);
    for (int i = 0; i< 0; i++) {
        readingsR.array[i] = AD_ReadADPin(PARALLEL_PIN_R);
        avgSignalR += readingsR.array[i];
    }
    for (int i = 0; i< 0; i++) {
        readingsL.array[i] = AD_ReadADPin(PARALLEL_PIN_L);
        avgSignalL += readingsL.array[i];
    }
    readingsR.head = 0;
    readingsR.tail = 1;
    readingsL.head = 0;
    readingsL.tail = 1;
}

void circularInc(uint8_t* index) {
    *index = ++(*index) % MOV_AVG_LENGTH;
}

uint32_t movingAvg(buffer* buf, uint16_t input, uint32_t signal) {
    buf->array[buf->head] = input;
    uint32_t returnVal = signal;
    returnVal += buf->array[buf->head];
    returnVal -= buf->array[buf->tail];
    circularInc(&(buf->head));
    circularInc(&(buf->tail));
    
    return returnVal;
}

uint8_t Parallel_CheckEvents(void) {
    uint8_t returnVal = FALSE;
    
    uint16_t curRsignal = movingAvg(&readingsR, AD_ReadADPin(PARALLEL_PIN_R), avgSignalR);
    uint16_t curLsignal = movingAvg(&readingsL, AD_ReadADPin(PARALLEL_PIN_L), avgSignalL);
    
#ifdef DEBUG
    if (abs(curLsignal - avgSignalL) <= PARALLEL_THRESHOLD) printf("\r\ncurrent Right: %d", curRsignal);
//    printf("\r\ncurrent Left: %d\r\n", curLsignal);
#endif
    
    if ((curRsignal < avgSignalR) && (abs(curRsignal - avgSignalR) <= PARALLEL_THRESHOLD)) {
        ES_Event thisEvent;
        thisEvent.EventType = WALL_PARALLEL_R;
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif
        returnVal = TRUE;
    }
    
    if ((curLsignal < avgSignalL) && (abs(curLsignal - avgSignalL) <= PARALLEL_THRESHOLD)) {
        ES_Event thisEvent;
        thisEvent.EventType = WALL_PARALLEL_L;
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
        returnVal = TRUE;
    }
    
    avgSignalR = curRsignal;
    avgSignalL = curLsignal;
    
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