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


#define ADDR1_Start 16
#define ADDR2_Start 25
#define ADDR3_Start 28
#define ADDR_length 2
#define BROADCAST 0
#define PROTOCOL_VERSION 0x01

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
#define SERIAL_BUFFER_SIZE 500
unsigned char SERIAL_BUFFER[SERIAL_BUFFER_SIZE];
unsigned char SERIAL_RX_BUFFER[SERIAL_BUFFER_SIZE];
int SERIAL_POS = 0;
int SERIAL_RX_POS = 0;
int UART_RX_DATA_READ_OUT = 0;
int uartConnected = 0;
unsigned char CONVERTED_DATA[SERIAL_BUFFER_SIZE][16];
int dataConvertedReadOutCount = 0;
char currentChar;
int dataSize;
int TX_Bit_Counter = 0; 
int count = 0; 
unsigned char RX_DATA[16]; 
int RX_Bit_Counter = 0;
char RX_Char;
int RX_Lock = 0;
    
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


CY_ISR(BackoffInterruptHandler){
    Backoff_STATUS;
    Backoff_Stop();
    systemState = IDLE;
}

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
   	Timer_STATUS; //clear timer
    
    if (!(logicLevel) ){
        systemState = IDLE;
    } else {
        systemState = COLLISION;
        TRANSMIT_Write(0);
        TX_Bit_Counter = 0;    
        count = 0;
        int backoff = 0;
	    //reset current byte transmission 
	    //generate sudo-random backoff, normally distributed between backoff/128 seconds, where N is an int between 0 and 128
	    backoff = (PRS_Read()+1);
	    //CyDelay((double)(backoff/128.0)*1000); //use cydelay for now, which takes millis
        Backoff_WritePeriod((backoff/128.0)*1000);
        Backoff_WriteCounter(((backoff/128.0)*1000)-1);
        Backoff_Start();
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
    TimerISR_StartEx(TimerInterruptHandler);
    RisingEdgeISR_StartEx(RisingEdgeInterruptHandler);
    FallingEdgeISR_StartEx(FallingEdgeInterruptHandler);
    ReceiveISR_StartEx(ReceiveInterruptHandler);  
    USBUART_Start(USBUART_device, USBUART_5V_OPERATION); //configuring USBUART to start
    TRANSMIT_Write(0);
    BackoffISR_StartEx(BackoffInterruptHandler);
    PRS_Start();
    systemState = IDLE;

    while(1){
         //check if UART is connected, then set flag
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
                
        //check if the UART has data, then place in buffer
        if(USBUART_DataIsReady() != 0){
            dataSize = USBUART_GetAll(SERIAL_BUFFER);
            while(!USBUART_CDCIsReady());
            USBUART_PutCRLF();
            dataConvertedReadOutCount = 0;            
            SERIAL_POS = 0;
        }
        //write converted data to UART
    /*
        if(UART_CDCIsReady() != 0){
            if(dataConvertedReadOutCount < dataSize && SERIAL_POS == dataSize){
                for(int y = 0; y < 16; y++){
                 while(!UART_CDCIsReady());
                 //UART_PutChar(CONVERTED_DATA[dataConvertedReadOutCount][y]); 
                UART_PutChar(RX_DATA[y]);
                }
                ++dataConvertedReadOutCount;
                while(!UART_CDCIsReady());
                UART_PutCRLF();
            }
        }
    */
        
        if(USBUART_CDCIsReady() != 0){
            
            if(UART_RX_DATA_READ_OUT != SERIAL_RX_POS){
                for(int y = 0; y < SERIAL_RX_POS; y++){
                    int startHeaderReceieved = 0;
                    int verNumMatch = 0;
                    char sourceAddr = 0x00;
                    char destAddr = 0x00;
                    int dataLength = 0;
                    char CRCused = 0x00;
                    char HeaderCRC = 0x00;
                    int addrDataPrinted = 0;
                    int dataPrintedOut = 0;
                                        
                    while(!USBUART_CDCIsReady());
                    //check for start bit
                    if(y == 0){
                        if(SERIAL_RX_BUFFER[y] == 0x00){
                            startHeaderReceieved = 1;
                        }
                    }
                    //check for version number
                    else if(y == 1){
                        if(SERIAL_RX_BUFFER[y] == 0x01){
                            verNumMatch = 1;
                        }
                    }
                    //get source address
                    else if(y == 2){
                        sourceAddr = SERIAL_RX_BUFFER[y];
                    }
                    //get destination address
                    else if(y == 3){
                        destAddr = SERIAL_RX_BUFFER[y];
                    //get datalegnth     
                    }else if(y == 4){
                        dataLength = SERIAL_RX_BUFFER[y];
                    //get CRC related data
                    }else if(y == 5){
                        CRCused = SERIAL_RX_BUFFER[y];
                    } else if(y == 6){
                        HeaderCRC = SERIAL_RX_BUFFER[y];   
                    }
                    
                    //check version numbers and start bit
                    else if((startHeaderReceieved)&&(verNumMatch)){
                        //check that the destination address is this terminal 
                        if(destAddr == BROADCAST || 
                            ((destAddr >= ADDR1_Start) && (destAddr <= ADDR1_Start+ADDR_length)) ||
                            ((destAddr >= ADDR2_Start) && (destAddr <= ADDR2_Start+ADDR_length)) ||
                            ((destAddr >= ADDR3_Start) && (destAddr <= ADDR3_Start+ADDR_length))){
                            
                            if(addrDataPrinted == 0){
                                addrDataPrinted = 1; 
                                while(!USBUART_CDCIsReady());
                                USBUART_PutString("Source Address: ");
                                while(!USBUART_CDCIsReady());
                                USBUART_PutChar(sourceAddr);
                                while(!USBUART_CDCIsReady());
                                USBUART_PutCRLF();
                                while(!USBUART_CDCIsReady());
                                USBUART_PutString("Destination Address: ");
                                while(!USBUART_CDCIsReady());
                                USBUART_PutChar(destAddr);
                                while(!USBUART_CDCIsReady());
                                USBUART_PutCRLF();
                            }
                            //print out data
                            if(dataPrintedOut < dataLength){
                                while(!USBUART_CDCIsReady());
                                USBUART_PutChar(SERIAL_RX_BUFFER[y]);
                                ++dataPrintedOut;
                            }else{
                            //we are out of data here
                            while(!USBUART_CDCIsReady());
                            USBUART_PutCRLF();
                            }
                                
                        }
                    }
                    USBUART_PutChar(SERIAL_RX_BUFFER[y]); //send data to UART
                    ++UART_RX_DATA_READ_OUT;
                    //while(!UART_CDCIsReady());
                    //UART_PutCRLF();
                }
            }
                SERIAL_RX_POS = 0;
                UART_RX_DATA_READ_OUT = 0;
                        
        }
    }
}
        
        
//    while (1) {  
//        
//        int size = getInputData(data);
//        data[size] = '\0'; // Null termination at CR.
//        
//        dataPtr = data;
//        firstHalfBit = true;
//        idx = 7;
//        
//        // int index = 0;
//        endOfTransmission = 0; //boolean sigifying complete transmission of data
//        
//        while (systemState != IDLE);
//        TransmitISR_StartEx(TransmitInterruptHandler);
//        TimerTX_Start();
//        while (!endOfTransmission) {
//            if (collisionDuringTransmit) {
//                TransmitISR_Stop();
//                TimerTX_Stop();
//                TRANSMIT_Write(0);
//                idx = 7;
//                firstHalfBit = true;
//                delay = (rand() % 1000) + 1;
//                CyDelay(delay);
//               // CyDelay(delay);
//                collisionDuringTransmit = false;
//                while (systemState != IDLE);
//                TransmitISR_StartEx(TransmitInterruptHandler);
//                TimerTX_Start();
//            }
//        }
//        endOfTransmission = false;
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
//        }
//}
    
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
