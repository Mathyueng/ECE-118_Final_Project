/*
 * File: TopHSM.h
 * Author: J. Edward Carryer
 * Modified: Gabriel H Elkaim, Duc Lam, Matthew Eng
 *
 * Template file to set up a Heirarchical State Machine to work with the Events and
 * Services Framework (ES_Framework) on the Uno32 for the CMPE-118/L class. Note that 
 * this file will need to be modified to fit your exact needs, and most of the names
 * will have to be changed to match your code.
 *
 * There is another template file for the SubHSM's that is slightly differet, and
 * should be used for all of the subordinate state machines (flat or heirarchical)
 *
 * This is provided as an example and a good place to start.
 *
 * Created on 23/Oct/2011
 * Updated on 16/Sep/2013
 */

#ifndef HSM_Top_H  // <- This should be changed to your own guard on both
#define HSM_Top_H  //    of these lines


/*******************************************************************************
 * PUBLIC #INCLUDES                                                            *
 ******************************************************************************/

#include "xc.h"
#include "stdio.h"
#include "LED.h"

#include "EventChecker.h"       // Event Checker Files
#include "Tape.h"
#include "Wall.h"
#include "Parallel.h"

#include "ES_Configure.h"       // defines ES_Event, INIT_EVENT, ENTRY_EVENT, and EXIT_EVENT
#include "Tank_DriveTrain.h"    // defines Drive Train Functions
#include "ES_Events.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#define TIMER_0_TICKS   5       // 0.0005 s
#define TIMER_BUFFER    100     // 0.001 s
#define TIMER_QUART_SEC 250     // 0.25 s
#define TIMER_HALF_SEC  500     // 0.5 s
#define TIMER_1_SEC     1000    // 1 s
#define TIMER_2_SEC     2000    // 2 s
#define TIMER_3_SEC     3000    // 3 s
#define TIMER_4_SEC     4000    // 4 s
#define TIMER_5_SEC     5000    // 5 s
#define TIMER_6_SEC     6000    // 6 s
#define TIMER_7_SEC     7000    // 7 s
#define TIMER_8_SEC     8000    // 8 s
#define TIMER_9_SEC     9000    // 9 s
#define TIMER_10_SEC    10000   // 10 s


#define DEBUG_TOP
//#define DEBUG_ROAM
//#define DEBUG_LOOP
#define DEBUG_DUMP

#define INTAKE_ACTIVE

#define TRACK_ACTIVE
/*******************************************************************************
 * PUBLIC TYPEDEFS                                                             *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
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
uint8_t InitTopHSM(uint8_t Priority);


/**
 * @Function PostTemplateHSM(ES_Event ThisEvent)
 * @param ThisEvent - the event (type and param) to be posted to queue
 * @return TRUE or FALSE
 * @brief This function is a wrapper to the queue posting function, and its name
 *        will be used inside ES_Configure to point to which queue events should
 *        be posted to. Remember to rename to something appropriate.
 *        Returns TRUE if successful, FALSE otherwise
 * @author J. Edward Carryer, 2011.10.23 19:25 */
uint8_t PostTopHSM(ES_Event ThisEvent);




/**
 * @Function RunTemplateHSM(ES_Event ThisEvent)
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
ES_Event RunTopHSM(ES_Event ThisEvent);

#endif /* HSM_Top_H */

