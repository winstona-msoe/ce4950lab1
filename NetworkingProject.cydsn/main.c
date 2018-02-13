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
 * - systemState machine with  channel monitor
 *   implementation (BUSY/IDLE/COLLISION)
 * - Software pushbutton debouncing used 
 * - LED to signal different states
 * - Interrupt handling for rising and 
  *  edge triggers of a timer
 * ========================================
*/



#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define BROADCAST_ADDRESS 0
#define VERSION_NUMBER 0x01
#define CRC 128 //CRCs are not being used
#define HEADER 0b1110111 //header CRC not being used 
#define BLANK_MESSAGE_CRC 0x77
#define COLLISION_PERIOD     52
#define COLLISION_COUNTER    51
#define IDLE_PERIOD     830
#define IDLE_COUNTER    829
#define RECEIVE_PERIOD   52
#define RECEIVE_COUNTER  51
#define TRANSMIT_PERIOD     47
#define TRANSMIT_COUNTER    46 
#define BUFFER_SIZE 500
//student assigned addressses
#define ADDR1Start 0x60 //46
#define ADDR2Start 55
#define ADDR3Start 124 //Kamith didn't have one listed so gave him 124
#define ADDRLength 2

#define MESSAGE_POS 7
#define HEADER_SIZE


//States include: 
//Busy - signals that the channel monitor is busy
//       and in either line high or line low 
//Collision - collision detected, line high
//Idle - initialized state looking for rising
//       edge to occur
volatile enum state {IDLE, COLLISION, BUSY} systemState;

//flag to help determine if signal level is low or high
volatile _Bool lowFlag = 0;

volatile unsigned char transmitData[BUFFER_SIZE][16];
volatile unsigned char buffer[BUFFER_SIZE];
volatile unsigned char receiveBuffer[BUFFER_SIZE];
volatile unsigned char receiveData[16]; 
volatile int position = 0;
volatile int receivePosition = 0;
volatile int dataBitsRead = 0;
volatile int idx = 0;
volatile char* dataPtr;
volatile int transmitLock = 0; // High when transmitting
volatile int receiveLock = 0;
volatile bool collisionLock = false; // High when collision should not.

volatile char sourceAddress = 0x00;
volatile char destinationAddress = 0x00;
volatile int  messageLength = 0;
volatile char crcUsage = 0x00;
volatile char headerCRC = 0x00;
volatile int addressZeroReceive = 0;
volatile char currentChar;
volatile int dataSize;
volatile int transmitBitCount = 0;
volatile int receiveBitCount = 0;
volatile int concatCount = 0;
volatile unsigned char transmitAddress = 0; 

bool transmitMode = true; // Whether currently transmitting or receiving

void checkForNewData(); 
void stateDiagram(); 

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

// TODO Code needs to be added to accept signals at
// various speeds. The documentation states that the signal
// MUST be tolerated at speeds of ±1.32%, and MUST NOT be
// tolerated at speeds of ±8% or so. See standard specification
// for more details.

// TODO Move characterRX logic into RX_Bit_Counter
// incrementation section of code. This will make
// this ISR more efficient.
CY_ISR(ReceiveInterruptHandler){
    int concatCount = 0;
    char characterRX = 0;
    
    TimerRX_STATUS; 
    
    if(systemState != COLLISION){
        TimerRX_Start();
        receiveData[receiveBitCount] = RECEIVE_Read(); 
        ++receiveBitCount;
        
    }else{
        
        receiveBitCount = 0;   
        receivePosition = 0;
        dataBitsRead = 0;
        TimerRX_Stop();
        receiveLock = 0;
    }
    
    if(receiveBitCount >= 15 ){
        receiveBitCount = 0;
        TimerRX_Stop();
        receiveLock = 0;
        
        
        for(int x = 15; x >= 0; x--){
            if(x == 15){
             characterRX = 0x00;    
            }
            if(x % 2 != 0){
                characterRX = (characterRX | (receiveData[x] << (concatCount-1)));              
                ++concatCount;
            }
        }
        receiveBuffer[receivePosition] = characterRX;
        ++receivePosition;
       
    }
}

 /**********************************************************
 * function name: ReceiveInterruptHandler
 * operation: Handles the transmit interrupt.
Fires each time we need to transmit.
 * inputs: none
 * returns: none
 * implemented: 14 Dec 2017
 * edited:
 *********************************************************/ 
CY_ISR(TransmitInterruptHandler)
{
	TimerTX_STATUS; 
    if (position >= dataSize) {
        return;
    }
	currentChar = buffer[position];

    if (!collisionLock) {
    	if(((position < dataSize) && (systemState == IDLE)) || ((transmitLock) && (systemState == BUSY))){ 
    		if(transmitBitCount == 0){
    			TRANSMIT_Write(1);
    			transmitData[position][transmitBitCount] = 0x31;
    		}else if(transmitBitCount%2 != 0){
    			TRANSMIT_Write(0);
    			transmitData[position][transmitBitCount] = 0x30;
    		}else{
    			if(currentChar & (1<<(6-idx))){
    				TRANSMIT_Write(1);
    				transmitData[position][transmitBitCount] = 0x31;
    			}else{
    				TRANSMIT_Write(0);
    				transmitData[position][transmitBitCount] = 0x30;   
    			}
    				++idx;
    		}
        }
        // CyDelayUs(495);
        ++transmitBitCount;
        if(transmitBitCount >= 16){
		    ++position;
		    idx = 0;
		    transmitBitCount = 0;
            transmitLock = 0;
            TRANSMIT_Write(0);

        }
    } else {
        position = 0;
        transmitBitCount = 0;
    }
}


 /**********************************************************
 * function name: TimerInterruptHandler
 * operation: When a node that is transmitting and detects 
 *            a collision, the node shall stop transmitting
 *            and wait a random amount of time between 0
 *            and 1.000 second (s), and then determine if
 *            the bus is idle and if so begin a retrans.
 * inputs: none
 * returns: none
 * implemented: 14 Dec 2017
 * edited:
 *********************************************************/

//TODO: WORK ON THE TRYING REATTEMPT UP TO 10 TIMES
CY_ISR(TimerInterruptHandler)
{     
   	Timer_STATUS; 
    
    if (!(lowFlag) ){
        systemState = IDLE;
         transmitLock = 1;
    } else {
        systemState = COLLISION;
        TRANSMIT_Write(0); 
        transmitLock = 0; // Don't transmit anymore.
        collisionLock = 1; // Collision
        position = 0; // Start over entire packet.
        receivePosition = 0;
        
        uint16_t randNum = (uint16_t)((float)rand() / RAND_MAX * 1000);
        CollisionDelay_WritePeriod(randNum);
        CollisionDelay_WriteCounter(randNum - 1);
        CollisionDelay_Start();
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

// TODO Fix "race against time" between ReceiveInterruptHandler ant his.
// If this wins the race, then RX_Bit_Counter doesn't increment properly.
// This is the reason why code didn't work well at higher speeds.
CY_ISR(RisingEdgeInterruptHandler)
{
    if ((!lowFlag)){
        Timer_WritePeriod(COLLISION_PERIOD);
        Timer_WriteCounter(COLLISION_COUNTER);
        Timer_Start();
        lowFlag = 1;
        systemState = BUSY;
    }
    if(receiveLock == 0){
        TimerRX_Start();
        receiveLock = 1;
        
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
// TODO This could also contribute to the "race against time" issue.
CY_ISR(FallingEdgeInterruptHandler)
{
    if (lowFlag){
        Timer_WritePeriod(IDLE_PERIOD);
        Timer_WriteCounter(IDLE_COUNTER);
        Timer_Start();
        lowFlag = 0;
        systemState = BUSY;
    }
}

/**
This interrupt should fire after the random delay period of the timer after
a collision has expired. All this does is tells the transmitter that it has
the green light to transmit.
*/
CY_ISR(CollisionDelayInterruptHandler)
{
    collisionLock = false;
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

    //enable global interrupts
    //start the interrupt handlers for the timers and other components
    CyGlobalIntEnable; 
    TimerISR_StartEx(TimerInterruptHandler);
    RisingEdgeISR_StartEx(RisingEdgeInterruptHandler);
    FallingEdgeISR_StartEx(FallingEdgeInterruptHandler);
    ReceiveISR_StartEx(ReceiveInterruptHandler);  

    // TimerTX_Start();
    TimerTX_WritePeriod(TRANSMIT_PERIOD);
    TimerTX_WriteCounter(TRANSMIT_COUNTER);
    TransmitISR_StartEx(TransmitInterruptHandler);
    
    TimerRX_WriteCounter(RECEIVE_COUNTER);
    TimerRX_WritePeriod(RECEIVE_PERIOD);
    ReceiveISR_StartEx(ReceiveInterruptHandler);
    
    CollisionDelayISR_StartEx(CollisionDelayInterruptHandler);
        
    //configure usb to start
    USBUART_Start(USBUART_device,USBUART_5V_OPERATION);
    
    while (!USBUART_GetConfiguration());
    USBUART_CDC_Init();
    
    //starting system in idle state
    systemState = IDLE;

    while(1) {
                
		/** if (0 != USBUART_IsConfigurationChanged())
		{
			
			if (0 != USBUART_GetConfiguration())
			{				
				USBUART_CDC_Init();
			}
		} */
        
        
        if(transmitMode){
            int inCount = 0;
            transmitAddress = 0;
            char input = 0;
            while(!USBUART_CDCIsReady());
            USBUART_PutString("Enter Address or press 'R' to switch to receive mode: ");
            while(inCount < 3){
                while(USBUART_DataIsReady() == 0) { 
                    stateDiagram();
                }
                
                input = USBUART_GetChar();
                
                //get the transmitAddress entered in
                if(inCount == 0){
                    if (toupper(input) == 'R') {
                        transmitMode = false;
                        TimerRX_Start();
                        USBUART_PutChar(input);
                        
                        USBUART_PutCRLF();
                        USBUART_PutString("Press 'T' to switch to transmit mode.\r\n");
                        USBUART_PutCRLF();
                        inCount = 0;
                        break;
                    } else {
                        transmitAddress  += 100*(input - (0x30));
                        USBUART_PutChar(input);
                        ++inCount;
                    }
                }else if(inCount == 1){
                    transmitAddress  += 10*(input - (0x30));
                    USBUART_PutChar(input);
                    ++inCount;
                }else if (inCount == 2){
                    transmitAddress  += input - (0x30);   
                    USBUART_PutChar(input);
                    ++inCount;
                }
                
            }
            if (!transmitMode) {
                continue;
            }

            while(!USBUART_CDCIsReady());
            USBUART_PutCRLF();
            while(!USBUART_CDCIsReady());
            USBUART_PutString("Enter message: ");
            inCount = 7; 
            input = 0; 

            
            buffer[0] = 0x00; // Start of header
            buffer[1] = VERSION_NUMBER; // Verison number
            buffer[2] = ADDR1Start; // Source address
            buffer[3] = transmitAddress; // Destination address
            buffer[4] = 0x00; // CRC disabled
            buffer[5] = CRC; // CRC (is 0x77 when disabled)
            buffer[6] = HEADER; // Header CRC (0x77 when disabled)
            
            //Enter = 0x0D (carriage return or \r)
            while(input != 0x0D){
                while(USBUART_DataIsReady() == 0); 
                input = USBUART_GetChar();
                if(input != 0x0D){
                    buffer[inCount] = input;
                    while(!USBUART_CDCIsReady());
                    USBUART_PutChar(buffer[inCount]);
                    ++inCount;
                }
            }
            buffer[4] = inCount - 7; // Removes CR character.
            if (buffer[4] > 44) {
                while(!USBUART_CDCIsReady());
                USBUART_PutString("\r\nMessage must be no more than 44 bytes.\r\n");
                continue;
            }

            dataSize = inCount;
            position = 0; // Reset position in transmit interrupt
            buffer[inCount] = BLANK_MESSAGE_CRC;
            dataSize++;
            TimerTX_Start();
            while (position < dataSize);
            TimerTX_Stop();
            while(!USBUART_CDCIsReady());
            USBUART_PutCRLF(); 
            
        } else { // Receive mode
            checkForNewData();
            stateDiagram();
            if (USBUART_DataIsReady()) { 
                char input;
                input = USBUART_GetChar();
                if (toupper(input) == 'T') {
                    transmitMode = true;
                }
            }
        }
    }
}

void stateDiagram(){
    switch(systemState){      
        case IDLE :;
            IDLE_Write(1);
            BUSY_Write(!IDLE_Read());
            COLLISION_Write(!IDLE_Read());
            
            dataBitsRead = 0;
            receivePosition = 0;
            addressZeroReceive = 0;
        break;

        case BUSY:
            BUSY_Write(1);
            IDLE_Write(!BUSY_Read());
            COLLISION_Write(!BUSY_Read());
        break;

        case COLLISION:;
            COLLISION_Write(1);
            IDLE_Write(!COLLISION_Read());
            BUSY_Write(!COLLISION_Read());
        break;
    }   
}

/**
*/
void checkForNewData(){
    if(USBUART_CDCIsReady() != 0){
        while(dataBitsRead != receivePosition){
            if(dataBitsRead <= 6){
    //            if(receiveBuffer[0] == 0x00){
    //                startHeaderReceieved = 1;
    //            }
    //            if(receiveBuffer[1] == 0x01){
    //                    verNumMatch = 1;
    //           }
                sourceAddress = receiveBuffer[2];
                destinationAddress = receiveBuffer[3];
                messageLength = receiveBuffer[4];
                crcUsage = receiveBuffer[5];
                headerCRC = receiveBuffer[6];            
            } else {
                if(destinationAddress == BROADCAST_ADDRESS || 
                ((destinationAddress >= ADDR1Start) && (destinationAddress <= ADDR1Start+ADDRLength)) ||
                ((destinationAddress >= ADDR2Start) && (destinationAddress <= ADDR2Start+ADDRLength)) ||
                ((destinationAddress >= ADDR3Start) && (destinationAddress <= ADDR3Start+ADDRLength))){
                    if(addressZeroReceive == 0){
                        while(!USBUART_CDCIsReady());
                        USBUART_PutCRLF();
                        while(!USBUART_CDCIsReady());
                        USBUART_PutCRLF();
                        while(!USBUART_CDCIsReady());
                        USBUART_PutCRLF(); 
                        while(!USBUART_CDCIsReady());
                        USBUART_PutString("Message From: ");
                        while(!USBUART_CDCIsReady());

                        USBUART_PutChar(((sourceAddress)/100)+0x30);

                        while(!USBUART_CDCIsReady());
                        USBUART_PutChar(((sourceAddress/10)%10)+0x30);
                        while(!USBUART_CDCIsReady());
             
                        USBUART_PutChar(((sourceAddress%10)%10)+0x30);
                        while(!USBUART_CDCIsReady());
                        USBUART_PutCRLF();
                        while(!USBUART_CDCIsReady());
                        USBUART_PutString("Sent To: ");
                        if(destinationAddress == BROADCAST_ADDRESS){
                            while(!USBUART_CDCIsReady());
                            USBUART_PutString("BCST ");
                            while(!USBUART_CDCIsReady());
                            USBUART_PutCRLF();
                        }else{
                            while(!USBUART_CDCIsReady());
             
                            USBUART_PutChar(((destinationAddress)/100)+0x30);

                            while(!USBUART_CDCIsReady());
                            USBUART_PutChar(((destinationAddress/10)%10)+0x30);
                            while(!USBUART_CDCIsReady());
               
                            USBUART_PutChar(((destinationAddress%10)%10)+0x30);
                            USBUART_PutCRLF();
                            while(!USBUART_CDCIsReady());
                        }
                        while(!USBUART_CDCIsReady());
                        USBUART_PutString("Size: ");
                        while(!USBUART_CDCIsReady());
                        USBUART_PutChar(((messageLength)/100)+0x30);
                        while(!USBUART_CDCIsReady());
                        USBUART_PutChar(((messageLength/10)%10)+0x30);
                        while(!USBUART_CDCIsReady());
                        USBUART_PutChar(((messageLength%10)%10)+0x30);
                        while(!USBUART_CDCIsReady());
                        USBUART_PutCRLF();
                        while(!USBUART_CDCIsReady());
                        USBUART_PutString("Message: ");
                        while(!USBUART_CDCIsReady());
                        addressZeroReceive = 1;
                    }
                    if(dataBitsRead > 6 && dataBitsRead < receiveBuffer[4] + 7){
                        USBUART_PutChar(receiveBuffer[dataBitsRead]);
                    }
                    
                }
            }
            ++dataBitsRead;
        }
    }
}
/* [] END OF FILE */