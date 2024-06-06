/*
 * File: RoamSubHSM.c
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
#include "RoamSubHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
typedef enum {
    InitPSubState,
    FindTape,
    Rotate_Tape,
    Forward,
    Rotate_Parallel_Wall,
    Avoid
} RoamSubHSMState_t;

static const char *StateNames[] = {
    "InitPSubState",
    "FindTape",
    "Rotate_Tape",
    "Forward",
    "Rotate_Parallel_Wall",
    "Avoid"
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

static RoamSubHSMState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;
static int Roam_Flag = 0;


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
uint8_t InitRoamSubHSM(void) {
    ES_Event returnEvent;
    LED_SetBank(LED_BANK1, 0x0);
    CurrentState = InitPSubState;
    returnEvent = RunRoamSubHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function RunRoamSubHSM(ES_Event ThisEvent)
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
ES_Event RunRoamSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to Roam_Flag transition
    RoamSubHSMState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                nextState = FindTape;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case FindTape: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    break;
                case TAPE_ON:
                    if (ThisEvent.EventParam == 0b0111) {
                        DT_Stop();
                        nextState = Rotate_Tape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    else if (ThisEvent.EventParam == 0b1011) {
                        DT_Stop();
                        nextState = Rotate_Tape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case WALL_ON:
                    DT_Stop();
                    nextState = Rotate_Parallel_Wall;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case PING:
                    nextState = Avoid;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Forward: // in the first state, replace this with correct names
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    break;
                case ES_ENTRY:
                    break;
                case TAPE_ON: // if we find corner of tape first
                    if (ThisEvent.EventParam == 0b0011) { // FL FR (0011, 3)
                        nextState = Rotate_Tape;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_EXIT:
                    DT_Stop();
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
        case Rotate_Tape: // Turn until parallel w/ Tape
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    Roam_Flag = 0;
                    ES_Timer_InitTimer(BUFFER_TIMER, TIMER_QUART_SEC);
                    DT_SpinCC(FWD_MID_SPEED); // spin til 90 degrees
                    break;
                case ES_TIMEOUT:
                    if (ThisEvent.EventParam == BUFFER_TIMER) {
                        Roam_Flag = 1;
#ifdef DEBUG
                        printf("\r\nRoam_Flag Active\r\n");
#endif
                    }
                    break;
                case TAPE_ON:
                    // Add Params here to specify
                    if (ThisEvent.EventParam == 0b0001) { // FL FR BR (0001, 1)
                        if (Roam_Flag) {
                            nextState = Forward;
                            makeTransition = TRUE;
                            ThisEvent.EventType = ES_NO_EVENT;
                        }
                    }
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Rotate_Parallel_Wall: // Turn until parallel w/ Wall
            switch (ThisEvent.EventType) {
                case ES_NO_EVENT:
                    break;
                case ES_ENTRY:
                    DT_SpinCC(REV_MID_SPEED);
                    break;
                case WALL_PARALLEL_L: // Turn till parallel w/ wall on left side
                    // Add Params here to specify 
                    nextState = Forward;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    DT_Stop();
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Avoid: // turn Left to get out of the way of the obstacle
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(BLACK_LOOP_TIMER, TIMER_4_SEC);
                    DT_DriveLeft(FWD_HI_SPEED, 11000);
                    break;
                case ES_TIMEOUT: // return to going forward
                    if (ThisEvent.EventParam == BLACK_LOOP_TIMER) {
                        nextState = Forward;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case ES_EXIT:
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
        RunRoamSubHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunRoamSubHSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    //    LED_SetBank(LED_BANK2, CurrentState);
    LED_SetBank(LED_BANK1, CurrentState);
    ES_Tail(); // trace call stack end
    return ThisEvent;
}

/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

