/*
 * File: DodgeSubHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim, Matthew Eng
 *
 * Template file to set up a Hierarchical State Machine to work with the Events and
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
 * 10/30/11 17:59 jec      fixed references to CurrentEvent in RunDodgeSM()
 * 10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
 */


/*******************************************************************************
 * MODULE #INCLUDE                                                             *
 ******************************************************************************/

#include "ES_Configure.h"
#include "ES_Framework.h"
#include "BOARD.h"
#include "TopHSM.h"
#include "DodgeSubHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/

#define BANK_RIGHT_SPEED    40
#define BANK_RIGHT_RADIUS   2000

typedef enum {
    InitPSubState,
    SetupLeft,
    BankRight,
    TurnLeft,
} DodgeSubHSMState_t;

static const char *StateNames[] = {
    "InitPSubState",
    "SetupLeft",
    "BankRight",
    "TurnLeft",
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

static DodgeSubHSMState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function InitDodgeSubHSM(uint8_t Priority)
 * @param Priority - internal variable to track which event queue to use
 * @return TRUE or FALSE
 * @brief This will get called by the framework at the beginning of the code
 *        execution. It will post an ES_INIT event to the appropriate event
 *        queue, which will be handled inside RunTemplateFSM function. Remember
 *        to rename this to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t InitDodgeSubHSM(void) {
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunDodgeSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function RunDodgeSubHSM(ES_Event ThisEvent)
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
ES_Event RunDodgeSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    DodgeSubHSMState_t nextState; // <- change type to correct enum
    uint8_t obstacleSensors = ReadObstacleSensors();
    uint8_t tapeSensors;
    ES_Tattle(); // trace call stack

#ifdef DEBUG_DODGE
    printf("\r\n\r\nDODGE state: %s", StateNames[CurrentState]);
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
                nextState = SetupLeft;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case SetupLeft:
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    break;
                case ES_ENTRY:
                    DT_DriveRight(-BANK_RIGHT_SPEED, 0);
                    ES_Timer_InitTimer(AVOID_TIMER, TIMER_1_SEC);
                    break;
                case TAPE_ON:
                    tapeSensors = ~(ReadTapeSensors());
                    if (tapeSensors & BLTape) {
                        ES_Timer_StopTimer(AVOID_TIMER);
                        nextState = BankRight;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == AVOID_TIMER) {
                        nextState = BankRight;
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

        case BankRight:
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    break;
                case ES_ENTRY:
                    DT_DriveRight(BANK_RIGHT_SPEED, BANK_RIGHT_RADIUS);
                    break;
                case OBSTACLE_ON:
                    nextState = TurnLeft;
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

        case TurnLeft:
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    break;
                case ES_ENTRY:
                    ES_Timer_InitTimer(AVOID_TIMER, TIMER_3_SEC);
                    DT_DriveRight(-BANK_RIGHT_SPEED, 0);
                    break;
                case OBSTACLE_OFF:
                    ES_Timer_StopTimer(AVOID_TIMER);
                    ES_Timer_InitTimer(AVOID_TIMER, TIMER_HALF_SEC);
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == AVOID_TIMER) {
                        nextState = BankRight;
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

        default: // all unhandled states fall into here
            break;
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunDodgeSubHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunDodgeSubHSM(ENTRY_EVENT); // <- rename to your own Run function
    }
#ifdef LED_USE_DODGE
    LED_SetBank(LED_BANK1, CurrentState);
#endif
    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

