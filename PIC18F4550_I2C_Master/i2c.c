  	#include "pic18f4550_i2c_header.h"   
  	
  	unsigned char gRecvData=0,gExpectAck =0,gSendingReadCtrlBits=0,gSendingWriteCtrlBits=0,gSendingData=0;
  	char gBuffer[32];
  	
  	void interrupt ISR()
    {
        if(SSPIE && SSPIF)
        {
            if(BCLIF == 1){
                sendToUart("Bus collision Detected");
                BCLIF=0;
            }
            if(WCOL == 1){
                sendToUart("Write collision Detected. Write failed");
                WCOL=0;
            }
            if(gRecvData && BF){
                //No need to clear BF, manually.
                gRecvData= 0;
                sendToUart("Data Recvd");
                recvRomData();
            }
            if(SSPSTATbits.S == 0b1){
                sendToUart("Start Detected");
            }
            if(SSPSTATbits.P == 0b1){
                sendToUart("Stop Detected");
            }
            
            if(SSPSTATbits.BF == 0b1){
                sendToUart("SSPBUF is Full");
            }
            
            /*
            * here we are assuming that , there are no intr happening between the SSPBUF become empty with 
            * intr notification and ACK/NACK recv with intr notification.
            */
            if(gExpectAck == 1){
               gExpectAck = 0;
                if(SSPCON2bits.ACKSTAT == 1) {
                    sendToUart("No ACK");
                }
                else{
                    sendToUart("ACK Recvd");
                    if(gSendingWriteCtrlBits == 1){
                         gSendingWriteCtrlBits =0;
                         sendToUart("ctrl bit write OK");
                         sendRomAddr();
                    }
                    if(gSendingData == 1){
                        sendToUart("ROM Addr write OK");
                        gSendingData=0;
                        SSPCON2bits.PEN =0b1; //iniating  Stop condition
                        SSPCON2bits.RCEN=0b1;//Receive Enable
                        gRecvData =1;
                    }
                     
               }
            }
            if(SSPSTATbits.BF == 0b0){
                if(gSendingWriteCtrlBits == 1){
                    gExpectAck =1;
                }
                 if(gSendingData == 1){
                    gExpectAck =1;
                }             
                sendToUart("SSPBUF is Empty");
            }
            
        }
        SSPIF = 0b0; //Clear the Interrupt
    }
    int recvRomData(){
        char recvCh=0;
        int recvVal=0;
        
        recvVal = SSPSR;
        memset(gBuffer,0,sizeof(gBuffer));
        sprintf(gBuffer,"%d",recvVal);
        sscanf(gBuffer,"%c",&recvCh);
        memset(gBuffer,0,sizeof(gBuffer));
        sprintf(gBuffer,"dec=%d hex=%x ch=%c",recvCh);
        sendToUart(gBuffer);
        
        invokeStart();
        return 0;
    }
    int sendRomAddr()
    {
        static char addr=0x41;
        gSendingData =1;
        
        //loading data
        SSPBUF = addr;
        
        if(addr <= 0x5A)
            addr = 0x41;
        else
            addr = addr + 0x01;
            
        return 0;
    }
  	int invokeStart()
  	{
  	    //ToDo: Check for Bus Idle state
  	    SSPCON2bits.SEN=0b1; //Start Condition
  	    gSendingWriteCtrlBits=1; 
  	    SSPBUF = (SLAVE_ADDR << 1) | MASTER_WRITE;
  	    
  	    return 0;
  	}
  	int start_I2C()
  	{
  	    invokeStart();
  	    return 0;
  	}
  	int init_I2c()
    {
        SSPCON1bits.SSPEN = 0b1;
    	
    	//setting bits 3:0 as 1110 to En I2C with start/stop interrupts
    	SSPCON1bits.SSPM3 = 0b1;
    	SSPCON1bits.SSPM2 = 0b1;
    	SSPCON1bits.SSPM1 = 0b1;
    	SSPCON1bits.SSPM0 = 0b0;
    	
    	SSPIE=1;
        return 0;
    }
    