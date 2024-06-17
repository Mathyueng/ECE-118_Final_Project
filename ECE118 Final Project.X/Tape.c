/*
 * File:   tape.c
 * Author: aanbaner
 *
 * Created on May 29, 2024, 7:48 PM
 */

#include "EventChecker.h"
#include "TopHSM.h"

#include "xc.h"
#include <stdio.h>
#include "Tape.h"
#include "ES_Events.h"
#include "ES_Timers.h"
#include "serial.h"
#include "IO_Ports.h"
#include "AD.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
//#define TapeMain
//#define DEBUG_TAPE
//#define INACTIVE

#ifdef TapeMain
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};
#endif

#define TAPE_PORT PORTZ
#define TAPE_PIN_1 PIN3     // FL
#define TAPE_PIN_2 PIN4     // FR
#define TAPE_PIN_3 PIN5     // BR
#define TAPE_PIN_4 PIN6     // BL

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/

/* Any private module level variable that you might need for keeping track of
   events would be placed here. Private variables should be STATIC so that they
   are limited in scope to this module. */

// initialize history of tape sensor to say we're off tape
static uint8_t prevT1 = 0;
static uint8_t prevT2 = 0;
static uint8_t prevT3 = 0;
static uint8_t prevT4 = 0;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t ReadTapeSensors() {
    uint8_t tapeActive =
            (!(IO_PortsReadPort(TAPE_PORT) & (TAPE_PIN_1)) << 3) |
            (!(IO_PortsReadPort(TAPE_PORT) & (TAPE_PIN_2)) << 2) |
            (!(IO_PortsReadPort(TAPE_PORT) & (TAPE_PIN_3)) << 1) |
            (!(IO_PortsReadPort(TAPE_PORT) & (TAPE_PIN_4)) << 0);
    return tapeActive;
}

uint8_t Tape_Init(void) {
    IO_PortsSetPortInputs(TAPE_PORT, TAPE_PIN_1 | TAPE_PIN_2 | TAPE_PIN_3 | TAPE_PIN_4);
    prevT1 = 0;
    prevT2 = 0;
    prevT3 = 0;
    prevT4 = 0;
}

/**
 * @Function TapeCheckEvents(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This function raises TAPE_ON & TAPE_OFF events; the event params specify which tape sensors were triggered or untriggered
 * @author Aarush Banerjee, 5/29
 * */
uint8_t Tape_CheckEvents(void) {

    uint8_t curT1 = (prevT1 << 1) | !(IO_PortsReadPort(TAPE_PORT) & TAPE_PIN_1);
    uint8_t curT2 = (prevT2 << 1) | !(IO_PortsReadPort(TAPE_PORT) & TAPE_PIN_2);
    uint8_t curT3 = (prevT3 << 1) | !(IO_PortsReadPort(TAPE_PORT) & TAPE_PIN_3);
    uint8_t curT4 = (prevT4 << 1) | !(IO_PortsReadPort(TAPE_PORT) & TAPE_PIN_4);

    uint8_t returnVal = FALSE;

#ifdef DEBUG_TAPE
    if (curT1 != prevT1) printf("\r\ncurT1: %x", curT1);
    if (curT2 != prevT2) printf("\r\ncurT2: %x", curT2);
    if (curT3 != prevT3) printf("\r\ncurT3: %x", curT3);
    if (curT4 != prevT4) printf("\r\ncurT4: %x", curT4);
#endif

    if ((curT1 != prevT1) ||
            (curT2 != prevT2) ||
            (curT3 != prevT3) ||
            (curT4 != prevT4)) {

        uint8_t tapeOff =
                ((curT1 && !prevT1) << 3) |
                ((curT2 && !prevT2) << 2) |
                ((curT3 && !prevT3) << 1) |
                ((curT4 && !prevT4) << 0);

        uint8_t tapeOn =
                ((!curT1 && prevT1) << 3) |
                ((!curT2 && prevT2) << 2) |
                ((!curT3 && prevT3) << 1) |
                ((!curT4 && prevT4) << 0);
        
        if (tapeOn) {
#ifdef DEBUG_TAPE
        printf("\r\nTapeOn\r\n");
#endif            
            ES_Event thisEvent;
            thisEvent.EventType = TAPE_ON;
            thisEvent.EventParam = tapeOn;
#ifdef DEBUG_HSM
            printf("TAPE_ON\r\n");
#endif
#ifndef TapeMain           // keep this as is for test harness
            PostTopHSM(thisEvent);
#else
            SaveEvent(thisEvent);
#endif   
            returnVal = TRUE;
        }

        // TAPE_OFF detection
        if (tapeOff) {
#ifdef DEBUG_TAPE
        printf("\r\nTapeOff\r\n");
#endif            
            ES_Event thisEvent;
            thisEvent.EventType = TAPE_OFF;
            thisEvent.EventParam = tapeOff;
#ifdef DEBUG_HSM
            printf("TAPE_OFF\r\n");
#endif
#ifndef TapeMain           // keep this as is for test harness
            PostTopHSM(thisEvent);
#else
            SaveEvent(thisEvent);
#endif 
            returnVal = TRUE;
        }
    }

    // update history
    prevT1 = curT1;
    prevT2 = curT2;
    prevT3 = curT3;
    prevT4 = curT4;

    return (returnVal);
}

#ifdef TapeMain

void PrintEvent(void);

void main(void) {
    BOARD_Init();
    /* user initialization code goes here */
    ES_Timer_Init();
    printf("\r\nTimer initialized");
    Tape_Init();
    // Do not alter anything below this line
    int i;

    printf("\r\nEvent checking test harness for %s", __FILE__);

    while (1) {
    if (Tape_CheckEvents()) {
        PrintEvent();
    }
    if (IsTransmitEmpty()) {
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
    //    printf("\r\nCurrent Active Tape Sensors: %x", GetCurrentTape());
}
#endif