/*
 * File:   Wall.c
 * Author: Matthew Eng
 *
 * Created on 02 Jun 2024, 11:27 PM
 */


#include "xc.h"
#include "Wall.h"
#include "ES_Configure.h"
#include "ES_Events.h"
#include "serial.h"
#include "IO_Ports.h"
#include "AD.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define WALL_PORT PORTZ
#define WALL_PIN_1 PIN7

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/

/* Any private module level variable that you might need for keeping track of
   events would be placed here. Private variables should be STATIC so that they
   are limited in scope to this module. */

// initialize history of wall sensor to say we're off wall
static uint8_t prevW1 = 0;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t Wall_Init(void) {
    IO_PortsSetPortInputs(WALL_PORT, WALL_PIN_1 | (IO_PortsReadPort(WALL_PORT)));
    prevW1 = 0;
}

/**
 * @Function WallCheckEvents(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This function raises WALL_ON & WALL_OFF events; the event params specify which wall sensors were triggered or untriggered
 * @author Aarush Banerjee, 5/29
 * */
uint8_t Wall_CheckEvents(void) {

    uint8_t curW1 = (prevW1 << 1) | !!(IO_PortsReadPort(WALL_PORT) & WALL_PIN_1);
    
    uint8_t returnVal = FALSE;

    uint8_t wallOff = 
            (curW1 && !prevW1);
    
    uint8_t wallOn =
            (!curW1 && prevW1);
    
    if (wallOn) {
        ES_Event thisEvent;
        thisEvent.EventType = WALL_ON;
        thisEvent.EventParam = wallOn;
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif   
        returnVal = TRUE;
    }
    
    // WALL_OFF detection
    if (wallOff) {
        ES_Event thisEvent;
        thisEvent.EventType = WALL_OFF;
        thisEvent.EventParam = wallOff & 0x0F;
#ifndef EVENTCHECKER_TEST           // keep this as is for test harness
//        PostGenericService(thisEvent);
#else
        SaveEvent(thisEvent);
#endif 
        returnVal = TRUE;
    }
    
    // update history
    prevW1 = curW1;

    return (returnVal);
}
