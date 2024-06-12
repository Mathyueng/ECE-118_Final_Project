/*
 * File:   Track.c
 * Author: Matthew Eng
 *
 * Created on 02 Jun 2024, 8:34 PM
 */

#include <stdio.h>

#include "EventChecker.h"
#include "TopHSM.h"

#include "xc.h"
#include "AD.h"
#include "Tank_DriveTrain.h"
#include "Track.h"
#include "pwm.h"
#include "ES_Configure.h"
#include "ES_Events.h"

#define TRACK_PIN_1 AD_PORTV7

#define TRACK_LOW_THRESHOLD 350
#define TRACK_HIGH_THRESHOLD 700

//#define TrackMain
//#define DEBUG
//#define TRACK_TEST

#ifdef TrackMain
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

static uint32_t prevSignal = 0;
static uint8_t TrackFlag = 0;

uint32_t ReadTrack() {
    return abs(AD_ReadADPin(TRACK_PIN_1));
}

void Track_Init() {
    AD_AddPins(TRACK_PIN_1);
    prevSignal = ReadTrack();
    TrackFlag = 0;
}

uint8_t Track_CheckEvents(void) {

    uint8_t returnVal = FALSE;

    uint32_t curSignal = ReadTrack();

    ES_Event thisEvent;
    thisEvent.EventType = ES_NO_EVENT;
    thisEvent.EventParam = 0;

#ifdef DEBUG
    //    printf("\r\n");
    //    printf("\r\nRight: %d", AD_ReadADPin(TRACK_PIN_R1));
    //    printf("\r\nLeft: %d", AD_ReadADPin(TRACK_PIN_L1));
#endif

    if (!TrackFlag && (curSignal > TRACK_HIGH_THRESHOLD)) {
#ifdef DEBUG
        printf("Track Active\r\n");
#endif
        thisEvent.EventType = TRACK_WIRE_ON;
        thisEvent.EventParam = curSignal;
        TrackFlag = 1;
        returnVal = TRUE;

    } else if (TrackFlag && (curSignal < TRACK_LOW_THRESHOLD)) {
#ifdef DEBUG
        printf("Track Inactive\r\n");
#endif
        thisEvent.EventType = TRACK_WIRE_OFF;
        thisEvent.EventParam = curSignal;
        TrackFlag = 0;
        returnVal = TRUE;
    }
    if (returnVal) PostTopHSM(thisEvent);
    prevSignal = curSignal;

    return returnVal;
}

#ifdef TrackMain
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};

void PrintEvent(void);

void main(void) {
    BOARD_Init();
    /* user initialization code goes here */
    ES_Timer_Init();
    printf("\r\nTimer initialized");
    Track_Init();
    // Do not alter anything below this line
    int i;

    printf("\r\nEvent checking test harness for %s", __FILE__);

    while (1) {
        if (Track_CheckEvents()) {
            PrintEvent();
        }
#ifdef DEBUG
        printf("\r\nCheck");
        PrintEvent();
#endif
    }
}

void PrintEvent(void) {
    printf("\r\nFunc: %s\tEvent: %s\tParam: 0x%X", eventName,
            EventNames[storedEvent.EventType], storedEvent.EventParam);
}
#endif