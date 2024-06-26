/*
 * File: TopHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel Elkaim and Soja-Marie Morgens
 * Modified: Matthew Eng and Duc Lam
 *
 * Template file to set up a Hierarchal State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the names
 * will have to be changed to match your code.
 *
 * There is another template file for the SubHSM's that is slightly different, and
 * should be used for all of the subordinate state machines (flat or hierarchal)
 *
 * This is provided as an example and a good place to start.
 *
 * History
 * When           Who     What/Why
 * -------------- ---     --------
 * 09/13/13 15:17 ghe      added tattletail functionality and recursive calls
 * 01/15/12 11:12 jec      revisions for Gen2 framework
 * 11/07/11 11:26 jec      made the queue static
 * 10/30/11 17:59 jec      fixed references to CurrentEvent in RunTemplateSM()
 * 10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "TopHSM.h"

//#include all sub state machines called
#include "LoopSubHSM.h" 
#include "RoamSubHSM.h"
#include "DumpSubHSM.h"
#include "WallSubHSM.h"
#include "TapeDodgeSubHSM.h"
#include "WallDodgeSubHSM.h"
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//Include any defines you need to do

// Debug Defines in TopHSM.h

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/


typedef enum {
    InitPState,
    Roaming,
    Looping,
    Dumping,
    WallTracking,
    TapeDodging,
    WallDodging,
} TopHSMState_t;

static const char *StateNames[] = {
    "InitPState",
    "Roaming",
    "Looping",
    "Dumping",
    "WallTracking",
    "TapeDodging",
    "WallDodging",
};


/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine
   Example: char RunAway(uint_8 seconds);*/
/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/
/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

static TopHSMState_t CurrentState = InitPState; // <- change enum name to match ENUM
static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitTopHSM(uint8_t Priority) {
    MyPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = InitPState;
    // post the initial transition event
    if (ES_PostToService(MyPriority, INIT_EVENT) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function PostTemplateHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostTopHSM(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

/**
 * @Function RunTemplateHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the hierarchal state
 *        machine, as this is called any time a new event is passed to the event
 *        queue. This function will be called recursively to implement the correct
 *        order for a state transition to be: exit current state -> enter next state
 *        using the ES_EXIT and ES_ENTRY events.
 * @note Remember to rename to something appropriate.
 *       The lower level state machines are run first, to see if the event is dealt
 *       with there rather than at the current level. ES_EXIT and ES_ENTRY events are
 *       not consumed as these need to pass pack to the higher level state machine.
 * @author J. Edward Carryer, 2011.10.23 19:25
 * @author Gabriel H Elkaim, 2011.10.23 19:25 */
ES_Event RunTopHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    TopHSMState_t nextState; // <- change type to correct enum
    uint8_t tapeSensors;
    uint8_t obstSensors;

    ES_Tattle(); // trace call stack
#ifdef DEBUG_TOP
    printf("\r\n\r\nTOP state: %s", StateNames[CurrentState]);
    printf("\r\nevent: %s", EventNames[ThisEvent.EventType]);
    printf("\r\nparams: %x", ThisEvent.EventParam); // & 0x0F

#endif
    switch (CurrentState) {
        case InitPState: // If current state is initial Pseudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state
                // Initialize all sub-state machines
                InitLoopSubHSM();
                InitRoamSubHSM();
                InitDumpSubHSM();
                InitWallSubHSM();
                InitTapeDodgeSubHSM();
                InitWallDodgeSubHSM();
                // now put the machine into the actual initial state
                nextState = Roaming;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
#ifdef INTAKE_ACTIVE
                DT_IntakeFwd();
#endif
                DT_Stop();
                break;
            }
            break;

        case Roaming:
            ThisEvent = RunRoamSubHSM(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    break;
                case TAPE_ON:
                    tapeSensors = ~(ReadTapeSensors());
                    if (tapeSensors & FRTape) {
                        nextState = Looping;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case TRACK_ON:
                    nextState = Dumping;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    InitRoamSubHSM();
                    break;
                default:
                    break;
            }
            break;

        case Looping:
            ThisEvent = RunLoopSubHSM(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case TRACK_ON:
                    nextState = Dumping;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case OBSTACLE_ON:
                    if (ThisEvent.EventParam & (LLObstacle | FLObstacle | CLObstacle)) {
#ifdef DEBUG_TOP
                        printf("\r\nLeft Obstacle\r\n");
#endif
                        DT_DriveFwd(REV_LOW_SPEED);
                        ES_Timer_InitTimer(AVOID_TIMER, TIMER_1_SEC);
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (ThisEvent.EventParam & (RRObstacle | FRObstacle | CRObstacle)) {
#ifdef DEBUG_TOP
                        printf("\r\nRight Obstacle\r\n");
#endif
                        nextState = TapeDodging;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case WALL_ON:
                    ES_Timer_InitTimer(TOP_TIMER, TIMER_4_SEC);
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == AVOID_TIMER) { // case if obstacle on left side of bot
                        nextState = TapeDodging;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;
                    } else if (ThisEvent.EventParam == TOP_TIMER) { // Case where we hit the wall but not track
                        ES_Timer_InitTimer(REVERSE_TIMER, TIMER_1_SEC);
                        DT_DriveRight(REV_LOW_SPEED, 2000);
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;
                    } else if (ThisEvent.EventParam == REVERSE_TIMER) { // backs up before starting walltrack
                        nextState = WallTracking;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;
                    }
                    break;
                case ES_EXIT:
                    DT_Stop();
                    InitLoopSubHSM();
                    break;
                default:
                    break;
            }
            break;

        case Dumping:
            ThisEvent = RunDumpSubHSM(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    break;
                case ES_ENTRY:
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DUMP_WATCHDOG_TIMER) {
                        nextState = Roaming;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case WALLTRACK:
                    ES_Timer_StopTimer(DUMP_WATCHDOG_TIMER);
                    nextState = WallTracking;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    InitDumpSubHSM();
                    break;
                default:
                    break;
            }
            break;

        case WallTracking:
            ThisEvent = RunWallSubHSM(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    break;
                case ES_ENTRY:
                    break;
                case OBSTACLE_ON:
                    nextState = WallDodging;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case TAPE_ON:
                    if (ThisEvent.EventParam & FLTape) {
                        nextState = Looping;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (ThisEvent.EventParam & FRTape) {
                        nextState = Looping;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case TAPE_OFF:
                    if (ThisEvent.EventParam & FLTape) {
                        nextState = Looping;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_EXIT:
                    InitWallSubHSM();
                    break;
                default:
                    break;
            }
            break;

        case TapeDodging:
            ThisEvent = RunTapeDodgeSubHSM(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    break;
                case ES_ENTRY:
                    break;
                case TAPE_ON:
                    if (ThisEvent.EventParam & FLTape) {
                        nextState = Looping;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (ThisEvent.EventParam & FRTape) {
                        nextState = Looping;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case WALL_ON:
                    nextState = WallTracking;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    InitTapeDodgeSubHSM();
                    break;
            }
            break;

        case WallDodging:
            ThisEvent = RunWallDodgeSubHSM(ThisEvent);
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    break;
                case ES_ENTRY:
                    break;
                case TAPE_ON:
                    if (ThisEvent.EventParam & FLTape) {
                        nextState = Looping;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    } else if (ThisEvent.EventParam & FRTape) {
                        nextState = Looping;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case WALL_ON:
                    nextState = WallTracking;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    InitWallDodgeSubHSM();
                    break;
            }
            break;

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunTopHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunTopHSM(ENTRY_EVENT); // <- rename to your own Run function
    }
#ifdef LED_USE_TOP
    LED_SetBank(LED_BANK1, CurrentState);
#endif
    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/
