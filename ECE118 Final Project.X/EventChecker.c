/*
 * File:   TemplateEventChecker.c
 * Author: Gabriel Hugh Elkaim
 *
 * Template file to set up typical EventCheckers for the  Events and Services
 * Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the
 * names will have to be changed to match your code.
 *
 * This EventCheckers file will work with both FSM's and HSM's.
 *
 * Remember that EventCheckers should only return TRUE when an event has occured,
 * and that the event is a TRANSITION between two detectable differences. They
 * should also be atomic and run as fast as possible for good results.
 *
 * This file includes a test harness that will run the event detectors listed in the
 * ES_Configure file in the project, and will conditionally compile main if the macro
 * EVENTCHECKER_TEST is defined (either in the project or in the file). This will allow
 * you to check you event detectors in their own project, and then leave them untouched
 * for your project unless you need to alter their post functions.
 *
 * Created on September 27, 2013, 8:37 AM
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "EventChecker.h"
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
 * EVENTCHECKER_TEST SPECIFIC CODE                                                             *
 ******************************************************************************/

#define EVENTCHECKER_TEST
#ifdef EVENTCHECKER_TEST
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#endif

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this EventChecker. They should be functions
   relevant to the behavior of this particular event checker */

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

uint8_t TapeInit(void) {
    IO_PortsSetPortInputs(TAPE_PORT, TAPE_PIN_1 | TAPE_PIN_2 | TAPE_PIN_3 | TAPE_PIN_4);
}

/**
 * @Function TapeCheckEvents(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This function is a prototype event checker that checks the battery voltage
 *        against a fixed threshold (#defined in the .c file). Note that you need to
 *        keep track of previous history, and that the actual battery voltage is checked
 *        only once at the beginning of the function. The function will post an event
 *        of either BATTERY_CONNECTED or BATTERY_DISCONNECTED if the power switch is turned
 *        on or off with the USB cord plugged into the Uno32. Returns TRUE if there was an 
 *        event, FALSE otherwise.
 * @note Use this code as a template for your other event checkers, and modify as necessary.
 * @author Gabriel H Elkaim, 2013.09.27 09:18
 * @modified Gabriel H Elkaim/Max Dunne, 2016.09.12 20:08 */
uint8_t TapeCheckEvents(void) {
    
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

/* 
 * The Test Harness for the event checkers is conditionally compiled using
 * the EVENTCHECKER_TEST macro (defined either in the file or at the project level).
 * No other main() can exist within the project.
 * 
 * It requires a valid ES_Configure.h file in the project with the correct events in 
 * the enum, and the correct list of event checkers in the EVENT_CHECK_LIST.
 * 
 * The test harness will run each of your event detectors identically to the way the
 * ES_Framework will call them, and if an event is detected it will print out the function
 * name, event, and event parameter. Use this to test your event checking code and
 * ensure that it is fully functional.
 * 
 * If you are locking up the output, most likely you are generating too many events.
 * Remember that events are detectable changes, not a state in itself.
 * 
 * Once you have fully tested your event checking code, you can leave it in its own
 * project and point to it from your other projects. If the EVENTCHECKER_TEST marco is
 * defined in the project, no changes are necessary for your event checkers to work
 * with your other projects.
 */
#ifdef EVENTCHECKER_TEST
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};

void PrintEvent(void);

void main(void) {
    BOARD_Init();
    /* user initialization code goes here */
    TapeInit();
    // Do not alter anything below this line
    int i;

    printf("\r\nEvent checking test harness for %s", __FILE__);

    while (1) {
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
}
#endif