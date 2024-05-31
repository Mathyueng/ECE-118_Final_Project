/*
 * File: BlueloopSubHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim
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
            }
            break;

        case Turn_Left_90_1: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case ES_TIMEOUT:
                    //Logic for Turn Left 90 1 timer param
                    nextState = Forward;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case ES_EXIT:
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Forward: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case ES_TIMEOUT:
                    //Logic for Forward timer param
                    nextState = Turn_Left_90_2;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case ES_EXIT:
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Turn_Left_90_2: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case ES_TIMEOUT:
                    //Logic for Turn Left 90 2 timer param
                    nextState = Forward_Half_1;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case ES_EXIT:
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Forward_Half_1: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case ES_TIMEOUT:
                    //Logic for Half Way Timer param
                    nextState = Swivel;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case PING:
                    //Logic for Turn Left 90 timer param
                    nextState = Avoid_Right;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case ES_EXIT:
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Swivel: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case ES_TIMEOUT:
                    //Logic for Swivel timer param
                    nextState = Forward_Half_2;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case ES_EXIT:
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Forward_Half_2: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case TAPE_ON:
                    //Logic for TAPE_ON param
                    nextState = Turn_Left_90_3;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case PING:
                    //Logic for Turn Left 90 timer param
                    nextState = Avoid_Right;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case ES_EXIT:
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Turn_Left_90_3: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case ES_TIMEOUT:
                    //Logic for Turn Left 90 3 timer param
                    nextState = Forward_Final;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case ES_EXIT:
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Forward_Final: // Should be the final case, leave by TRACK_WIRE_EQUAL
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
                    break;
                case ES_TIMEOUT:
                    //Logic for Turn Left 90 timer param
                    nextState = Forward_Half_1;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case TAPE_ON:
                    //Logic for Tape Sensor param
                    nextState = Reverse;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case ES_EXIT:
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Reverse: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case ES_TIMEOUT:
                    //Logic for Reverse timer param
                    nextState = Avoid_Left;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                case ES_EXIT:
                    break;
                case ES_NO_EVENT:
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Avoid_Left: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    break;
                case ES_TIMEOUT:
                    //Logic for Avoid Left timer param
                    nextState = Forward;
                    makeTransition = Turn_Left_90_3;
                    ThisEvent.EventType = ES_NO_EVENT;
                case ES_EXIT:
                    break;
                case ES_NO_EVENT:
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

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

