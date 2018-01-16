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

//States include: 
//Busy - signals that the channel monitor is busy
//       and in either line high or line low 
//Collision - collision detected, line high
//Idle - initialized state looking for rising
//       edge to occur
enum state { BUSY, COLLISION, IDLE } systemState;

int logicLevel = 0;
int idx = 0; //count
int delay = 500; //delay of 500ms, configured later through random and uniformly distributed to be 0-1s
char data[45]; //designed to transmit a message as long as 44 characters 
char *dataPtr; // Pointer to current character in data.
bool firstHalfBit; // Flag for which part of bit we're in.
char binaryOfChar[8]; //represents the 8 bit representation of the character 
bool endOfTransmission;
bool collisionDuringTransmit = false;
    
    
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
        collisionDuringTransmit = true;
        COLLISION_Write(1);
        BUSY_Write(0);
        IDLE_Write(0);
    } else {
        systemState = IDLE;
        COLLISION_Write(0);
        BUSY_Write(0);
        IDLE_Write(1);
    }
}


CY_ISR(TransmitInterruptHandler)
{
    TimerTX_STATUS;
    if (*dataPtr == '\0') {
        TRANSMIT_Write(0);
        TransmitISR_Stop();
        endOfTransmission = true;
    } else {
        if (systemState == COLLISION) {
            /*TransmitISR_Stop();
            TimerTX_Stop();
            collisionDuringTransmit = true;
            idx = 7;
            firstHalfBit = true;
            TRANSMIT_Write(0);*/
            return;
        }
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
        TimerRX_Stop();
        TimerRX_WritePeriod(COLLISION_PERIOD);
        TimerRX_WriteCounter(COLLISION_COUNTER);
        TimerRX_Start();
        logicLevel = 1;
        systemState = BUSY;
        COLLISION_Write(0);
        BUSY_Write(1);
        IDLE_Write(0);
    } else {
        
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
        TimerRX_Stop();
        TimerRX_WritePeriod(IDLE_PERIOD);
        TimerRX_WriteCounter(IDLE_COUNTER);
        TimerRX_Start();
        logicLevel = 0;
        systemState = BUSY;
        COLLISION_Write(0);
        BUSY_Write(1);
        IDLE_Write(0);
    } 
    else {
    } 
}


int getInputData(char *);



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
    TRANSMIT_Write(0);
    systemState = IDLE;
        
    while (1) {  
        
        int size = getInputData(data);
        data[size] = '\0'; // Null termination at CR.
        
        dataPtr = data;
        firstHalfBit = true;
        idx = 7;
        
        // int index = 0;
        endOfTransmission = 0; //boolean sigifying complete transmission of data
        
        while (systemState != IDLE);
        TransmitISR_StartEx(TransmitInterruptHandler);
        TimerTX_Start();
        while (!endOfTransmission) {
            if (collisionDuringTransmit) {
                TransmitISR_Stop();
                TimerTX_Stop();
                TRANSMIT_Write(0);
                idx = 7;
                firstHalfBit = true;
                delay = (rand() % 1000) + 1;
                CyDelay(delay);
               // CyDelay(delay);
                collisionDuringTransmit = false;
                while (systemState != IDLE);
                TransmitISR_StartEx(TransmitInterruptHandler);
                TimerTX_Start();
            }
        }
        endOfTransmission = false;
        /*while (!endOfTransmission) 
        { */    
//            switch(systemState)
//            {
//                case IDLE:
//                    IDLE_Write(1);
//                    BUSY_Write(!IDLE_Read());
//                    COLLISION_Write(!IDLE_Read());
//                    if((*(data+index)) != '\0')
//                    {
//                        TRANSMIT_Write(1);
//                        CyDelayUs(delay);
//                        TRANSMIT_Write(0);
//                        CyDelayUs(delay);
//                        
//                        itoa((*(data+index)), binaryOfChar, 2);
//                        for (int i = 0; i < 7; ++i) 
//                        {
//                            if(binaryOfChar[i] == '1')
//                            {
//                                TRANSMIT_Write(1);
//                                CyDelayUs(500);
//                                TRANSMIT_Write(0);
//                                CyDelayUs(delay);                                                       
//                            } else {
//                                TRANSMIT_Write(0);
//                                CyDelayUs(2*delay);
//                            }
//                        }
//                        ++index;
//                    } else {
//                        endOfTransmission = 1;
//                    }
//                                       
//                break;
//                
//                case BUSY:
//                    BUSY_Write(1);
//                    IDLE_Write(!BUSY_Read());
//                    COLLISION_Write(!BUSY_Read());
//                break;
//                
//                case COLLISION:
//                    COLLISION_Write(1);
//                    IDLE_Write(!COLLISION_Read());
//                    BUSY_Write(!COLLISION_Read());
//                    delay = ((rand() / 128) * 1000);
//                    
//                break;
//            }
        }
}
    
int getInputData(char* input) 
{
    int dataSize = 0; //size of retrieved data to then be transmitted
    int dataPosition = 0; //position looping through the data to transmit
    int endOfData = 0; //boolean to determine if the entire characters has been received
    unsigned char buffer[10]; //buffer used to hold single blob of data 
           
    dataPosition = 0;
    
    while (endOfData == 0) {      
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
        // Check for input data
        if (0u != USBUART_GetConfiguration())
        {
            if (0u != USBUART_DataIsReady())
            {
                // Read received data
                dataSize = USBUART_GetAll(buffer);
                
                if (0u != dataSize) 
                {
                    for(int i=0; i<dataSize; i++)
                    {
                        if(buffer[i]==0x0D)
                        //if((buffer[i]=='\r') || (buffer[i] == '\n'))
                        {
                            endOfData=1;
                            USBUART_PutCRLF();
                        }
                        else
                        {
                            input[dataPosition]=buffer[i];
                            dataPosition++;
                        }
                    }                  
                }
            }
        }
    }
    return dataPosition;
}
