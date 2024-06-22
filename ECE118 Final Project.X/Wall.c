/*
 * File:   wall.c
 * Author: aanbaner
 *
 * Created on May 29, 2024, 7:48 PM
 */

#include "EventChecker.h"
#include "TopHSM.h"

#include "xc.h"
#include <stdio.h>
#include "wall.h"
#include "ES_Events.h"
#include "ES_Timers.h"
#include "serial.h"
#include "IO_Ports.h"
#include "AD.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
//#define WallMain
//#define DEBUG_WALL_CHECK
//#define INACTIVE

#ifdef WallMain
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};
#endif

#define WALL_PORT PORTZ
#define WALL_PIN_1 PIN7     // LL
#define WALL_PIN_2 PIN8     // FL
#define WALL_PIN_3 PIN9     // FR
#define WALL_PIN_4 PIN11    // RR

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/

/* Any private module level variable that you might need for keeping track of
   events would be placed here. Private variables should be STATIC so that they
   are limited in scope to this module. */

// initialize history of wall sensor to say we're off wall
static uint8_t prevW1 = 0;
static uint8_t prevW2 = 0;
static uint8_t prevW3 = 0;
static uint8_t prevW4 = 0;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t ReadWallSensors() {
    uint8_t wallActive =
            ((IO_PortsReadPort(WALL_PORT) & (WALL_PIN_1)) << 3) |
            ((IO_PortsReadPort(WALL_PORT) & (WALL_PIN_2)) << 2) |
            ((IO_PortsReadPort(WALL_PORT) & (WALL_PIN_3)) << 1) |
            ((IO_PortsReadPort(WALL_PORT) & (WALL_PIN_4)) << 0);
    return wallActive;
}

uint8_t Wall_Init(void) {
    IO_PortsSetPortInputs(WALL_PORT, WALL_PIN_1 | WALL_PIN_2 | WALL_PIN_3 | WALL_PIN_4);
    prevW1 = 0;
    prevW2 = 0;
    prevW3 = 0;
    prevW4 = 0;
}

/**
 * @Function WallCheckEvents(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This function raises WALL_ON & WALL_OFF events; the event params specify which wall sensors were triggered or untriggered
 * @author Aarush Banerjee, 5/29
 * */
uint8_t Wall_CheckEvents(void) {
    ES_Event thisEvent;
    uint8_t curW1 = (prevW1 << 1) | !!(IO_PortsReadPort(WALL_PORT) & WALL_PIN_1);
    uint8_t curW2 = (prevW2 << 1) | !!(IO_PortsReadPort(WALL_PORT) & WALL_PIN_2);
    uint8_t curW3 = (prevW3 << 1) | !!(IO_PortsReadPort(WALL_PORT) & WALL_PIN_3);
    uint8_t curW4 = (prevW4 << 1) | !!(IO_PortsReadPort(WALL_PORT) & WALL_PIN_4);

    uint8_t returnVal = FALSE;

#ifdef DEBUG_WALL_CHECK
//    printf("\r\ncurW2: %x, prevW2: %x", (IO_PortsReadPort(WALL_PORT) & WALL_PIN_2), prevW2);
    if (curW1 != prevW1) printf("\r\ncurW1: %x", curW1);
    if (curW2 != prevW2) printf("\r\ncurW2: %x", curW2);
    if (curW3 != prevW3) printf("\r\ncurW3: %x", curW3);
    if (curW4 != prevW4) printf("\r\ncurW4: %x", curW4);
#endif
    
    if ((curW1 != prevW1) ||
            (curW2 != prevW2) ||
            (curW3 != prevW3) ||
            (curW4 != prevW4)) {
        
        uint8_t wallOff =
                ((curW1 && !prevW1) << 3) |
                ((curW2 && !prevW2) << 2) |
                ((curW3 && !prevW3) << 1) |
                ((curW4 && !prevW4) << 0);

        uint8_t wallOn =
                ((!curW1 && prevW1) << 3) |
                ((!curW2 && prevW2) << 2) |
                ((!curW3 && prevW3) << 1) |
                ((!curW4 && prevW4) << 0);

        uint8_t wallActive =
                (!(IO_PortsReadPort(WALL_PORT) & (WALL_PIN_1)) << 3) |
                (!(IO_PortsReadPort(WALL_PORT) & (WALL_PIN_2)) << 2) |
                (!(IO_PortsReadPort(WALL_PORT) & (WALL_PIN_3)) << 1) |
                (!(IO_PortsReadPort(WALL_PORT) & (WALL_PIN_4)) << 0);

        if (wallOn) {
            ES_Event thisEvent;
            thisEvent.EventType = WALL_ON;
            thisEvent.EventParam = wallActive;
#ifdef DEBUG_HSM
            printf("WALL_ON\r\n");
#endif

#ifndef WallMain           // keep this as is for test harness
            PostTopHSM(thisEvent);
#else
            SaveEvent(thisEvent);
#endif   
            returnVal = TRUE;
        }

        // WALL_OFF detection
        if (wallOff) {
            ES_Event thisEvent;
            thisEvent.EventType = WALL_OFF;
            thisEvent.EventParam = wallActive;
#ifdef DEBUG_HSM
            printf("WALL_OFF\r\n");
#endif
#ifndef WallMain           // keep this as is for test harness
            PostTopHSM(thisEvent);
#else
            SaveEvent(thisEvent);
#endif 
            returnVal = TRUE;
        }
    }

    // update history
    prevW1 = curW1;
    prevW2 = curW2;
    prevW3 = curW3;
    prevW4 = curW4;

    return (returnVal);
}
#ifdef WallMain


void PrintEvent(void);

void main(void) {
    BOARD_Init();
    /* user initialization code goes here */
    ES_Timer_Init();
    printf("\r\nTimer initialized");
    Wall_Init();
    // Do not alter anything below this line
    int i;

    printf("\r\nEvent checking test harness for %s", __FILE__);

    while (1) {
        if (Wall_CheckEvents()) {
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
    //    printf("\r\nCurrent Active Wall Sensors: %x", GetCurrentWall());
}
#endif