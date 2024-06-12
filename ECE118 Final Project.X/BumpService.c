/*
 * File: TemplateService.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
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
#include "BumpService.h"
#include "TopHSM.h"
#include "IO_Ports.h"
#include "LED.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

#define BUMP_PORT   PORTY
#define BUMP_PIN_1  PIN9
#define BUMP_PIN_2  PIN11

#define ALLRELEASED 0

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
uint8_t InitBumpService(uint8_t Priority) {
    ES_Event ThisEvent;

    MyPriority = Priority;

    // in here you write your initialization code
    // this includes all hardware and software initialization
    // that needs to occur.

    // post the initial transition event
    IO_PortsSetPortInputs(BUMP_PORT, BUMP_PIN_1 | BUMP_PIN_2 | (IO_PortsReadPort(BUMP_PORT)));
    ThisEvent.EventType = ES_INIT;
    ES_Timer_InitTimer(BUMP_SERVICE_TIMER, TIMER_0_TICKS);
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
uint8_t PostBumpService(ES_Event ThisEvent) {
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
ES_Event RunBumpService(ES_Event ThisEvent)
{
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
        case ES_TIMERACTIVE:
        case ES_TIMERSTOPPED:
            break;
        case ES_TIMEOUT:
            ES_Timer_InitTimer(BUMP_SERVICE_TIMER, TIMER_0_TICKS);


            //uint8_t bumpOccurred = CheckBump();
            unsigned char curBump = ReadBumpSensors();
            static unsigned char preBump = ALLRELEASED;

            //printf("\r\n PreBump: 0x%X",preBump);
            //printf("\r\n CurBump: 0x%X",curBump);
            //printf("\r\n cur = pre?: %d", (int)curBump == (int)preBump);
            if (curBump != preBump) {
                printf("\r\nBumpChange\n");
                ReturnEvent.EventType = BUMPER_EVENT; // event set
                ReturnEvent.EventParam = curBump; // params
                preBump = curBump; // Update        

                //        printf("\r\nReturn Event: %s\tParam: 0x%X",
                //                EventNames[ReturnEvent.EventType], ReturnEvent.EventParam);
#ifndef SIMPLESERVICE_TEST           // keep this as is for test harness
                PostTopHSM(ReturnEvent);
#else
                PostRoachFSM(ReturnEvent);
#endif   
            }
            break;



#ifdef SIMPLESERVICE_TEST     // keep this as is for test harness      
        default:
            //        printf("\nTemplate");
            //printf("\r\nEvent: %s\tParam: 0x%X",
            //EventNames[ThisEvent.EventType], ThisEvent.EventParam);
            break;
#endif
    }

    return ReturnEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/
unsigned char ReadBumpSensors() {
    unsigned char BumpActive =
            (!(IO_PortsReadPort(BUMP_PORT) & (BUMP_PIN_1)) << 1) |
            (!(IO_PortsReadPort(BUMP_PORT) & (BUMP_PIN_2)) << 0);
    return BumpActive;
}
