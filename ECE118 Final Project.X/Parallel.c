/*
 * File:   Parallel.c
 * Author: Matthew Eng
 *
 * Created on May 29, 2024, 7:48 PM
 */

#include "EventChecker.h"
#include "TopHSM.h"

#include "xc.h"
#include <stdio.h>
#include "Parallel.h"
#include "ES_Events.h"
#include "ES_Timers.h"
#include "serial.h"
#include "IO_Ports.h"
#include "AD.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
//#define ParallelMain
//#define DEBUG_PARALLEL
//#define INACTIVE

#ifdef ParallelMain
#include <stdio.h>
#define SaveEvent(x) do {eventName=__func__; storedEvent=x;} while (0)

static const char *eventName;
static ES_Event storedEvent;
#include <stdio.h>
static uint8_t(*EventList[])(void) = {EVENT_CHECK_LIST};
#endif

#define PARALLEL_PORT PORTY
#define PARALLEL_PIN_1 PIN9     // L
#define PARALLEL_PIN_2 PIN11    // R

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                    *
 ******************************************************************************/

/* Any private module level variable that you might need for keeping track of
   events would be placed here. Private variables should be STATIC so that they
   are limited in scope to this module. */

// initialize history of Parallel sensor to say we're off Parallel
static uint8_t prevP1 = 0;
static uint8_t prevP2 = 0;
static uint8_t Parallel_Flag = 0;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

uint8_t ReadParallelSensors() {
    uint8_t ParallelActive =
            ((IO_PortsReadPort(PARALLEL_PORT) & (PARALLEL_PIN_1)) << 1) |
            ((IO_PortsReadPort(PARALLEL_PORT) & (PARALLEL_PIN_2)) << 0);
    return ParallelActive;
}

uint8_t Parallel_Init(void) {
    IO_PortsSetPortInputs(PARALLEL_PORT, PARALLEL_PIN_1 | PARALLEL_PIN_2);
    prevP1 = 0;
    prevP2 = 0;
}

/**
 * @Function ParallelCheckEvents(void)
 * @param none
 * @return TRUE or FALSE
 * @brief This function raises PARALLEL_ON & PARALLEL_OFF events; the event params specify which Parallel sensors were triggered or untriggered
 * @author Matthew Eng, 6/11
 * */
uint8_t Parallel_CheckEvents(void) {
    ES_Event thisEvent;
    uint8_t returnVal = FALSE;
    uint8_t curP1 = (prevP1 << 1) | !!(IO_PortsReadPort(PARALLEL_PORT) & PARALLEL_PIN_1);
    uint8_t curP2 = (prevP2 << 1) | !!(IO_PortsReadPort(PARALLEL_PORT) & PARALLEL_PIN_2);

#ifdef DEBUG_PARALLEL
    if (curP1 != prevP1) printf("\r\ncurP1: %x", curP1);
    if (curP2 != prevP2) printf("\r\ncurP2: %x", curP2);
#endif
    uint8_t ParallelActive = ReadParallelSensors();
    if ((curP1 != prevP1) || (curP2 != prevP2)) {
        if (curP1 && !(prevP1)) {
            thisEvent.EventType = PARALLEL_ON_L;
            thisEvent.EventParam = 1;
            returnVal = TRUE;
#ifdef DEBUG_HSM
            printf("PARA_L\r\n");
#endif
        } else if (curP2 && !(prevP2)) {
            thisEvent.EventType = PARALLEL_ON_R;
            thisEvent.EventParam = 2;
            returnVal = TRUE;
#ifdef DEBUG_HSM
            printf("PARA_R\r\n");
#endif
        } else if ((!curP1 && prevP1) || (!curP2 && prevP2)) {
            thisEvent.EventType = PARALLEL_OFF;
            thisEvent.EventParam = 0;
            returnVal = TRUE;
#ifdef DEBUG_HSM
            printf("PARA_OFF\r\n");
#endif
        }
    }
    if (returnVal) {
#ifndef ParallelMain           // keep this as is for test harness
        PostTopHSM(thisEvent);
#else
        SaveEvent(thisEvent);
#endif 
    }
    // update history
    prevP1 = curP1;
    prevP2 = curP2;

    return (returnVal);
}
#ifdef ParallelMain

void PrintEvent(void);

void main(void) {
    BOARD_Init();
    /* user initialization code goes here */
    ES_Timer_Init();
    printf("\r\nTimer initialized");
    Parallel_Init();
    // Do not alter anything below this line
    int i;

    printf("\r\nEvent checking test harness for %s", __FILE__);

    while (1) {
        if (Parallel_CheckEvents()) {
            PrintEvent();
        }
    }
    //    while (1) {
    //        if (IsTransmitEmpty()) {
    //            for (i = 0; i< sizeof (EventList) >> 2; i++) {
    //                if (EventList[i]() == TRUE) {
    //                    PrintEvent();
    //                    break;
    //                }
    //
    //            }
    //        }
    //    }
}

void PrintEvent(void) {
    printf("\r\nFunc: %s\tEvent: %s\tParam: 0x%X", eventName,
            EventNames[storedEvent.EventType], storedEvent.EventParam);
    //    printf("\r\nCurrent Active Parallel Sensors: %x", GetCurrentParallel());
}
#endif