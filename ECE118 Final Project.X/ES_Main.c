#include <BOARD.h>
#include <xc.h>
#include <stdio.h>
#include "ES_Configure.h"
#include "ES_Framework.h"

#include "AD.h"
#include "LED.h"
#include "pwm.h"
#include "RC_Servo.h"
#include "serial.h"
#include "timers.h"
#include "EventChecker.h"
void main(void)
{
    ES_Return_t ErrorType;

    BOARD_Init();
    printf("Starting ES Framework Template\r\n");
    printf("using the 2nd Generation Events & Services Framework\r\n");


    // Your hardware initialization function calls go here
    AD_Init();
    LED_Init();
    PWM_Init();
    RC_Init();
    TIMERS_Init();
    InitAllEventCheckers();
    DT_Init();
#ifdef LED_USE
    LED_AddBanks (LED_BANK1);
//    LED_AddBanks (LED_BANK2);
//    LED_AddBanks (LED_BANK3);

    LED_SetBank(LED_BANK1, 0x0);
//    LED_SetBank(LED_BANK2, 0x0);
//    LED_SetBank(LED_BANK3, 0x0);
#endif
    printf("All Inits complete.\r\n");
    // now initialize the Events and Services Framework and start it running
    ErrorType = ES_Initialize();
    if (ErrorType == Success) {
        ErrorType = ES_Run();
    }
    //if we got to here, there was an error
    switch (ErrorType) {
    case FailedPointer:
        printf("Failed on NULL pointer");
        break;
    case FailedInit:
        printf("Failed Initialization");
        break;
    default:
        printf("Other Failure: %d", ErrorType);
        break;
    }
    for (;;)
        ;

};

/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/
