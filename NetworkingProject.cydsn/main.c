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
#include <stdbool.h>
#include "stdio.h"

#define COLLISION_PERIOD                   52
#define COLLISION_COUNTER                  51
#define IDLE_PERIOD                        830
#define IDLE_COUNTER                       829

#define RX_BUFFER_SIZE                     32

#define NULL_PAD                           1

#define MAX_RX_HIGH_TIME_CLK               52
#define EXPECTED_SYMBOL_TIME_CLK           50
#define MIN_RX_HIGH_TIME_CLK               48

#define NO_BIT                             8
#define START_BIT                          7


//States include: 
//Busy - signals that the channel monitor is busy
//       and in either line high or line low 
//Collision - collision detected, line high
//Idle - initialized state looking for rising
//       edge to occur
typedef enum { BUSY, COLLISION, IDLE } State;
State systemState;

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

/******************************************************
Receiving character logic variables
******************************************************/
//Which RX bit occurred during last HI period. 8 if no bit received yet.
uint8_t rxBit = 8;
// Contains status of most recent capture
typedef enum {DATA_RISING, DATA_FALLING, DATA_NONE, DATA_ERROR} EdgeStatus;
EdgeStatus edgeDataStatus = DATA_NONE;
uint16_t lastCapture; // Contains most recent capture
// Buffer for receiving text.
// TODO Adjust size of buffer. See if buffer is even necessary.
//char rxBuffer[RX_BUFFER_SIZE + NULL_PAD];
//char *rxBufferPos; // Current char position.
char currRxChar;
bool timerWrapped;
    
    
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
        COLLISION_Write(1);
        IDLE_Write(0);
        BUSY_Write(0);
    } else {
        systemState = IDLE;
        COLLISION_Write(0);
        IDLE_Write(1);
        BUSY_Write(0);
    }
    timerWrapped = true;
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
        RisingEdgeISR_ClearPending();
        TimerRX_Sleep();
        lastCapture = TimerRX_ReadCounter();
        TimerRX_WritePeriod(COLLISION_PERIOD);
        TimerRX_WriteCounter(COLLISION_COUNTER);
        TimerRX_Start();
        logicLevel = 1;
        systemState = BUSY;
        BUSY_Write(1);
        COLLISION_Write(0);
        IDLE_Write(0);
        timerWrapped = false;
        if (edgeDataStatus == DATA_NONE) {
            edgeDataStatus = DATA_RISING;
        } else {
            edgeDataStatus = DATA_ERROR;
        }
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
    if (logicLevel) {
        FallingEdgeISR_ClearPending();
        TimerRX_Sleep();
        lastCapture = TimerRX_ReadCounter();
        TimerRX_WritePeriod(IDLE_PERIOD);
        TimerRX_WriteCounter(IDLE_COUNTER);
        TimerRX_Start();
        logicLevel = 0;
        systemState = BUSY;
        BUSY_Write(1);
        COLLISION_Write(0);
        IDLE_Write(0);
        timerWrapped = false;
        if (edgeDataStatus == DATA_NONE) {
            edgeDataStatus = DATA_FALLING;
        } else {
            edgeDataStatus = DATA_ERROR;
        }
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
//    TimerTX_Start();
//    TimerTX_WritePeriod(45);
//    TimerTX_WriteCounter(44);
    ReceiveISR_StartEx(ReceiveInterruptHandler);
//    TransmitISR_StartEx(TransmitInterruptHandler);

    // rxBufferPos = rxBuffer;
    USBUART_Start(USBUART_device, USBUART_5V_OPERATION);
    while (!USBUART_GetConfiguration()) {
    }
    USBUART_CDC_Init();
    
    while(!(USBUART_GetDTERate() == 57600));
	
	//Host can send double SET_INTERFACE request, which sounds sub-optimal for us if we don't handle that
	while (!USBUART_IsConfigurationChanged());
	//re-initialize device
	while (!USBUART_GetConfiguration());
	//Enumeration is done, allow receieving data from host
	USBUART_CDC_Init();
    while(!USBUART_CDCIsReady());
    USBUART_PutChar('!');
    
    while (true) {
        int uartConnected = 0;
        if(USBUART_GetDTERate() == 57600){
			uartConnected = 1;
		}
	
		//Host can send double SET_INTERFACE request, which sounds sub-optimal for us if we don't handle that
		if (0 != USBUART_IsConfigurationChanged())
		{
			//re-initialize device
			if (0 != USBUART_GetConfiguration())
			{
				//Enumeration is done, allow receieving data from host
				USBUART_CDC_Init();
			}
		}
        while(!USBUART_CDCIsReady());
        USBUART_PutString("Source Address: ");
        State localState = systemState; // Save in case of preemption.
        // TODO Make this code conditional so it doesn't check
        // for a new bit unless something special happened.
        // This code should only run when an edge was detected.
        // TODO Make other code for when state changes to COLLISION
        // or IDLE.
        
        // Save this locally in case of preemption.
        if (localState == BUSY) {
            if (edgeDataStatus != DATA_NONE) {
                bool localTimerWrapped = timerWrapped;
                EdgeStatus localEdgeStatus = edgeDataStatus;
                uint16_t inputCaptureTime = lastCapture;
                edgeDataStatus = DATA_NONE;
                // Error detection. Caused by too many successive IRQs.
                if (localEdgeStatus == DATA_ERROR) {
                    // Error state. Stop interpreting received data until idle.
                    rxBit = NO_BIT;
                    currRxChar = '\0';
                    while (systemState != IDLE) {}
                    edgeDataStatus = DATA_NONE; // Sanity check
                    TimerRX_ClearFIFO();
                // This code should do bit shifting (or byte shifting if necessary)
                // on the rising edge.
                // On the falling edge, after proper high length was confirmed, the
                // appropriate bit should be set.
                } else if (localEdgeStatus == DATA_RISING) { // Was last low
                    if (rxBit == 8) {
                        rxBit--;
                        continue;
                    }
                    if (localTimerWrapped) {
                        rxBit = NO_BIT;
                        USBUART_PutChar(currRxChar);
                        currRxChar = '\0';
                        continue;
                    }
                    int timeClk = IDLE_COUNTER - inputCaptureTime;
                    int symbolClkCycles = timeClk % EXPECTED_SYMBOL_TIME_CLK;
                    // Find out the nearest rounding of symbol time.
                    int nearestNumSymbols = timeClk / EXPECTED_SYMBOL_TIME_CLK
                    + ((symbolClkCycles >= EXPECTED_SYMBOL_TIME_CLK / 2) ? 1 : 0);
                    int bitsToShift = nearestNumSymbols / 2;
                    if (bitsToShift > rxBit) {
                        rxBit = START_BIT;
                        USBUART_PutChar(currRxChar);
                        currRxChar = '\0';
//                        rxBufferPos++;
//                        *rxBufferPos = '\0'; // Nullify new buffer pos.
                        continue;
                    }
                    // Improper number of symbols between highs.
                    if ((nearestNumSymbols <= 0)
                    || (nearestNumSymbols % 2 == 0) // Even num of symbols
                    // Below appropraite min
                    || (timeClk < 
                     nearestNumSymbols * MIN_RX_HIGH_TIME_CLK) 
                    // Below appropriate min
                    || (timeClk > 
                     nearestNumSymbols * MAX_RX_HIGH_TIME_CLK)) {
                        // Error state.
                        int x = nearestNumSymbols;
                        timeClk;
                        rxBit = NO_BIT;
                        currRxChar = '\0';
                        // *rxBufferPos = '\0';
                        while (systemState != IDLE) {}
                    } else {
                        // Will round down properly, since 0.5 is truncated.
                        rxBit -= bitsToShift;
                    }
                } else { // Was last high.
                    int timeClk = COLLISION_COUNTER - inputCaptureTime;
                    if ((timeClk >= MIN_RX_HIGH_TIME_CLK) && !localTimerWrapped) {
                        currRxChar |= (1 << rxBit);
                        USBUART_PutChar(rxBit+0x30);
                        // *rxBufferPos |= (1 << rxBit);
                    }
                }
            }
        } else if (localState == IDLE) {
            // If idle, finish off current bit.
            if (rxBit != NO_BIT) {
                USBUART_PutChar(currRxChar);
                currRxChar = '\0';
                // rxBufferPos++;
                // *rxBufferPos = '\0';
                rxBit = NO_BIT;
            }
        } else {
            // If collision, STOP trying to receive current bit.
            rxBit = NO_BIT;
            currRxChar = '\0';
            // *rxBufferPos = '\0'; // Clear current char data. Is bad.
        }
        
    }
    return 0;

// This was the original transmit code. Not for milestone 3.    
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
//        
//        if(USBUART_DataIsReady() != 0){
//            dataSize = USBUART_GetAll(buffer);
//            while(!USBUART_CDCIsReady());
//            USBUART_PutCRLF();
//            count = 0;            
//            bufferPosition = 0;
//        }
//
//        if(USBUART_CDCIsReady() != 0){
//            if(count < dataSize && bufferPosition == dataSize){
//                for(int y = 0; y < 16; y++){
//                 while(!USBUART_CDCIsReady());
//                 USBUART_PutChar(data[count][y]); 
//                }
//                ++count;
//                while(!USBUART_CDCIsReady());
//                USBUART_PutCRLF();
//            }
//        }
//    }
//}

        
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
   
}

/* [] END OF FILE */
