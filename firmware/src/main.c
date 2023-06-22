
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

#include "application.h"
#include "Protocol/protocol.h"

 /** 
     * \defgroup appMainModule  Main Module 
     * \ingroup applicationModule
     *     
     * This section implements the data structures of the module
     *  @{
     */

static uint8_t trigger_time = 0;

#define _1024_ms_TriggerTime 0x1
#define _7820_us_TriggerTime 0x2
#define _15_64_ms_TriggerTime 0x4


/**
 * This is the callback generated by the RTC module at the interrupt event.
 * 
 * @param intCause this is the cause of the interrupt event;
 * @param context this is the data pointer passed with the  RTC_Timer32CallbackRegister() routine;
 */
static void rtcEventHandler (RTC_TIMER32_INT_MASK intCause, uintptr_t context)
{
    // Periodic Interval Handler: Freq = 1024 / 2 ^ (n+3)
    
    if (intCause & RTC_TIMER32_INT_MASK_PER0) trigger_time |= _7820_us_TriggerTime;  // 7.82ms Interrupt
    if (intCause & RTC_TIMER32_INT_MASK_PER1) trigger_time |= _15_64_ms_TriggerTime; // 15.64ms Interrupt
    if (intCause & RTC_TIMER32_INT_MASK_PER7) trigger_time |= _1024_ms_TriggerTime;  // 1024 ms Interrupt
    
}

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    // Registers the RTC interrupt routine to the RTC module
    RTC_Timer32CallbackRegister(rtcEventHandler, 0);
    RTC_Timer32Start(); // Start the RTC module
    
    
    
    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        // Protocol management
        ApplicationProtocolLoop();
               
        if(trigger_time & _1024_ms_TriggerTime){
            trigger_time &=~ _1024_ms_TriggerTime;            
            
            ACTIVITY_Toggle();                      
        }        
 
        // Timer events activated into the RTC interrupt
        if(trigger_time & _7820_us_TriggerTime){
            trigger_time &=~ _7820_us_TriggerTime;

        }

        if(trigger_time & _15_64_ms_TriggerTime){
            trigger_time &=~ _15_64_ms_TriggerTime;      
 
        
        }
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

/** @}*/
/*******************************************************************************
 End of File
*/

