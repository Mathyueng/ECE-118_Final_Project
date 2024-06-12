/*
 * File: TapeService.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim, Matthew Eng
 *
 * Template file to set up a simple service to work with the Events and Services
 * Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that this file
 * will need to be modified to fit your exact needs, and most of the names will have
 * to be changed to match your code.
 *
 * This is provided as an example and a good place to start.
 *
 * Created on 23/Oct/2011
 * Updated on 13/Nov/2013
 * Modified on 06/Jun/2024
 */

/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/
#include "xc.h"
#include <stdio.h>
#include <stdlib.h>

#include "BOARD.h"
#include "AD.h"
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "TapeService.h"
#include "TopHSM.h"
#include "IO_Ports.h"
#include "LED.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

//#define TapeMain
//#define DEBUG

#ifdef TapeMain
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
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/
/* You will need MyPriority and maybe a state variable; you may need others
 * as well. */

static uint8_t MyPriority;

// initialize history of tape sensor to say we're off tape
static uint8_t prevT1 = 0;
static uint8_t prevT2 = 0;
static uint8_t prevT3 = 0;
static uint8_t prevT4 = 0;

static uint8_t PrevTape;
/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateService(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateService function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitTapeService(uint8_t Priority) {
    ES_Event ThisEvent;

    MyPriority = Priority;

    // in here you write your initialization code
    // this includes all hardware and software initialization
    // that needs to occur.
    IO_PortsSetPortInputs(TAPE_PORT, TAPE_PIN_1 | TAPE_PIN_2 | TAPE_PIN_3 | TAPE_PIN_4 | (IO_PortsReadPort(TAPE_PORT)));
    prevT1 = 0;
    prevT2 = 0;
    prevT3 = 0;
    prevT4 = 0;

    PrevTape = 0;
    // post the initial transition event
    ThisEvent.EventType = ES_INIT;
    ES_Timer_InitTimer(TAPE_SERVICE_TIMER, TIMER_0_TICKS);
    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function PostTemplateService(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostTapeService(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

/**
 * @Function RunTemplateService(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the service,
 *        as this is called any time a new event is passed to the event queue. 
 * @note Remember to rename to something appropriate.
 *       Returns ES_NO_EVENT if the event have been "consumed." 
 * @author J. Edward Carryer, 2011.10.23 19:25 */
ES_Event RunTapeService(ES_Event ThisEvent) {
    ES_Event ReturnEvent;
    ReturnEvent.EventType = ES_NO_EVENT; // assume no errors

    /********************************************
     in here you write your service code
     *******************************************/

    switch (ThisEvent.EventType) {
        case ES_INIT:
            // No hardware initialization or single time setups, those
            // go in the init function above.
            //
            // This section is used to reset service for some reason
            break;

        case ES_TIMEOUT:
            ES_Timer_InitTimer(TAPE_SERVICE_TIMER, TIMER_0_TICKS);
            uint8_t returnVal = FALSE;

            uint8_t curT1 = (prevT1 << 1) | !(IO_PortsReadPort(TAPE_PORT) & TAPE_PIN_1);
            uint8_t curT2 = (prevT2 << 1) | !(IO_PortsReadPort(TAPE_PORT) & TAPE_PIN_2);
            uint8_t curT3 = (prevT3 << 1) | !(IO_PortsReadPort(TAPE_PORT) & TAPE_PIN_3);
            uint8_t curT4 = (prevT4 << 1) | !(IO_PortsReadPort(TAPE_PORT) & TAPE_PIN_4);

            uint8_t CurTape = ReadTapeSensors();
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
#ifdef DEBUG
                    printf("\r\nPrevTape: 0x%X\r\n", PrevTape);
                    printf("\r\nCurTape: 0x%X\r\n", CurTape);
#endif
                if (tapeOn) { // check for change from last time
                    ReturnEvent.EventType = TAPE_ON;
                    ReturnEvent.EventParam = CurTape;
                    
                    returnVal = TRUE;
#ifdef DEBUG
                    printf("\r\nReturn Event: %s\tParam: 0x%X",
                            EventNames[ReturnEvent.EventType], ReturnEvent.EventParam);
#endif
                } else if (tapeOff) { // check for change from last time
                    ReturnEvent.EventType = TAPE_OFF;
                    ReturnEvent.EventParam = CurTape;
                    
                    returnVal = TRUE;
#ifdef DEBUG
                    printf("\r\nReturn Event: %s\tParam: 0x%X",
                            EventNames[ReturnEvent.EventType], ReturnEvent.EventParam);
#endif
                }
                
                PrevTape = CurTape;
                
                if (returnVal) PostTopHSM(ReturnEvent);
                
                prevT1 = curT1;
                prevT2 = curT2;
                prevT3 = curT3;
                prevT4 = curT4;
                
                break;
            }
            return ReturnEvent;
    }
}

/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/
uint8_t ReadTapeSensors() {
    uint8_t tapeActive =
            (!(IO_PortsReadPort(TAPE_PORT) & (TAPE_PIN_1)) << 3) |
            (!(IO_PortsReadPort(TAPE_PORT) & (TAPE_PIN_2)) << 2) |
            (!(IO_PortsReadPort(TAPE_PORT) & (TAPE_PIN_3)) << 1) |
            (!(IO_PortsReadPort(TAPE_PORT) & (TAPE_PIN_4)) << 0);
    return tapeActive;
}
