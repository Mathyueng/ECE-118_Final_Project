#include <xc.h>
#include <sys/attribs.h> 
#include <string.h>
#include <stdio.h>
#include "FreeRunningTimer.h"
#include "BOARD.h"

//#define FRT_INTERRUPT

// macros
#define MICROSECOND_ROLLOVER 4294967000
    // 4294967000 is the highest multiple of 1000 that can be represented with 32 bits
    // used multiples of 1000 so that the rollover didn't create an offset
#define MILLISECOND_ROLLOVER 4294967000
#define TICKS_PER_SECOND 40

// variables
static unsigned int microseconds;
static unsigned int milliseconds;

/**
 * @Function FreeRunningTimer_Init(void)
 * @param none
 * @return None.
 * @brief  Initializes the timer module */
void FreeRunningTimer_Init(void) {
    
    T5CON = 0; // turn off T, select internal clk, 1:1 pre scalar
    
    // Period Register
    PR5 = 40001; // PRx = 40MHz/(desired * PreScalar) + 1
    
    // interrupts
    IFS0bits.T5IF = 0; // clear the flag
    IPC5bits.T5IP = 2; // priority bit
    IPC5bits.T5IS = 0; // subpriority bit
    IEC0bits.T5IE = 1; // enable interrupts
    
    T5CONbits.ON = 1; // turn on 
    
}

/**
 * Function: FreeRunningTimer_GetMilliSeconds
 * @param None
 * @return the current MilliSecond Count
   */
unsigned int FreeRunningTimer_GetMilliSeconds(void) {return milliseconds;}

/**
 * Function: FreeRunningTimer_GetMicroSeconds
 * @param None
 * @return the current MicroSecond Count
   */
unsigned int FreeRunningTimer_GetMicroSeconds(void) {return (microseconds + (TMR5 / TICKS_PER_SECOND));}

#ifdef FRT_INTERRUPT
void __ISR(_TIMER_5_VECTOR) Timer5IntHandler(void) {
    
    if (IFS0bits.T5IF) {
        IFS0bits.T5IF = 0; // clear the flag
        
        LEDS_SET(LEDS_GET() ^ 0x10);
        
        // counters
        milliseconds++;           // milliseconds increment
        microseconds += 1000;     // microsecond increment

        // rollovers
        if (microseconds >= MICROSECOND_ROLLOVER) microseconds = 0; // microseconds rollover
        if (microseconds >= MILLISECOND_ROLLOVER) milliseconds = 0; // milliseconds rollover
        
    }
    
}
#endif FRT_INTERRUPT