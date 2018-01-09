/* ========================================
 * FILENAME: main.c
 * AUTHORS: Tyler Wells, Andy Winston, 
            Kamith Mirissage
 * DATE: 11 DEC 2017
 * PROVIDES:
 * - A PSOC implementation of a system 
 *   that fucntions as a message-exchange 
 *   on CSMA/CD bus using unipolar return
 *   to zero (RZ) line coding 
 * - State machine with  channel monitor
 *   implementation (BUSY/IDLE/COLLISION)
 * - Software pushbutton debouncing used 
 * - LED to signal different states
 * - Interrupt handling for rising and 
  *  edge triggers of a timer
 * ========================================
*/

#include "project.h"
#include <stdlib.h>
#include "stdio.h"

//States include: 
//Busy - signals that the channel monitor is busy
//       and in either line high or line low 
//Collision - collision detected, line high
//Idle - initialized state looking for rising
//       edge to occur
enum state { BUSY, COLLISION, IDLE } systemState;

_Bool logicLevel = 0;

#define COLLISION_PERIOD                   52
#define COLLISION_COUNTER                  51
#define IDLE_PERIOD                        830
#define IDLE_COUNTER                       829
#define SIZE_OF_TRANSMITTED_DATA           352


/*******************************************************************************
* Define Interrupt service routine and allocate an vector to the Interrupt
********************************************************************************/

 /**********************************************************
 * function name: TimerInterruptHandler
 * operation: Handles interrupt timer if the timer expires
 *            indicates an idle condition or colllision
 *            reads the logic levon the input pin/signal
 *            high -> collision set
 *            low -> idle set
 * inputs: none
 * returns: none
 * implemented: 14 Dec 2017
 * edited:
 *********************************************************/ 
CY_ISR(TimerInterruptHandler)
{
   	TIMER1_STATUS; 
    if (logicLevel){
        systemState = COLLISION;
    } else {
        systemState = IDLE;
    }
}


 /**********************************************************
 * function name: RisingEdgeInterruptHandler
 * operation: Handles interrupt for when a rising edge on 
 *            the timer occurs. Triggers system to look for
 *            falling edge (1 -> 0). Schedules a timer
 *            interupt to occur in .6ms and sets state to 
 *            busy
 * inputs: none
 * returns: none
 * implemented: 14 Dec 2017
 * edited:
 *********************************************************/ 
CY_ISR(RisingEdgeInterruptHandler)
{
    if ((!logicLevel)){
        TIMER1_WritePeriod(COLLISION_PERIOD);
        TIMER1_WriteCounter(COLLISION_COUNTER);
        TIMER1_Start();
        logicLevel = 1;
        systemState = BUSY;
    }
}
 

 /**********************************************************
 * function name: FallingEdgeInterruptHandler
 * operation: Handles interrupt for when a falling edge on 
 *            the timer occurs. Triggers system to look for
 *            rising edge (0 -> 1). Schedules a timer
 *            interupt to occur in 7.7 ms and sets state to 
 *            busy
 * inputs: none
 * returns: none
 * implemented: 14 Dec 2017
 * edited:
 *********************************************************/ 
CY_ISR(FallingEdgeInterruptHandler)
{
    if (logicLevel){
        TIMER1_WritePeriod(IDLE_PERIOD);
        TIMER1_WriteCounter(IDLE_COUNTER);
        TIMER1_Start();
        logicLevel = 0;
        systemState = BUSY;
    }
}

 /**********************************************************
 * function name: main
 * operation: Handles the main state machine diagram for the
 *            system. Changes  states when needed and calls
 *            appropriate interrupt routines when triggered.                
 * inputs: none
 * returns: none
 * implemented: 14 Dec 2017
 * edited:
 *********************************************************/ 
int main(void)
{

    systemState = IDLE;
    CyGlobalIntEnable;
    TimerISR_StartEx(TimerInterruptHandler);
    RisingEdgeISR_StartEx(RisingEdgeInterruptHandler);
    FallingEdgeISR_StartEx(FallingEdgeInterruptHandler);
//    USBUART_Start(USBUART_device, USBUART_5V_OPERATION);
// 
//    int delay = 500;   
//    char input[44];
//    int index = 0;
//    int count = 0;
//    unsigned char buffer[SIZE_OF_TRANSMITTED_DATA];
//    
//    while (1) {
//        
//        //Ensure device is connected and configured 
//        if (0u != USBUART_IsConfigurationChanged())
//        {           
//            // Initialize IN endpoints
//            if (0u != USBUART_GetConfiguration())
//            {
//                // Enable OUT endpoint to receive data from host
//                USBUART_CDC_Init();
//            }
//        }
//        
//        //Check for input data is ready 
//        if (0u != USBUART_DataIsReady())
//        {
//            count = USBUART_GetAll(buffer);
//            
//            //Read recevied data
//            if (0u != count)
//            {
//                while (USBUART_CDCIsReady() == 0u);
//                USBUART_PutData(buffer,count);
//            }
//        }
//    }

    while(1) {
        switch(systemState){
            case IDLE:
                IDLE_Write(1);
                BUSY_Write(!IDLE_Read());
                COLLISION_Write(!IDLE_Read());
            break;
            
            case BUSY:
                BUSY_Write(1);
                IDLE_Write(!BUSY_Read());
                COLLISION_Write(!BUSY_Read());
            break;
            
            case COLLISION:
                COLLISION_Write(1);
                IDLE_Write(!COLLISION_Read());
                BUSY_Write(!COLLISION_Read());
            break;
        }
    }
    
}


/* [] END OF FILE */
