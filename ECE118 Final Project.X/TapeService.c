/*
 * File: TapeService.h
 * Author: J. Edward Carryer
 * Modified: Matthew Eng
 *
 * File to set up a simple service to work with the Events and Services
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
#include "TapeService.h"
#include "IO_Ports.h"
#include "LED.h"
#include <stdio.h>

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

//#define DEBUG

#define ALL_PINS 0x07F8 // 0b0000 0111 1111 1000 All Digital Pins
#define NONETRIPPED 0 // 8'b 0000 0000
#define TIMER_0_TICKS 500     // milliseconds

#define FRONT_LEFT_DIR      PORTZ03_TRIS
#define FRONT_FRONT_DIR     PORTZ04_TRIS
#define FRONT_RIGHT_DIR     PORTZ05_TRIS
#define SIDE_RIGHT_DIR      PORTZ06_TRIS
#define BACK_RIGHT_DIR      PORTZ07_TRIS
#define BACK_BACK_DIR       PORTZ08_TRIS
#define BACK_LEFT_DIR       PORTZ09_TRIS
#define SIDE_LEFT_DIR       PORTZ10_TRIS

#define FRONT_LEFT_IN      PORTZ03_LAT
#define FRONT_FRONT_IN     PORTZ04_LAT
#define FRONT_RIGHT_IN     PORTZ05_LAT
#define SIDE_RIGHT_IN      PORTZ06_LAT
#define BACK_RIGHT_IN      PORTZ07_LAT
#define BACK_BACK_IN       PORTZ08_LAT
#define BACK_LEFT_IN       PORTZ09_LAT
#define SIDE_LEFT_IN       PORTZ10_LAT

#define FRONT_LEFT_READ     PORTZ03_BIT
#define FRONT_FRONT_READ    PORTZ04_BIT
#define FRONT_RIGHT_READ    PORTZ05_BIT
#define SIDE_RIGHT_READ     PORTZ06_BIT
#define BACK_RIGHT_READ     PORTZ07_BIT
#define BACK_BACK_READ      PORTZ08_BIT
#define BACK_LEFT_READ      PORTZ09_BIT
#define SIDE_LEFT_READ      PORTZ10_BIT


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
unsigned char ReadTapeSensors(void) {
    unsigned int F_L = FRONT_LEFT_READ;
    unsigned int F_F = FRONT_FRONT_READ;             // NOTES HERE
    unsigned int F_R = FRONT_RIGHT_READ;
    unsigned int S_R = SIDE_RIGHT_READ;           // fl ff fr
    unsigned int B_R = BACK_RIGHT_READ;           // sl    sr
    unsigned int B_B = BACK_BACK_READ;            // bl bb br
    unsigned int B_L = BACK_LEFT_READ;
    unsigned int S_L = SIDE_LEFT_READ;
    unsigned char out = (!F_L+((!F_F) << 1)+((!F_R) << 2)+((!S_R) << 3)+((!B_R) << 4)+((!B_B) << 5)+((!B_L) << 6)+((!S_L) << 7));
    return (out);
}
/**
 * @Function InitTapeService(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTapeService function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitTapeService(uint8_t Priority)
{
    ES_Event ThisEvent;

    MyPriority = Priority;

    // in here you write your initialization code
    // this includes all hardware and software initialization
    // that needs to occur.

    // post the initial transition event
    ThisEvent.EventType = ES_INIT;
    
    FRONT_LEFT_DIR = 1;
    FRONT_FRONT_DIR = 1;
    FRONT_RIGHT_DIR = 1;
    SIDE_RIGHT_DIR = 1;
    BACK_RIGHT_DIR = 1;
    BACK_BACK_DIR = 1;
    BACK_LEFT_DIR = 1;
    SIDE_LEFT_DIR = 1;

    
    LED_AddBanks(LED_BANK1);    
    LED_AddBanks(LED_BANK2);
    LED_AddBanks(LED_BANK3);
    
#ifdef DEBUG
    printf("\r\nStarting Tape Service\r\n");
#endif
//    ES_Timer_InitTimer(TAPE_SERVICE_TIMER, TIMER_0_TICKS);
    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function PostTapeService(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostTapeService(ES_Event ThisEvent)
{
    return ES_PostToService(MyPriority, ThisEvent);
}

/**
 * @Function RunTapeService(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the service,
 *        as this is called any time a new event is passed to the event queue. 
 * @note Remember to rename to something appropriate.
 *       Returns ES_NO_EVENT if the event have been "consumed." 
 * @author J. Edward Carryer, 2011.10.23 19:25 */
ES_Event RunTapeService(ES_Event ThisEvent)
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

    case ES_TIMEOUT:
//        ES_Timer_InitTimer(TAPE_SERVICE_TIMER, TIMER_0_TICKS);
        TapeSensorCheck();
        
#ifndef SIMPLESERVICE_TEST           // keep this as is for test harness
        //    PostGenericService(ReturnEvent);  // Set to SM Post
#else
            PostTapeService(ReturnEvent);   // Set to SM Post
#endif   
        break;
    }


#ifdef SIMPLESERVICE_TEST     // keep this as is for test harness      
    default:
        printf("\r\nEvent: %s\tParam: 0x%X",
                EventNames[ThisEvent.EventType], ThisEvent.EventParam);
        break;
#endif
}
static uint8_t prevT1 = 0;
static uint8_t prevT2 = 0;
static uint8_t prevT3 = 0;
static uint8_t prevT4 = 0;
void TapeSensorCheck() {
    ES_Event ReturnEvent;
    ReturnEvent.EventType = ES_NO_EVENT;
    
    uint8_t curT1, curT2, curT3, curT4;
    int F_L = FRONT_LEFT_READ;
    int F_R = FRONT_RIGHT_READ;
    int B_R = BACK_RIGHT_READ;
    int B_L = BACK_LEFT_READ;
#ifdef DEBUG
    printf("\r\n F_L: %d... \r\n F_R: %d... \r\n B_R: %d... \r\n B_L: %d... ", F_L, F_R, B_R, B_L);
#endif
    curT1 = ((prevT1 << 1) | (!F_L));
    curT2 = ((prevT2 << 1) | (!F_R));
    curT3 = ((prevT3 << 1) | (!B_R));
    curT4 = ((prevT4 << 1) | (!B_L));
    
    // TAPE_ON
    if ((curT1) && (!prevT1)) {
        printf("TAPE_ON_1");
        ReturnEvent.EventType = TAPE_ON;
        ReturnEvent.EventParam = 1;
    }
    if ((curT2) && (!prevT2)) {
        printf("TAPE_ON_2");
        ReturnEvent.EventType = TAPE_ON;
        ReturnEvent.EventParam = 2;        
    }
    if ((curT3) && (!prevT3)) {
        printf("TAPE_ON_3");
        ReturnEvent.EventType = TAPE_ON;
        ReturnEvent.EventParam = 3;        
    }
    if ((curT4) && (!prevT4)) {
        printf("TAPE_ON_4");
        ReturnEvent.EventType = TAPE_ON;
        ReturnEvent.EventParam = 4;        
    }
    
    // TAPE_OFF
    if ((!curT1) && (prevT1)) {
        printf("TAPE_OFF_1");
        ReturnEvent.EventType = TAPE_OFF;
        ReturnEvent.EventParam = 5;        
    }
    if ((!curT2) && (prevT2)) {
        printf("TAPE_OFF_2");
        ReturnEvent.EventType = TAPE_OFF;
        ReturnEvent.EventParam = 6;          
    }
    if ((!curT3) && (prevT3)) {
        printf("TAPE_OFF_3");
        ReturnEvent.EventType = TAPE_OFF;
        ReturnEvent.EventParam = 7;          
    }
    if ((!curT4) && (prevT4)) {
        printf("TAPE_OFF_4");
        ReturnEvent.EventType = TAPE_OFF;
        ReturnEvent.EventParam = 8;          
    }
        
#ifndef SIMPLESERVICE_TEST           // keep this as is for test harness
    //    PostGenericService(ReturnEvent);  // Set to SM Post
#else
        PostTapeService(ReturnEvent);   // Set to SM Post
#endif   
}


/*******************************************************************************
 * PRIVATE FUNCTIONs                                                           *
 ******************************************************************************/