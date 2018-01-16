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

#define COLLISION_PERIOD                   52
#define COLLISION_COUNTER                  51
#define IDLE_PERIOD                        830
#define IDLE_COUNTER                       829

//States include: 
//Busy - signals that the channel monitor is busy
//       and in either line high or line low 
//Collision - collision detected, line high
//Idle - initialized state looking for rising
//       edge to occur
enum state { BUSY, COLLISION, IDLE } systemState;

_Bool logicLevel = 0;
char currentDataCharacter; //current char
int bufferPosition = 0; //serial_pos
unsigned char buffer[500]; //serial buffer

// data converted to non-terminated string showing 1s and 0s
unsigned char data[16];

int dataSize; //data size

// Symbol position in unipolar RZ signal. There are 16 symbols, 2 per bit.
int symbolPosition = 0;

int count; //dataConvertedReadOutCount

// bit pos in transmitted character (0 is MSB, 6 is LSB, excludes start bit)
int idx = 0;
    
    
/*******************************************************************************
* Define Interrupt service routine and allocate an vector to the Interrupt
********************************************************************************/

 /**********************************************************
 * function name: ReceiveInterruptHandler
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
CY_ISR(ReceiveInterruptHandler)
{
   	TimerRX_STATUS; 
    if (logicLevel) {
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
        TimerRX_WritePeriod(COLLISION_PERIOD);
        TimerRX_WriteCounter(COLLISION_COUNTER);
        TimerRX_Start();
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
        TimerRX_WritePeriod(IDLE_PERIOD);
        TimerRX_WriteCounter(IDLE_COUNTER);
        TimerRX_Start();
        logicLevel = 0;
        systemState = BUSY;
    }
}




 /**********************************************************
 * function name: TransmitInterruptHandler
 * operation: Handles interrupt for transmition of data
 * inputs: none
 * returns: none
 * implemented: 10 Jan 2018
 * edited:
 *********************************************************/
CY_ISR(TransmitInterruptHandler)
{
    TimerTX_STATUS;
	currentDataCharacter = buffer[bufferPosition];  
	if(bufferPosition < dataSize) { 
        // Always send 1 in first part of start bit.
	    if(symbolPosition == 0) {
    		TRANSMIT_Write(1);
    		data[symbolPosition] = '1';
        // At second half of each bit, return to 0.
    	} else if(symbolPosition % 2 != 0){
    		TRANSMIT_Write(0);
    		data[symbolPosition] = '0';
        // All other cases, high or low depending on bit
    	} else {
    		if(currentDataCharacter & (1 << (6 - idx))){
    			TRANSMIT_Write(1);
    			data[symbolPosition] = '1';
    		}else{
    			TRANSMIT_Write(0);
    			data[symbolPosition] = '0';   
    		}
    		++idx;
    	}
        CyDelayUs(450);
        ++symbolPosition;
        if(symbolPosition >= 16) {
    	    ++bufferPosition;
    	    idx = 0;
    	    symbolPosition = 0;
            TRANSMIT_Write(0);
        }
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
    //systemState = IDLE;
    CyGlobalIntEnable;
    RisingEdgeISR_StartEx(RisingEdgeInterruptHandler);
    FallingEdgeISR_StartEx(FallingEdgeInterruptHandler);
    TimerTX_Start();
    TimerTX_WritePeriod(45);
    TimerTX_WriteCounter(44);
    ReceiveISR_StartEx(ReceiveInterruptHandler);
    TransmitISR_StartEx(TransmitInterruptHandler);

    
    USBUART_Start(USBUART_device, USBUART_5V_OPERATION);
    

    
    while (1) {
        
        //Ensure device is connected and configured 
        if (0u != USBUART_IsConfigurationChanged())
        {           
            // Initialize IN endpoints
            if (0u != USBUART_GetConfiguration())
            {
                // Enable OUT endpoint to receive data from host
                USBUART_CDC_Init();
            }
        }      
        
        
        if(USBUART_DataIsReady() != 0){
            dataSize = USBUART_GetAll(buffer);
            while(!USBUART_CDCIsReady());
            USBUART_PutCRLF();
            count = 0;            
            bufferPosition = 0;
        }

        if(USBUART_CDCIsReady() != 0){
            if(count < dataSize && bufferPosition == dataSize){
                for(int y = 0; y < 16; y++){
                 while(!USBUART_CDCIsReady());
                 USBUART_PutChar(data[count][y]); 
                }
                ++count;
                while(!USBUART_CDCIsReady());
                USBUART_PutCRLF();
            }
        }
    }
}

        
//    while(1) {
//        switch(systemState){
//            case IDLE:
//                IDLE_Write(1);
//                BUSY_Write(!IDLE_Read());
//                COLLISION_Write(!IDLE_Read());
//            break;
//            
//            case BUSY:
//                BUSY_Write(1);
//                IDLE_Write(!BUSY_Read());
//                COLLISION_Write(!BUSY_Read());
//            break;
//            
//            case COLLISION:
//                COLLISION_Write(1);
//                IDLE_Write(!COLLISION_Read());
//                BUSY_Write(!COLLISION_Read());
//            break;
//        }
//    }
   


/* [] END OF FILE */
