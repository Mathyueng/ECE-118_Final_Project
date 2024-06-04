/*
 * File: BlueloopSubHSM.c
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
#include "BlueLoopSubHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
typedef enum {
    InitPSubState,
    Turn_Left_90_1,
    Forward,
    Turn_Left_90_2,
    Forward_Half_1,
    Swivel,
    Forward_Half_2,
    Turn_Left_90_3,
    Forward_Final,
    Avoid_Right,
    Reverse,
    Avoid_Left,
} BlueLoopSubHSMState_t;

static const char *StateNames[] = {
    "InitPSubState",
    "Turn_Left_90_1",
    "Forward",
    "Turn_Left_90_2",
    "Forward_Half_1",
    "Swivel",
    "Forward_Half_2",
    "Turn_Left_90_3",
    "Forward_Final",
    "Avoid_Right",
    "Reverse",
    "Avoid_Left",
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

static BlueLoopSubHSMState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;
int Swivel_Flag = 0;


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
uint8_t InitBlueLoopSubHSM(void) {
    ES_Event returnEvent;
    LED_SetBank(LED_BANK2, 0x0);
    LED_SetBank(LED_BANK3, 0x0);
    CurrentState = InitPSubState;
    returnEvent = RunBlueLoopSubHSM(INIT_EVENT);
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
ES_Event RunBlueLoopSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    BlueLoopSubHSMState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack
    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                nextState = Turn_Left_90_1;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
                break;
            }
            break;

        case Turn_Left_90_1: // Turn 90 Left To South
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case TAPE_ON:
                    if (ThisEvent.EventParam == 6) { // FR BR (0110, 6)
                        //Logic for Turn Left 90 1 timer param
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

        case Forward: // Move Forward South
            // code for moving forward needed here
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(BLUE_LOOP_TIMER, TIMER_2_SEC);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == BLUE_LOOP_TIMER) {
                        nextState = Turn_Left_90_2;
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

        case Turn_Left_90_2: // Turn 90 Left To East
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case TAPE_ON:
                    if (ThisEvent.EventParam == 3) { // BR BL (0011, 3)
                        //Logic for Turn Left 90 2 timer param
                        nextState = Forward_Half_1;
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

        case Forward_Half_1: // Move Forward East
            // code to drive forward needed here
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(BLUE_LOOP_TIMER, TIMER_4_SEC);
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == BLUE_LOOP_TIMER) {
                        nextState = Swivel;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case PING:
                    //Logic for Turn Left 90 timer param
                    nextState = Avoid_Right;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Swivel: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    Swivel_Flag = 0;
                    ES_Timer_InitTimer(BLUE_LOOP_TIMER, TIMER_2_SEC); // bounce timer
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == BLUE_LOOP_TIMER) {
                        if (Swivel_Flag == 5) {
                            nextState = Forward_Half_2;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                        } else {
                            if ((Swivel_Flag % 2) == 0) {
                                // code to go the other direction
                            } else {
                                // code to go other other direction
                            }
                            Swivel_Flag += 1; // Flag increment
#ifdef DEBUG
                            printf("\tSwivel_Flag = %d\n", Swivel_Flag);
#endif
                            ES_Timer_InitTimer(BLUE_LOOP_TIMER, TIMER_2_SEC); // restarts timer
                        }
                    }
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Forward_Half_2: // Continue Heading East
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case TAPE_ON:
                    if (ThisEvent.EventParam == 0b1100) { // FL FR (1100, C)
                        nextState = Turn_Left_90_3;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;
                    }
                case PING:
                    //Logic for Turn Left 90 timer param
                    nextState = Avoid_Right;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Turn_Left_90_3: // Turn 90 North
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case TAPE_ON:
                    if (ThisEvent.EventParam == 6) { // FR BR (0110, 6)
                        nextState = Forward_Final;
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

        case Forward_Final: // Head North (Should be the final case, leave by TRACK_WIRE_EQUAL)
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Avoid_Right: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(BLUE_LOOP_TIMER, TIMER_4_SEC);
                    break;
                case ES_TIMEOUT:
                    //Logic for Turn Left 90 timer param
                    if (ThisEvent.EventParam == BLUE_LOOP_TIMER) {
                        nextState = Forward_Half_1;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case TAPE_ON:
                    //Logic for Tape Sensor param
                    nextState = Reverse;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Reverse: // If we hit Tape while AVOID
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case ES_TIMEOUT:
                    //Logic for Reverse timer param
                    nextState = Avoid_Left;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Avoid_Left: // Fallback Avoid if we hit Tape
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == BLUE_LOOP_TIMER) {
                        nextState = Forward;
                        makeTransition = Turn_Left_90_3;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_EXIT:
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
        RunBlueLoopSubHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunBlueLoopSubHSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    LED_SetBank(LED_BANK2, CurrentState);
    LED_SetBank(LED_BANK3, CurrentState);

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

