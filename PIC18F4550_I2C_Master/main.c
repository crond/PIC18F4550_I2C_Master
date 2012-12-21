#include "pic18f4550_i2c_header.h"   

    int toggleLed(void)
    {
    	PORTB = 0x00;
    	while(1)
    	{
    		PORTB = 0xAA;
    		delay(200);
    		PORTB	= 0x00;
    		delay(200);
    	}
    	return 0;	
    }	
    	
    
    int delay(int cnt)
    {
    	int i=0,j=0;
    	for(i =0;i<3;i++)
    	{
    		for(j=0;j<cnt;j++)
    		{
    		}	
    	}
    	return 0;	
    }
    int init(void)
    {
    	//8Mhx interal osc
    	OSCCON	= 0x73;
    	
    	//For LED
    	PORTB	= 0x00;
    	TRISB	= 0x00;
    	
    	//PIN ctrl for I2C
    	TRISD	=0xFF;
    	PORTD	=0xFF;
        
        //Enable general,peripheral,ssp interrupts
    	GIE=1;
    	PEIE=1;
    	
    	init_I2C();
        
        /*
        * ToDo:
        * 1. BRG calculation
        * oper voltage calcualton
        * I2C commu freq value
        */
        return 0;
    }		
  
    int main()
    {
        init();
        readData();
        while(1){
            delay(50);
        }
        return 0;
    }