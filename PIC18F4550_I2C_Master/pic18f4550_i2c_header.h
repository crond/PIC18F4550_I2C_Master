#include <htc.h>
//#include <pic18f45j50.h>
#include <stdio.h>
#include <string.h>
  
#define MASTER_WRITE   0b0
#define MASTER_READ    0b1
#define SLAVE_ADDR      0b01010101

int toggleLed(void);
int delay(int cnt);
int init(void);
int init_I2c();
int readData();
int initUart();
int start_I2C();
int sendToUart(char *data);
int invokeStart();
