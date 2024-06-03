/*
 * File:   Track.c
 * Author: Matthew Eng
 *
 * Created on 02 Jun 2024, 8:34 PM
 */

#include <stdio.h>

#include "xc.h"
#include "AD.h"
#include "Tank_DriveTrain.h"
#include "Track.h"
#include "pwm.h"
#include "ES_Configure.h"
#include "ES_Events.h"

#define TRACK_PIN_L1 AD_PORTV7
#define TRACK_PIN_R1 AD_PORTV8

#define TRACK_LOW_THRESHOLD 350
#define TRACK_HIGH_THRESHOLD 700

//#define EVENTCHECKER_TEST
//#define DEBUG
//#define TRACK_TEST

#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

static uint32_t prevRsignal = 0;
static uint32_t prevLsignal = 0;
static uint8_t leftTrack = 0;
static uint8_t rightTrack = 0;

uint32_t ReadRightTrack() {
    return abs(AD_ReadADPin(TRACK_PIN_R1));
}

uint32_t ReadLeftTrack() {
    return abs(AD_ReadADPin(TRACK_PIN_L1));
}

void Track_Init() {
    AD_AddPins(TRACK_PIN_R1 | TRACK_PIN_L1);
    prevRsignal = ReadRightTrack();
    prevLsignal = ReadLeftTrack();
    leftTrack = 0;
    rightTrack = 0;
}

uint8_t Track_CheckEvents(void) {
    
    uint8_t returnVal = FALSE;
    
    uint32_t curRsignal = ReadRightTrack();
    uint32_t curLsignal = ReadLeftTrack();
    
#ifdef DEBUG
//    printf("\r\n");
//    printf("\r\nRight: %d", AD_ReadADPin(TRACK_PIN_R1));
//    printf("\r\nLeft: %d", AD_ReadADPin(TRACK_PIN_L1));
#endif
    
    if (((curLsignal > TRACK_HIGH_THRESHOLD)) && (curRsignal > TRACK_HIGH_THRESHOLD) && ((!leftTrack) || (!rightTrack))) {
#ifdef DEBUG
        printf("Track Parallel\r\n");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = TRACK_WIRE_EQUAL;
        thisEvent.EventParam = (AD_ReadADPin(TRACK_PIN_L1));
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
        leftTrack = 1;
        rightTrack = 1;
        returnVal = TRUE;
    } else if (((curLsignal < TRACK_LOW_THRESHOLD) || (curRsignal < TRACK_LOW_THRESHOLD)) && (leftTrack && rightTrack)) {
#ifdef DEBUG
        printf("Track Not Parallel\r\n");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = TRACK_WIRE_EQUAL;
        thisEvent.EventParam = (AD_ReadADPin(TRACK_PIN_L1));
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
        leftTrack = 0;
        rightTrack = 0;
        returnVal = TRUE;
    } else if (!rightTrack && (curRsignal > TRACK_HIGH_THRESHOLD)) {
#ifdef DEBUG
        printf("Right Track Active\r\n");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = TRACK_WIRE_R;
        thisEvent.EventParam = (curRsignal + curLsignal) >> 1; // avg
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif
        rightTrack = 1;
        returnVal = TRUE;
    } else if (rightTrack && (curRsignal < TRACK_LOW_THRESHOLD)) {
#ifdef DEBUG
        printf("Right Track Inactive\r\n");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = WALL_OFF_R;
        thisEvent.EventParam = (AD_ReadADPin(TRACK_PIN_R1));
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif
        rightTrack = 0;
        returnVal = TRUE;        
    } else if (!leftTrack && (curLsignal > TRACK_HIGH_THRESHOLD)) {
#ifdef DEBUG
        printf("Left Track Wire Active\r\n");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = TRACK_WIRE_L;
        thisEvent.EventParam = (curRsignal + curLsignal) >> 1; // avg
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
        leftTrack = 1;
        returnVal = TRUE;
    } else if (leftTrack && (curLsignal < TRACK_LOW_THRESHOLD)) {
#ifdef DEBUG
        printf("Left Track Wire Inactive\r\n");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = TRACK_OFF_L;
        thisEvent.EventParam = (AD_ReadADPin(TRACK_PIN_L1));
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
        leftTrack = 0;
        returnVal = TRUE;
    } 
    
    prevRsignal = curRsignal;
    prevLsignal = curLsignal;
    
    return returnVal;
}

#ifdef TRACK_TEST
void main (void) {
    BOARD_Init();
    AD_Init();
    PWM_Init();
    DT_Init();
    Track_Init();
    
    while (1) {
        if (Track_CheckEvents()) {
//            printf("\r\nTrack Parallel"); 
        }
    }
    
}
#endif