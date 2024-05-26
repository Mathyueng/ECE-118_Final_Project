/*
 * File:   ping.c
 * Author: aarushbanerjee
 *
 * Created on May 6, 2024, 6:42 PM
 */


#include "xc.h"
#include "ping.h"
#include "IO_Ports.h"
#include "ES_Timers.h"
#include "ES_Configure.h"

unsigned int FreeRunningTimer;

#define NOPS_FOR_10US 1020
#define PING_10US
#define PING_1MS

#define D_OVER_EW_RATIO 1000 / 148 // D/EW, adjusted for taking in ms instead of us
#define PING_SENSOR_PORT PORTY
#define TRIGGER_PIN PIN3
#define ECHO_PIN PIN4
#define PING_TIMER 

#define TIMER_uS_PER_TICK 25
static uint16_t start_time;
static uint8_t trigger_flag = 0;
static uint8_t echo_flag = 0;
/* 
 * @definition:
 * called by ES_TIMEOUT events for respective PING_TIMER
 * this function triggers the ping sensor if inactive. 
 * if active, it keeps count until called when ECHO goes low
 * @params:
 * no parameters
 * @return:
 * this function will return 1 when no value is available
 * when it has a distance it has detected from the ping sensor, it will return that distance in increments of 6-7 inches
 */
uint8_t PingAndReportDistance() { // function for Problem 6, part c
    if (!(IO_PortsReadPort(PING_SENSOR_PORT) & ECHO_PIN)) { // if the echo pin is low
        IO_PortsWritePort(PING_SENSOR_PORT, TRIGGER_PIN); // set output pin TRIGGER to 1
        ES_Timer_InitTimer(1,1); // set a 1ms timer on timer 1
        trigger_flag = 1; // raise trigger flag while we're in trigger period
        
    } else if (trigger_flag) { // if we've raised the trigger for 1ms
        trigger_flag = 0; // stop the triggering
        IO_PortsClearPortBits(PING_SENSOR_PORT, 0xFF); // clear the TRIGGER pin
        ES_Timer_InitTimer(1,1); // set a timer to call the function back in 1ms
        
    } else if (!(echo_flag)) { // if we're not yet in the echo stage
        if (IO_PortsReadPort(PING_SENSOR_PORT) & ECHO_PIN) {  // if the echo pin goes high
            start_time = FreeRunningTimer; // record the time at which we start counting
            echo_flag = 1; // raise the echo flag
        }
        ES_Timer_InitTimer(1,1); // set a timer to call the function back in 1ms
        
    } else { // in the echo stage
        // when echo goes low, return time elapsed, converted to distance
        if (!(IO_PortsReadPort(PING_SENSOR_PORT) & ECHO_PIN)) return /*ReadTimer1() * */D_OVER_EW_RATIO / (1000 / TIMER_uS_PER_TICK);
        ES_Timer_InitTimer(1,1); // set a timer to call the function back in 1ms
    }
    return 1;
}
    
    
    
    