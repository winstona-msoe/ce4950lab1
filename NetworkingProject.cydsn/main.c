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

//States include: 
//Busy - signals that the channel monitor is busy
//       and in either line high or line low 
//Collision - collision detected, line high
//Idle - initialized state looking for rising
//       edge to occur
enum state {IDLE, COLLISION, BUSY} systemState;

//flag to help determine if signal level is low or high
_Bool lowFlag = 0;




#define BROADCAST_ADDRESS 0


#define VERSION_NUMBER 0x01
#define CRC 128 //CRCs are not being used
#define HEADER 0b1110111 //header CRC not being used 
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
#define ADDR1Start 46
#define ADDR2Start 55
#define ADDR3Start 124 //Kamith didn't have one listed so gave him 124
#define ADDRLength 2


unsigned char transmitData[BUFFER_SIZE][16];
unsigned char buffer[BUFFER_SIZE];
unsigned char receiveBuffer[BUFFER_SIZE];
unsigned char receiveData[16]; 
int position = 0;
int receivePosition = 0;
int dataBitsRead = 0;
int idx = 0;
char* dataPtr;
int transmitLock = 0;
int receiveLock = 0;
//int startHeaderReceieved = 0;
//int verNumMatch = 0;
char sourceAddress = 0x00;
char destinationAddress = 0x00;
int  messageLength = 0;
char crcUsage = 0x00;
char headerCRC = 0x00;
int addressZeroReceive = 0;
char currentChar;
int dataSize;
int transmitBitCount = 0;
int receiveBitCount = 0;
int concatCount = 0;
unsigned char transmitAddress = 0; 



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
 * operation: Handles the receive interrupt
 * inputs: none
 * returns: none
 * implemented: 14 Dec 2017
 * edited:
 *********************************************************/ 
CY_ISR(TransmitInterruptHandler)
{
	TimerTX_STATUS; 

	currentChar = buffer[position];

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
        CyDelayUs(495);
        ++transmitBitCount;
        if(transmitBitCount >= 16){
		    ++position;
		    idx = 0;
		    transmitBitCount = 0;
            transmitLock = 0;
            TRANSMIT_Write(0);

        }
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
        transmitBitCount = 0;    
        transmitLock = 0;
        idx = 0;
        int delay = 0; 
        //wait a time betwee 0 and 1 second
         delay = (rand() % 1000) + 1;
         CyDelay(delay);

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

    TimerTX_Start();
    TimerTX_WritePeriod(TRANSMIT_PERIOD);
    TimerTX_WriteCounter(TRANSMIT_COUNTER);
    TransmitISR_StartEx(TransmitInterruptHandler);
    
    TimerRX_WriteCounter(RECEIVE_COUNTER);
    TimerRX_WritePeriod(RECEIVE_PERIOD);
    ReceiveISR_StartEx(ReceiveInterruptHandler);
        
    //configure usb to start
    USBUART_Start(USBUART_device,USBUART_5V_OPERATION);
    
    //starting system in idle state
    systemState = IDLE;

    while(1){
        int printPrompt = 0;
	
		
		if (0 != USBUART_IsConfigurationChanged())
		{
			
			if (0 != USBUART_GetConfiguration())
			{				
				USBUART_CDC_Init();
			}
		}
        
        
        if(printPrompt == 0){
            int inCount = 0;
            transmitAddress = 0;
            char input = 0;
            while(!USBUART_CDCIsReady());
            USBUART_PutString("Enter Address (3 digits): ");
            while(inCount < 3){
                while(USBUART_DataIsReady() == 0) { 
                    checkForNewData();
                    stateDiagram();
                }
                
                input = USBUART_GetChar();
                
                //get the transmitAddress entered in
                if(inCount == 0){
                    transmitAddress  += 100*(input - (0x30));
                    USBUART_PutChar(input);
                }else if(inCount == 1){
                    transmitAddress  += 10*(input - (0x30));
                    USBUART_PutChar(input);
                }else if (inCount == 2){
                    transmitAddress  += input - (0x30);   
                    USBUART_PutChar(input);
                }
                ++inCount;
            } 

            while(!USBUART_CDCIsReady());
            USBUART_PutCRLF();
            while(!USBUART_CDCIsReady());
            USBUART_PutString("Enter message: ");
            inCount = 6; 
            input = 0; 

            
            buffer[0] = 0x00; 
            buffer[1] = VERSION_NUMBER;
            buffer[2] = ADDR1Start;
            buffer[3] = transmitAddress;
            buffer[4] = 0x00; 
            buffer[5] = CRC;
            buffer[6] = HEADER;
            
            //Enter = 0x0D
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
            buffer[4] = inCount;

        dataSize = buffer[4];
        position = 0;
        printPrompt = 0;
        while(!USBUART_CDCIsReady());
        USBUART_PutCRLF();       
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

void checkForNewData(){
 if(USBUART_CDCIsReady() != 0){
    while(dataBitsRead != receivePosition){
        if(dataBitsRead == 0){
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
        }
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
            if(dataBitsRead > 6){
            USBUART_PutChar(receiveBuffer[dataBitsRead]);
            }
            
        }
        ++dataBitsRead;
    }
}

/* [] END OF FILE */