/*
 * File: DumpSubHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim, Duc Lam, Matthew Eng
 *
 * Template file to set up a Heirarchical State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that
 * this file will need to be modified to fit your exact needs, and most of the names
 * will have to be changed to match your code.
 *
 * There is for a substate machine. Make sure it has a unique name
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
#include "DumpSubHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
#define TURN_SPEED          60
#define BANK_RIGHT_SPEED    30
#define BANK_RIGHT_RADIUS   11000

//#define LOOP_TEST

// Debug Defines in TopHSM.h

typedef enum {
    InitPSubState,
    Reverse,
    LowerArm,
    Forward,
    Dump,
    LeftTurn,
    BankRight,
    BankLeft,
    LeftSideBuffer,
    SpinLeft
} DumpSubHSMState_t;

static const char *StateNames[] = {
    "InitPSubState",
    "Reverse",
    "LowerArm",
    "Forward",
    "Dump",
    "LeftTurn",
    "BankRight",
    "BankLeft",
    "LeftSideBuffer",
    "SpinLeft"
};



/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES                                                 *
 ******************************************************************************/
/* Prototypes for private functions for this machine. They should be functions
   relevant to the behavior of this state machine */

/*******************************************************************************
 * PRIVATE MODULE VARIABLES                                                            *
 ******************************************************************************/
/* You will need MyPriority and the state variable; you may need others as well.
 * The type of state variable should match that of enum in header file. */

static DumpSubHSMState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitTemplateSubHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitDumpSubHSM(void) {
    ES_Event returnEvent;
    LED_SetBank(LED_BANK2, 0x0);
    CurrentState = InitPSubState;
    returnEvent = RunDumpSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function RunTemplateSubHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be responded.
 * @return Event - return event (type and param), in general should be ES_NO_EVENT
 * @brief This function is where you implement the whole of the heirarchical state
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
ES_Event RunDumpSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    DumpSubHSMState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack
#ifdef DEBUG_DUMP
    printf("\r\n\r\nDUMP state: %s", StateNames[CurrentState]);
    printf("\r\nevent: %s", EventNames[ThisEvent.EventType]);
    printf("\r\nparams: %x", ThisEvent.EventParam & 0x0F);
#endif
    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
#ifndef LOOP_TEST
                nextState = Reverse;
#else
                nextState = LeftTurn; // Testing Looping
#endif
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
                break;
            }
            break;

        case Reverse:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
#ifdef DEBUG_HSM
                    printf("DUMP - Reverse 1\r\n");
#endif                    
                    ES_Timer_InitTimer(DUMP_TIMER, TIMER_1_SEC);
                    DT_DriveFwd(REV_LOW_SPEED);
                    break;
                    //#ifndef TRACK_ACTIVE
                    //                case WALL_OFF:
                    //#else
                    //                case TRACK_OFF:
                    //#endif
                    //                    ThisEvent.EventType = ES_NO_EVENT;
                    //                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DUMP_TIMER) {
                        nextState = LowerArm;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_EXIT:
                    DT_Stop();
                    break;
                case ES_NO_EVENT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case LowerArm: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(DUMP_TIMER, TIMER_1_SEC);
                    DT_ExtendArm();
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DUMP_TIMER) {
                        nextState = Forward;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Forward:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    DT_DriveFwd(FWD_LOW_SPEED);
                    ES_Timer_InitTimer(DUMP_TIMER, TIMER_1_SEC);
                    break;
                    //#ifndef TRACK_ACTIVE
                    //                case WALL_ON:
                    //#else
                    //                case TRACK_ON:
                    //#endif
                    //                    nextState = Dump;
                    //                    makeTransition = TRUE;
                    //                    ThisEvent.EventType = ES_NO_EVENT;
                    //                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DUMP_TIMER) {
                        nextState = Dump;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_EXIT:
                    DT_Stop();
                    break;
                default:
                    break;
            }
            break;

        case Dump:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
#ifdef INTAKE_ACTIVE
                    DT_IntakeBack();
#endif
                    DT_Stop();
                    ES_Timer_InitTimer(DUMP_TIMER, TIMER_3_SEC);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DUMP_TIMER) {
                        nextState = LeftTurn;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_EXIT:
#ifdef INTAKE_ACTIVE
                    DT_IntakeFwd();
#endif
                    break;
                default:
                    break;
            }
            break;
        case LeftTurn:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    DT_RetractArm();
                    DT_DriveRight(-20, 0);
                    break;
                case TAPE_ON:
                    if (ThisEvent.EventParam & BLTape) {
                        nextState = BankRight;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_EXIT:
                    DT_Stop();
                    break;
                case ES_NO_EVENT:
                    break;
                default:
                    break;
            }
            break;

        case BankRight:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    DT_DriveRight(BANK_RIGHT_SPEED, BANK_RIGHT_RADIUS);
                    break;
                case PARALLEL_ON_R:
                    nextState = BankLeft;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case WALL_ON:
                    nextState = SpinLeft;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    DT_Stop();
                    break;
                default:
                    break;
            }
            break;

        case BankLeft:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    DT_DriveRight(-BANK_RIGHT_SPEED, 0);
                    break;
                case PARALLEL_OFF:
                    nextState = LeftSideBuffer;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    DT_Stop();
                    break;
                default:
                    break;
            }
            break;

        case LeftSideBuffer:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(DUMP_TIMER, TIMER_BUFFER);
                    break;
                case ES_NO_EVENT:
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == DUMP_TIMER) {
                        nextState = BankRight;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_EXIT:
                    break;
                default:
                    break;
            }
            break;

        case SpinLeft: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    DT_SpinCC(TURN_SPEED);
                    //                    DT_DriveRight(-BANK_RIGHT_SPEED, 2000);
                    break;
                case WALL_OFF:
                    nextState = LeftSideBuffer;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case PARALLEL_ON_R:
                    nextState = BankLeft;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_NO_EVENT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunDumpSubHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunDumpSubHSM(ENTRY_EVENT); // <- rename to your own Run function
    }
#ifdef LED_USE
    LED_SetBank(LED_BANK1, CurrentState);
#endif
    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/