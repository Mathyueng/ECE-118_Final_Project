/*
 * File:   Obstacle.c
 * Author: Matthew Eng
 *
 * Created on June 11, 2024, 9:48 PM
 */

#include "EventChecker.h"
#include "TopHSM.h"

#include "xc.h"
#include <stdio.h>
#include "Obstacle.h"
#include "ES_Events.h"
#include "ES_Timers.h"
#include "serial.h"
#include "IO_Ports.h"
#include "AD.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
//#define ObstacleMain
//#define DEBUG_OBSTACLE
//#define INACTIVE

#ifdef ObstacleMain
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};
#endif

#define OBSTACLE_PORT PORTX
#define OBSTACLE_PIN_1 PIN3     // LL
#define OBSTACLE_PIN_2 PIN4     // FL
#define OBSTACLE_PIN_3 PIN5     // FR
#define OBSTACLE_PIN_4 PIN6     // RR

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/

/* Any private module level variable that you might need for keeping track of
   events would be placed here. Private variables should be STATIC so that they
   are limited in scope to this module. */

// initialize history of Obstacle sensor to say we're off Obstacle
static uint8_t prevObs1 = 0;
static uint8_t prevObs2 = 0;
static uint8_t prevObs3 = 0;
static uint8_t prevObs4 = 0;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t ReadObstacleSensors() {
    uint8_t obstacleActive =
            ((IO_PortsReadPort(OBSTACLE_PORT) & (OBSTACLE_PIN_1)) << 3) |
            ((IO_PortsReadPort(OBSTACLE_PORT) & (OBSTACLE_PIN_2)) << 2) |
            ((IO_PortsReadPort(OBSTACLE_PORT) & (OBSTACLE_PIN_3)) << 1) |
            ((IO_PortsReadPort(OBSTACLE_PORT) & (OBSTACLE_PIN_4)) << 0);
    return obstacleActive;
}

uint8_t Obstacle_Init(void) {
    IO_PortsSetPortInputs(OBSTACLE_PORT, OBSTACLE_PIN_1 | OBSTACLE_PIN_2 | OBSTACLE_PIN_3 | OBSTACLE_PIN_4);
    prevObs1 = 0;
    prevObs2 = 0;
    prevObs3 = 0;
    prevObs4 = 0;
}

/**
 * @Function ObstacleCheckEvents(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This function raises OBSTACLE_ON & OBSTACLE_OFF events; the event params specify which obstacle sensors were triggered or untriggered
 * @author Aarush Banerjee, 5/29
 * */
uint8_t Obstacle_CheckEvents(void) {
    ES_Event thisEvent;
    uint8_t curObs1 = (prevObs1 << 1) | !(IO_PortsReadPort(OBSTACLE_PORT) & OBSTACLE_PIN_1);
    uint8_t curObs2 = (prevObs2 << 1) | !(IO_PortsReadPort(OBSTACLE_PORT) & OBSTACLE_PIN_2);
    uint8_t curObs3 = (prevObs3 << 1) | !(IO_PortsReadPort(OBSTACLE_PORT) & OBSTACLE_PIN_3);
    uint8_t curObs4 = (prevObs4 << 1) | !(IO_PortsReadPort(OBSTACLE_PORT) & OBSTACLE_PIN_4);

    uint8_t returnVal = FALSE;

#ifdef DEBUG_OBSTACLE
    if (curObs1 != prevObs1) printf("\r\ncurObs1: %x", curObs1);
    if (curObs2 != prevObs2) printf("\r\ncurObs2: %x", curObs2);
    if (curObs3 != prevObs3) printf("\r\ncurObs3: %x", curObs3);
    if (curObs4 != prevObs4) printf("\r\ncurObs4: %x", curObs4);
#endif

    if ((curObs1 != prevObs1) ||
            (curObs2 != prevObs2) ||
            (curObs3 != prevObs3) ||
            (curObs4 != prevObs4)) {

        uint8_t obstacleOff =
                ((curObs1 && !prevObs1) << 3) |
                ((curObs2 && !prevObs2) << 2) |
                ((curObs3 && !prevObs3) << 1) |
                ((curObs4 && !prevObs4) << 0);

        uint8_t obstacleOn =
                ((!curObs1 && prevObs1) << 3) |
                ((!curObs2 && prevObs2) << 2) |
                ((!curObs3 && prevObs3) << 1) |
                ((!curObs4 && prevObs4) << 0);

        uint8_t obstacleActive =
                (!(IO_PortsReadPort(OBSTACLE_PORT) & (OBSTACLE_PIN_1)) << 3) |
                (!(IO_PortsReadPort(OBSTACLE_PORT) & (OBSTACLE_PIN_2)) << 2) |
                (!(IO_PortsReadPort(OBSTACLE_PORT) & (OBSTACLE_PIN_3)) << 1) |
                (!(IO_PortsReadPort(OBSTACLE_PORT) & (OBSTACLE_PIN_4)) << 0);

        if (obstacleOn) {
            ES_Event thisEvent;
            thisEvent.EventType = OBSTACLE_ON;
#ifdef INACTIVE
            thisEvent.EventParam = obstacleOn;
#else
            thisEvent.EventParam = obstacleActive;
#endif

#ifndef ObstacleMain           // keep this as is for test harness
            PostTopHSM(thisEvent);
#else
            SaveEvent(thisEvent);
#endif   
            returnVal = TRUE;
        }

        // OBSTACLE_OFF detection
        if (obstacleOff) {
            ES_Event thisEvent;
            thisEvent.EventType = OBSTACLE_OFF;
#ifdef INACTIVE
            thisEvent.EventParam = obstacleOff; //  & 0x0F
#else
            thisEvent.EventParam = obstacleActive;
#endif

#ifndef ObstacleMain           // keep this as is for test harness
            PostTopHSM(thisEvent);
#else
            SaveEvent(thisEvent);
#endif 
            returnVal = TRUE;
        }
    }

    // update history
    prevObs1 = curObs1;
    prevObs2 = curObs2;
    prevObs3 = curObs3;
    prevObs4 = curObs4;

    return (returnVal);
}
#ifdef ObstacleMain


void PrintEvent(void);

void main(void) {
    BOARD_Init();
    /* user initialization code goes here */
    ES_Timer_Init();
    printf("\r\nTimer initialized");
    Obstacle_Init();
    // Do not alter anything below this line
    int i;

    printf("\r\nEvent checking test harness for %s", __FILE__);

    while (1) {
        if (Obstacle_CheckEvents()) {
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
    //    printf("\r\nCurrent Active Obstacle Sensors: %x", GetCurrentObstacle());
}
#endif