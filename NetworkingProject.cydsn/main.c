/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

#define MAX_DATA_SIZE 44

typedef struct {
    uint8_t start; // Header start (must be 0x80)
    uint8_t version; // Version number (0x81 for 1st version)
    uint8_t source; // Source address
    uint8_t destination; // Destination address
    uint8_t length; // Length of message (must be between 0 and 44. Bit 7 is 1)
    
    // Indicates which CRC(s) are used.
    // See Interface Standard for more details.
    uint8_t crcUsage;
    
    uint8_t headerCrc; // Header CRC. Should be 0xF7 when unused
} Header;


typedef enum state {idleState, collisionState, busyState} State;

State systemState = idleState;

_Bool lowFlag = 0;

//Address constants
// TODO From Andy: Why are these addresses from Anthony,
// Shah, and Divin?
#define ADDR1_Start 16
#define ADDR2_Start 25
#define ADDR3_Start 28
#define ADDR_length 2
#define BROADCAST 0
#define PROTOCOL_VERSION 0x01

//CRC constants
#define CRC_USE 128 //crcs not used
#define HEADER_CRC 0b1110111 

//period and counter for timer. Used for collision/idle detection.
#define COLL_PERIOD     52
#define COLL_COUNTER    51
#define IDLE_PERIOD     830
#define IDLE_COUNTER    829

//Receive timer stuff. Ensures proper receiving speed.
#define RX_PERIOD   52
#define RX_COUNTER  51

//transmit timer stuff
#define TX_PERIOD     47 //gives 0.50ms or 500us for unipolar-rz at 1000bps
#define TX_COUNTER    46 //empirically tuned...
#define RANDON_BACKOFF_MAX_RETRY 10

//serial buffer size
#define SERIAL_BUFFER_SIZE 500

//serial buffer
unsigned char serialBuffer[SERIAL_BUFFER_SIZE];
unsigned char serialRxBuffer[SERIAL_BUFFER_SIZE];
int serialPos = 0;
int serialRxPos = 0;
int USBUART_rxDataReadOut = 0;

//flag to determine if the UART is connected
int uartConnected = 0;

//data cache for later use
unsigned char CONVERTED_DATA[SERIAL_BUFFER_SIZE][16];
int dataConvertedReadOutCount = 0;
char currentChar;
int dataSize;
int TX_Bit_Counter = 0; //keeps track of what would be a for loop in the ISR
int count = 0; //keeps track of which bit we're on
unsigned char rxData[16]; //receieved data buffer
int RX_Bit_Counter = 0;
char RX_Char;
int RX_Lock = 0;
int TX_Lock = 0; //allows us to transmit when in busy if we are transmitting
int bitConCatCount = 0;
unsigned char TX_Addr = 0; //transmit dest address
unsigned char TX_length = 0; //transmit length

int startHeaderReceieved = 0;
int verNumMatch = 0;
char sourceAddr = 0x00;
char destAddr = 0x00;
int dataLength = 0;
char CRCused = 0x00;
char HeaderCRC = 0x00;
int addrDataPrinted = 0;
int dataPrintedOut = 0;

void checkNewBytes(); //gets new messages
void checkState(); //gets current system state
void getHeader(); //gets the header

// TODO Code needs to be added to accept signals at
// various speeds. The documentation states that the signal
// MUST be tolerated at speeds of ±1.32%, and MUST NOT be
// tolerated at speeds of ±8% or so. See standard specification
// for more details.

// TODO Move characterRX logic into RX_Bit_Counter
// incrementation section of code. This will make
// this ISR more efficient.
CY_ISR(ReceiveInterruptHandler){
    int bitConCatCount = 0;
    char characterRX = 0;
    
    TimerRX_STATUS; //clear stat
    //only receive data in not collision
    if(systemState != collisionState){
        TimerRX_Start();
        rxData[RX_Bit_Counter] = RECEIVE_Read(); //read bit
        ++RX_Bit_Counter;
        
    }else{
        //reset buffers when in collision 
        RX_Bit_Counter = 0;   
        serialRxPos = 0;
        USBUART_rxDataReadOut = 0;
        TimerRX_Stop();
        RX_Lock = 0;
    }
    //reset bit counter if we intook all the bits
    if(RX_Bit_Counter >= 15 ){
        RX_Bit_Counter = 0;
        TimerRX_Stop();
        RX_Lock = 0;
        
        
        for(int x = 15; x >= 0; x--){
            if(x == 15){
             characterRX = 0x00; //account for start bit   
            }
            if(x % 2 != 0){
                characterRX = (characterRX | (rxData[x] << (bitConCatCount-1)));
                //UART_PutChar((rxData[x])+0x30);
                ++bitConCatCount;
            }
        }
        serialRxBuffer[serialRxPos] = characterRX;
        ++serialRxPos;
       
    }
}

CY_ISR(TransmitInterruptHandler)
{
	TimerTX_STATUS; //clear TX timer
	//get data
	currentChar = serialBuffer[serialPos];

	if(((serialPos < dataSize) && (systemState == idleState)) || ((TX_Lock) && (systemState == busyState))){ 
	//encode into Unipolar-RZ
	
	//Data to be transmitted. This represents one byte of data and will
	//be encoded in unipolar RZ. For every bit there will be 2 bits, and there are 8 bits
	
		if(TX_Bit_Counter == 0){
			TRANSMIT_Write(1);
			CONVERTED_DATA[serialPos][TX_Bit_Counter] = 0x31;
		}else if(TX_Bit_Counter%2 != 0){
			TRANSMIT_Write(0);
			CONVERTED_DATA[serialPos][TX_Bit_Counter] = 0x30;
		}else{
			//see if a 1 exists at the bit of the char, otherwise write a 0 out
			//this should transmit MSB first
			if(currentChar & (1<<(6-count))){
				TRANSMIT_Write(1);
				CONVERTED_DATA[serialPos][TX_Bit_Counter] = 0x31;
			}else{
				TRANSMIT_Write(0);
				CONVERTED_DATA[serialPos][TX_Bit_Counter] = 0x30;   
			}
				++count;
		}
        CyDelayUs(495);
        ++TX_Bit_Counter;
        if(TX_Bit_Counter >= 16){
		    ++serialPos;
		    count = 0;
		    TX_Bit_Counter = 0;
            TX_Lock = 0;
            TRANSMIT_Write(0);
        }
 
    }else if(systemState == collisionState){
      
    }
	
}

CY_ISR(BackoffInterruptHandler){
    Backoff_STATUS;
    Backoff_Stop();
    TX_Lock = 1;
    systemState = idleState;
}

CY_ISR(TimerInterruptHandler)
{
   	Timer_STATUS; //clear timer
    
    if (!(lowFlag) ){
        systemState = idleState;
    } else {
        systemState = collisionState;
        TRANSMIT_Write(0);
        TX_Bit_Counter = 0;    
        TX_Lock = 0;
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

// TODO Fix "race against time" between ReceiveInterruptHandler ant his.
// If this wins the race, then RX_Bit_Counter doesn't increment properly.
// This is the reason why code didn't work well at higher speeds.
CY_ISR(RisingEdgeInterruptHandler)
{
    if ((!lowFlag)){
        Timer_WritePeriod(COLL_PERIOD);
        Timer_WriteCounter(COLL_COUNTER);
        Timer_Start();
        lowFlag = 1;
        systemState = busyState;
    }
    if(RX_Lock == 0){
        TimerRX_Start();
        RX_Lock = 1;       
    } else {
        RX_Bit_Counter++;
        // TODO Code doesn't quite take into account corner case
        // when RX_Bit_Counter reached 15. Now we gotta set the byte.
    }
}

// TODO This could also contribute to the "race against time" issue.
CY_ISR(FallingEdgeInterruptHandler)
{
    if (lowFlag){
        Timer_WritePeriod(IDLE_PERIOD);
        Timer_WriteCounter(IDLE_COUNTER);
        Timer_Start();
        lowFlag = 0;
        systemState = busyState;
    }
}

int main(void)
{

    CyGlobalIntEnable; //enable global interrupts
    //starting the interrupt handlers
    TimerISR_StartEx(TimerInterruptHandler);
    RisingEdgeISR_StartEx(RisingEdgeInterruptHandler);
    FallingEdgeISR_StartEx(FallingEdgeInterruptHandler);
    ReceiveISR_StartEx(ReceiveInterruptHandler);  

    TimerTX_Start();
    TimerTX_WritePeriod(TX_PERIOD);
    TimerTX_WriteCounter(TX_COUNTER);
    TransmitISR_StartEx(TransmitInterruptHandler);
    
    TimerRX_WriteCounter(RX_COUNTER);
    TimerRX_WritePeriod(RX_PERIOD);
    ReceiveISR_StartEx(ReceiveInterruptHandler);
    
    BackoffISR_StartEx(BackoffInterruptHandler);
    PRS_Start();
    USBUART_Start(USBUART_device,USBUART_5V_OPERATION);


    while(1){
        int printPrompt = 0;
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
        
        //get address
        if(printPrompt == 0 && uartConnected){
            int inCount = 0;
            TX_Addr = 0;
            char input = 0;
            while(!USBUART_CDCIsReady());
            USBUART_PutString("Enter Address (3 digits): ");
            while(inCount < 3){
                while(USBUART_DataIsReady() == 0){ //wait for digits, perform other tasks here
                    checkNewBytes();
                    checkState();
                }
                //why minus 0x30? because these are ASCII chars from the keyboard...
                input = USBUART_GetChar();
                if(inCount == 0){
                    TX_Addr  += 100*(input - (0x30));
                    USBUART_PutChar(input);
                }else if(inCount == 1){
                    TX_Addr  += 10*(input - (0x30));
                    USBUART_PutChar(input);
                }else if (inCount == 2){
                    TX_Addr  += input - (0x30);   
                    USBUART_PutChar(input);
                }
                ++inCount;
            } 
            //Display print newline and prompt for message
            while(!USBUART_CDCIsReady());
            USBUART_PutCRLF();
            while(!USBUART_CDCIsReady());
            USBUART_PutString("Enter message: ");
            inCount = 6; //reset counter, accounting for header
            input = 0; //reset input
            //encode header
            
            serialBuffer[0] = 0x00; //does not take into account starting bit
            serialBuffer[1] = PROTOCOL_VERSION;
            serialBuffer[2] = ADDR1_Start;
            serialBuffer[3] = TX_Addr;
            serialBuffer[4] = 0x00; //padding for now, will be replaced with actual length after user input
            serialBuffer[5] = CRC_USE;
            serialBuffer[6] = HEADER_CRC;
            
            //OD is the 'enter' key
            while(input != 0x0D){
                while(USBUART_DataIsReady() == 0); //wait for message data
                input = USBUART_GetChar();
                if(input != 0x0D){
                    serialBuffer[inCount] = input;
                    while(!USBUART_CDCIsReady());
                    USBUART_PutChar(serialBuffer[inCount]);
                    ++inCount;
                }
            }
            serialBuffer[4] = inCount; //replace padding with actual message length
        //send data by setting variable
        dataSize = serialBuffer[4];
        serialPos = 0;
        printPrompt = 0;
        while(!USBUART_CDCIsReady());
        USBUART_PutCRLF();       
        }
        

        
    }
}

void checkState(){
 switch(systemState){
        //idle state
        case idleState :;
            IDLE_Write(1);
            BUSY_Write(!IDLE_Read());
            COLLISION_Write(!IDLE_Read());
            //since nothing happens in IDLE this is a safe bet...
            USBUART_rxDataReadOut = 0;
            serialRxPos = 0;
            addrDataPrinted = 0;
        break;
        //busy state
        case busyState:
            BUSY_Write(1);
            IDLE_Write(!BUSY_Read());
            COLLISION_Write(!BUSY_Read());
        break;
        //collision state
        case collisionState:;
            COLLISION_Write(1);
            IDLE_Write(!COLLISION_Read());
            BUSY_Write(!COLLISION_Read());
        break;
    }   
}

void checkNewBytes(){
 if(USBUART_CDCIsReady() != 0){
    while(USBUART_rxDataReadOut != serialRxPos){
        if(USBUART_rxDataReadOut == 0){
            getHeader();
        }
        if(destAddr == BROADCAST || 
        ((destAddr >= ADDR1_Start) && (destAddr <= ADDR1_Start+ADDR_length)) ||
        ((destAddr >= ADDR2_Start) && (destAddr <= ADDR2_Start+ADDR_length)) ||
        ((destAddr >= ADDR3_Start) && (destAddr <= ADDR3_Start+ADDR_length))){
            if(addrDataPrinted == 0){
            while(!USBUART_CDCIsReady());
            USBUART_PutCRLF();
            while(!USBUART_CDCIsReady());
            USBUART_PutCRLF();
            while(!USBUART_CDCIsReady());
            USBUART_PutCRLF(); 
            while(!USBUART_CDCIsReady());
            USBUART_PutString("Message From: ");
            while(!USBUART_CDCIsReady());
            //convert numbers to ascii
            //hundreds
            USBUART_PutChar(((sourceAddr)/100)+0x30);
            //tens
            while(!USBUART_CDCIsReady());
            USBUART_PutChar(((sourceAddr/10)%10)+0x30);
            while(!USBUART_CDCIsReady());
            //ones
            USBUART_PutChar(((sourceAddr%10)%10)+0x30);
            while(!USBUART_CDCIsReady());
            USBUART_PutCRLF();
            while(!USBUART_CDCIsReady());
            USBUART_PutString("Sent To: ");
            if(destAddr == BROADCAST){
                while(!USBUART_CDCIsReady());
                USBUART_PutString("BCST ");
                while(!USBUART_CDCIsReady());
                USBUART_PutCRLF();
            }else{
                while(!USBUART_CDCIsReady());
                //hundreds
                USBUART_PutChar(((destAddr)/100)+0x30);
                //tens
                while(!USBUART_CDCIsReady());
                USBUART_PutChar(((destAddr/10)%10)+0x30);
                while(!USBUART_CDCIsReady());
                //ones
                USBUART_PutChar(((destAddr%10)%10)+0x30);
                USBUART_PutCRLF();
                while(!USBUART_CDCIsReady());
            }
            while(!USBUART_CDCIsReady());
            USBUART_PutString("Size: ");
            while(!USBUART_CDCIsReady());
            //hundreds
            USBUART_PutChar(((dataLength)/100)+0x30);
            //tens
            while(!USBUART_CDCIsReady());
            USBUART_PutChar(((dataLength/10)%10)+0x30);
            while(!USBUART_CDCIsReady());
            //ones
            USBUART_PutChar(((dataLength%10)%10)+0x30);
            while(!USBUART_CDCIsReady());
            USBUART_PutCRLF();
            while(!USBUART_CDCIsReady());
            USBUART_PutString("Message: ");
            while(!USBUART_CDCIsReady());
            addrDataPrinted = 1;
            }
            if(USBUART_rxDataReadOut > 6){
            USBUART_PutChar(serialRxBuffer[USBUART_rxDataReadOut]);
            }
            
        }
        ++USBUART_rxDataReadOut;
    }
     
   }
}

void getHeader(){
    //if there is at least enough data in the buffer to hold the header, fetch it
    if(serialRxBuffer[0] == 0x00){
        startHeaderReceieved = 1;
    
    }
    //check for version number
    if(serialRxBuffer[1] == 0x01){
            verNumMatch = 1;
   }
   //get source address
    sourceAddr = serialRxBuffer[2];
    //get destination address
    destAddr = serialRxBuffer[3];
  
    //get datalegnth     
    dataLength = serialRxBuffer[4];

    //get CRC related data
    CRCused = serialRxBuffer[5];

    HeaderCRC = serialRxBuffer[6];
 
}
/* [] END OF FILE */