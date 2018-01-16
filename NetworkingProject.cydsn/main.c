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
#define IDLE_LEVEL 0

//States include: 
//Busy - signals that the channel monitor is busy
//       and in either line high or line low 
//Collision - collision detected, line high
//Idle - initialized state looking for rising
//       edge to occur
enum state { BUSY, COLLISION, IDLE } systemState;

bool logicLevel = 0;

int idx = 0; //Bit position
bool firstHalfBit;
char *dataPtr;
bool collisionDelay = false;
    
    
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
}

/**

*/
CY_ISR(TransmitInterruptHandler)
{
    TimerTX_STATUS;
    if (*dataPtr == '\0') {
        TRANSMIT_Write(IDLE_LEVEL);
        TransmitISR_Stop();
    } else {
        if (firstHalfBit) {
            if (idx == 7) {
                TRANSMIT_Write(1);
            } else {
                // Sets the level to the current bit.
                TRANSMIT_Write((*dataPtr & (1 << idx)) ? 1 : 0);
            }
            firstHalfBit = false;
        } else {
            TRANSMIT_Write(0);
            if (idx == 0) {
                idx = 7;
                dataPtr++;
            } else {
                idx--;
            }
            firstHalfBit = true;
        }
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
        RisingEdgeISR_ClearPending();
        TimerRX_Stop();
        TimerRX_WritePeriod(COLLISION_PERIOD);
        TimerRX_WriteCounter(COLLISION_COUNTER);
        TimerRX_Start();
        logicLevel = 1;
        systemState = BUSY;
        BUSY_Write(1);
        COLLISION_Write(0);
        IDLE_Write(0);
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
        TimerRX_Stop();
        TimerRX_WritePeriod(IDLE_PERIOD);
        TimerRX_WriteCounter(IDLE_COUNTER);
        TimerRX_Start();
        logicLevel = 0;
        systemState = BUSY;
        BUSY_Write(1);
        COLLISION_Write(0);
        IDLE_Write(0);
    }
}

/**
This runs at the end of the interrupt.
*/
CY_ISR(CollisionInterruptHandler)
{
    collisionDelay = false;
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
    CyGlobalIntEnable; //enable global interrupts
    //starting the interrupt handlers
    RisingEdgeISR_StartEx(RisingEdgeInterruptHandler);
    FallingEdgeISR_StartEx(FallingEdgeInterruptHandler);
    ReceiveISR_StartEx(ReceiveInterruptHandler);  
    USBUART_Start(USBUART_device, USBUART_5V_OPERATION); //configuring USBUART to start
    int delay = 500; //delay of 500ms, configured later through random and uniformly distributed to be 0-1s
    int dataSize = 0; //size of retrieved data to then be transmitted
    int dataPosition = 0; //position looping through the data to transmit
    _Bool endOfData = 0; //boolean to determine if the entire characters has been received
    unsigned char buffer[10]; //buffer used to hold single blob of data 
    char data[44 + 1]; //designed to transmit a message as long as 44 characters
    bool endOfTransmission = 0; //boolean sigifying complete transmission of data 
    char binaryOfChar[8 + 1]; //represents the 8 bit representation of the character 
        
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
        
        while (endOfData == 0) {
            // Check for input data
            if (0u != USBUART_DataIsReady())
            {
                // Read received data
                dataSize = USBUART_GetAll(buffer);
                
                if (0u != dataSize) 
                {
                    for(int i=0; i<dataSize; i++)
                    {
                        if((buffer[i] == '\r') || (buffer[i] == '\n'))
                        {
                            endOfData = true;
                            USBUART_PutCRLF();
                            USBUART_PutString("Enter is pressed");
                        }
                        else
                        {
                            data[dataPosition]=buffer[i];
                            dataPosition++;
                        }
                    }                  
                }
            }
        }
        
        data[dataPosition]='\0'; // Writes an ending null terminator char.
        
        dataPtr = data;
        firstHalfBit = true;
        idx = 7;
        
        TransmitISR_StartEx(TransmitInterruptHandler);
        while (systemState != IDLE); // Wait for lines to be idle
        while (!endOfTransmission);
        /*    if (systemState == COLLISION){
        }*///TODO Add collision detection logic
        
        /*
        dataPosition = 0;
        TRANSMIT_Write(0);
        
        while (systemState != IDLE);
        
        while (!endOfTransmission) 
        {     
            switch(systemState)
            {
                case IDLE:
                    IDLE_Write(1);
                    BUSY_Write(!IDLE_Read());
                    COLLISION_Write(!IDLE_Read());
                    if((*(data+dataPosition)) != '\0')
                    {
                        TRANSMIT_Write(1);
                        CyDelay(delay);
                        TRANSMIT_Write(0);
                        CyDelay(delay);
                        
                        itoa((*(data+dataPosition)), binaryOfChar, 2);
                        for (int i = 0; i < 7; ++i) 
                        {
                            if(binaryOfChar[i] == '1')
                            {
                                TRANSMIT_Write(1);
                                CyDelay(500);
                                TRANSMIT_Write(0);
                                CyDelay(delay);                                                       
                            } else {
                                TRANSMIT_Write(0);
                                CyDelay(2*delay);
                            }
                        }
                        ++dataPosition;
                    } else {
                        endOfTransmission = 1;
                    }
                                       
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
        */
    }
}
