/*
 * File: LoopSubHSM.c
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim, Matthew Eng, and Duc Lam ;)
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
#include "LoopSubHSM.h"

/*******************************************************************************
 * MODULE #DEFINES                                                             *
 ******************************************************************************/
typedef enum {
    InitPSubState,
    Forward_East,
    Forward_West,
    Turn_180_Right,
    Turn_180_Left,
    Turn_90_Left,
    Avoid,
} LoopSubHSMState_t;

static const char *StateNames[] = {
	"InitPSubState",
	"Forward_East",
	"Forward_West",
	"Turn_180_Right",
	"Turn_180_Left",
	"Turn_90_Left",
	"Avoid",
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

static LoopSubHSMState_t CurrentState = InitPSubState; // <- change name to match ENUM
static uint8_t MyPriority;
static int Direction; //the current direction of the robot
static int Loop_ctr; //the amounts of loops traversed


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
uint8_t InitLoopSubHSM(void) {
    ES_Event returnEvent;
    LED_SetBank(LED_BANK3, 0x0);
    CurrentState = InitPSubState;
    returnEvent = RunLoopSubHSM(INIT_EVENT);
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
ES_Event RunLoopSubHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    LoopSubHSMState_t nextState; // <- change type to correct enum

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitPSubState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                nextState = Forward_West;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
                break;
            }
            break;

        case Forward_West:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    Direction = 0;
                    DT_DriveLeft(30, 4000);
                    break;
                case WALL_ON:
                    if (ThisEvent.EventParam == 0b0110) {
                        DT_DriveFwd(50);
                    }
                    break;
                case WALL_OFF:
                    DT_SetLeftDrive(70);
                    DT_SetRightDrive(60);
                    break;
                case TAPE_ON:
                    if (ThisEvent.EventParam == 0b1100) { // FL FR (1100, )
                        //Logic for Turn Left 90 2 timer param
                        nextState = Turn_180_Left;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case OBSTACLE_EVENT:
                    nextState = Avoid;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Turn_180_Left:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(LOOP_TIMER, TIMER_2_SEC);
                    DT_DriveRight(FWD_HI_SPEED, 11000);
                    break;
                case ES_TIMEOUT: // return to going forward
                    if (ThisEvent.EventParam == LOOP_TIMER) {
                        nextState = Forward_East;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case OBSTACLE_EVENT:
                    nextState = Avoid;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Forward_East:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    Direction = 1;
                    DT_DriveRight(-30, 4000);
                    Loop_ctr++;
                    break;
                case WALL_ON:
                    if (ThisEvent.EventParam == 0b1001) { // FL BL (1001, 9)
                        DT_DriveFwd(50);
                    }
                    break;
                case WALL_OFF:
                    DT_SetRightDrive(70);
                    DT_SetLeftDrive(60);
                    break;
                case TAPE_ON:
                    if (ThisEvent.EventParam == 0b1100) { // FL FR (1100, )
                        if (Loop_ctr < 3)
                            nextState = Turn_180_Right;
                        else
                            nextState = Turn_90_Left;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case OBSTACLE_EVENT:
                    nextState = Avoid;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Turn_180_Right:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(LOOP_TIMER, TIMER_2_SEC);
                    DT_DriveLeft(FWD_HI_SPEED, 11000);
                    break;
                case ES_TIMEOUT: // return to going forward
                    if (ThisEvent.EventParam == LOOP_TIMER) {
                        nextState = Forward_West;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;
                case OBSTACLE_EVENT:
                    nextState = Avoid;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Turn_90_Left:
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(LOOP_TIMER, TIMER_2_SEC);
                    DT_DriveRight(FWD_HI_SPEED, 11000);
                    break;
                case ES_EXIT:
                    break;
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Avoid: // turn Left to get out of the way of the obstacle
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    ES_Timer_InitTimer(LOOP_TIMER, TIMER_4_SEC);
                    DT_DriveLeft(FWD_HI_SPEED, 11000);
                    break;
                case ES_TIMEOUT: // return to going forward
                    if (ThisEvent.EventParam == LOOP_TIMER) {
                        nextState = Forward_East; // alter for when we do sensor testing
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
        RunLoopSubHSM(EXIT_EVENT); // <- rename to your own Run function
        CurrentState = nextState;
        RunLoopSubHSM(ENTRY_EVENT); // <- rename to your own Run function
    }

    LED_SetBank(LED_BANK3, CurrentState);

    ES_Tail(); // trace call stack end
    return ThisEvent;
}


/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/