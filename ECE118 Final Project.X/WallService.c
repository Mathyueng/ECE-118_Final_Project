/*
 * File: WallService.c
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
#include "WallService.h"
#include "TopHSM.h"
#include "IO_Ports.h"
#include "LED.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

//#define WallMain
//#define DEBUG

#ifdef WallMain
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};
#endif

#define WALL_PORT PORTZ
#define WALL_PIN_1 PIN7     // Front

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

// initialize history of wall sensor to say we're off wall
static uint8_t OnOff = 0;


static uint8_t PrevWall;
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
uint8_t InitWallService(uint8_t Priority) {
    ES_Event ThisEvent;

    MyPriority = Priority;

    // in here you write your initialization code
    // this includes all hardware and software initialization
    // that needs to occur.
    IO_PortsSetPortInputs(WALL_PORT, WALL_PIN_1 | (IO_PortsReadPort(WALL_PORT)));


    PrevWall = 0;
    // post the initial transition event
    ThisEvent.EventType = ES_INIT;
    ES_Timer_InitTimer(WALL_SERVICE_TIMER, TIMER_0_TICKS);
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
uint8_t PostWallService(ES_Event ThisEvent) {
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
ES_Event RunWallService(ES_Event ThisEvent) {
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
            ES_Timer_InitTimer(WALL_SERVICE_TIMER, TIMER_0_TICKS);

            uint8_t CurWall = ReadWallSensors();

            if (CurWall != PrevWall) { // check for change from last time
#ifdef DEBUG
                printf("\r\nPrevWall: 0x%X\r\n", PrevWall);
                printf("\r\nCurWall: 0x%X\r\n", CurWall);
#endif
                if (!OnOff) {
                    ReturnEvent.EventType = WALL_ON;
                    ReturnEvent.EventParam = CurWall;
                    OnOff = 1;
                } else {
                    ReturnEvent.EventType = WALL_OFF;
                    ReturnEvent.EventParam = CurWall;
                    OnOff = 0;
                }
                PrevWall = CurWall; // update history
#ifdef DEBUG
                printf("\r\nReturn Event: %s\tParam: 0x%X",
                        EventNames[ReturnEvent.EventType], ReturnEvent.EventParam);
#endif
#ifndef SIMPLESERVICE_TEST           // keep this as is for test harness
                PostTopHSM(ReturnEvent);
#else
                PostTemplateService(ReturnEvent);
#endif   
            }
            break;
#ifdef SIMPLESERVICE_TEST     // keep this as is for test harness      
        default:
            printf("\r\nEvent: %s\tParam: 0x%X",
                    EventNames[ThisEvent.EventType], ThisEvent.EventParam);
            break;
#endif
    }

    return ReturnEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/
uint8_t ReadWallSensors() {
    uint8_t wallActive =
            (!(IO_PortsReadPort(WALL_PORT) & (WALL_PIN_1)));
    return wallActive;
}

