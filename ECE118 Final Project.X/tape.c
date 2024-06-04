/*
 * File:   tape.c
 * Author: aanbaner
 *
 * Created on May 29, 2024, 7:48 PM
 */


#include "xc.h"
#include <stdio.h>
#include "tape.h"

#include "EventChecker.h"
#include "ES_Configure.h"
#include "ES_Events.h"
#include "serial.h"
#include "IO_Ports.h"
#include "AD.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define TAPE_PORT PORTZ
#define TAPE_PIN_1 PIN3
#define TAPE_PIN_2 PIN4
#define TAPE_PIN_3 PIN5
#define TAPE_PIN_4 PIN6

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif
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

uint8_t Tape_Init(void) {
    IO_PortsSetPortInputs(TAPE_PORT, TAPE_PIN_1 | TAPE_PIN_2 | TAPE_PIN_3 | TAPE_PIN_4 | (IO_PortsReadPort(TAPE_PORT)));
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
#ifdef DEBUG
        printf ("TapeEvent");
#endif
        ES_Event thisEvent;
        thisEvent.EventType = TAPE_ON;
        thisEvent.EventParam = tapeOn;
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
        returnVal = TRUE;
    }
    
    // TAPE_OFF detection
    if (tapeOff) {
        ES_Event thisEvent;
        thisEvent.EventType = TAPE_OFF;
        thisEvent.EventParam = tapeOff & 0x0F;
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif 
        returnVal = TRUE;
    }
    
    // update history
    prevT1 = curT1;
    prevT2 = curT2;
    prevT3 = curT3;
    prevT4 = curT4;
    
    return (returnVal);
}
