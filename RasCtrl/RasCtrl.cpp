/** example program for the use of the AT30TSE7xx class 
 *
 *  for more info see the README.MD of the repository 
 *  https://github.com/wimbeaumont/picoprojects
 * 
 *  reads temperature and ee-prom 
 *  (C) Wim Beaumont Universiteit Antwerpen 2022
 *
 *  ver  0.10  mainly copied from AT30TSE7xxx test program added  power ctrl 
 *  ver  0.20  tested with hardware 
 *  License see
 *  https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
 */ 

#define RASCTRL "0.20"

#include "dev_interface_def.h"
#include "AT30TSE75x.h"


// OS / platform  specific  configs 

// OS / platform  specific  configs 
#ifdef __PICO__ 
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "stdlib.h" // for atof 
//#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "PicoI2CInterface.h"
PicoI2CInterface mbedi2c;
PicoI2CInterface*  mbedi2cp = &mbedi2c;
#elif  defined  __MBED__ 
#define  OS_SELECT "MBED" 

#include "mbed.h"
#if   defined (TARGET_KL25Z) || defined (TARGET_KL46Z)
  PinName const SDA = PTE0;
  PinName const SCL = PTE1;
#elif defined (TARGET_KL05Z)
  PinName const SDA = PTB4;
  PinName const SCL = PTB3;
#elif defined (TARGET_K20D50M)
  PinName const SDA = PTB1;
  PinName const SCL = PTB0;
#else
  #error TARGET NOT DEFINED
#endif

//Serial pc(USBTX, USBRX);

#include "I2C.h"
#include "MBEDI2CInterface.h"  
MBEDI2CInterface mbedi2c( SDA, SCL); 
MBEDI2CInterface* mbedi2cp=  &mbedi2c ;

//------------------ end MBED specific config
#elif defined __LINUX__ 

#define  OS_SELECT "linux_i2c" 

#include <cstdio>
#include <cstdlib>
#include "LinuxI2CInterface.h"

char *filename = (char*)"/dev/i2c-1";  //hard coded for the moment 
LinuxI2CInterface  mbedi2c(filename);
LinuxI2CInterface* mbedi2cp= &mbedi2c;

//------------------ end Linux I2C specific config
#else 
#define  OS_SELECT "linux_dummy" 

#include <cstdio>
#include <cstdlib>
#include "DummyI2CInterface.h"
DummyI2CInterface  mbedi2c;
DummyI2CInterface* mbedi2cp= &mbedi2c;

#endif  // __MBED__ 
//------------------ end Linux dummy specific config
#ifndef OS_SELECT 
#define OS_SELECT "linux_dummy" 
#endif
// --- end platform specific configs 

const uint ActPP=13;  // Actuator power pin 


I2CInterface* i2cdev= mbedi2cp;

void core1_entry() {
	char c ;
	while (1) {
		c=getchar();
		if (c =='1' ) { gpio_put(ActPP,1); gpio_put(PICO_DEFAULT_LED_PIN,1);}
		if (c =='0' ) {gpio_put(ActPP,0); gpio_put(PICO_DEFAULT_LED_PIN,0);}
	}
}



void print_buf_hex( char *data, int length){
    int nr;
    char *ptr=data;
    for ( int lc=0; lc < length ; lc++){
        nr= (int) *(ptr++);
        printf( "%02x ",nr);
    }
    printf("\n\r");
}        






int main(void) { 
#ifdef __PICO__    
       stdio_init_all();// pico init 
       gpio_init(PICO_DEFAULT_LED_PIN);gpio_init(ActPP);
		gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);gpio_set_dir(ActPP, GPIO_OUT);
       gpio_pull_up(ActPP);
        gpio_put(ActPP,0); //start power off 
       gpio_put(PICO_DEFAULT_LED_PIN,0);
       // all init is done so launch the voltage switch 
       
#endif 

   // get the version of getVersion 
   getVersion gv;
   int i2cerr;
   
   printf("AT30TSE752  example program version %s, compile date %s time %s\n\r",RASCTRL,__DATE__,__TIME__);
   printf("getVersion :%s\n\r ",gv.getversioninfo());
   printf("compiled for: %s\n\r ", OS_SELECT); 
   
   
   AT30TSE75x tid[8] ={ AT30TSE75x( i2cdev ,0), AT30TSE75x( i2cdev ,1),AT30TSE75x( i2cdev ,2) ,AT30TSE75x( i2cdev ,3),
                        AT30TSE75x( i2cdev ,4), AT30TSE75x( i2cdev ,5),AT30TSE75x( i2cdev ,6) ,AT30TSE75x( i2cdev ,7)};
   bool addrfound[8];
   printf ( "AT30SE75x version :%s\n\r ",tid[0].getversioninfo());   
   for (int lc=0; lc <8 ;lc++) {
        printf( "Taddr %x , Eaddr %x \n\r ", tid[lc].getTaddr(),tid[lc].getEaddr());
        if( tid[lc].getInitStatus() ){ 
		addrfound[lc]=false; 
		printf("reading config registers failed ");
		printf("last: ack status:%d , i2cerr %d not supported %d \n\r",
			(int)i2cdev->getLastAckStatus(),i2cdev->getLastComError(),(int)i2cdev->getNotSupported());
        } 
        else {
            addrfound[lc]=true;   
            tid[lc].set_resolution(12 , i2cerr );
            tid[lc].set_FaultTollerantQueue('6', i2cerr );
            tid[lc].set_AlertPinPolarity(0,i2cerr);
            tid[lc].set_AlarmThermostateMode(0,i2cerr);
            tid[lc].set_config(i2cerr,0);            
            int configrd= tid[lc].read_config( i2cerr, 0);
            printf( " config %x  I2cerr %d \n\r", configrd,i2cerr ); 
        }
   }   
   bool somefound=false;  
   for( int lc=0; lc <8 ;lc++) { somefound =somefound || addrfound[lc] ;} 
   if ( ! somefound) {
       int error; 
       while (1) { // just loop to get the I2C line active to debug
            for( int lc=0; lc <8 ;lc++) {
                float THlim=tid[lc].get_THighLimitRegister(error, 1);
                if( error) { printf("error for addr  %d \n\r", tid[lc].getTaddr()); }
                else {   printf("for addr  %d got %f \n\r", tid[lc].getTaddr(),THlim ); }
            }
            gpio_put(PICO_DEFAULT_LED_PIN,0);
            i2cdev->wait_for_ms(100);
            gpio_put(PICO_DEFAULT_LED_PIN,1);
            i2cdev->wait_for_ms(100);
            gpio_put(PICO_DEFAULT_LED_PIN,0);
            i2cdev->wait_for_ms(100);
            gpio_put(PICO_DEFAULT_LED_PIN,1);
            i2cdev->wait_for_ms(400);            
       }
    }
#ifdef __PICO__       
   multicore_launch_core1(core1_entry);
   // all init is done so launch the voltage switch 
       
#endif 
   int pagenr=0;
   const int nrstrs=4;
   char str[nrstrs][16];
   while(1) {
	 for (int lc=0; lc<7 ;lc++) {	
      if( addrfound[lc]) { 
        pagenr=0;
        // no need to read e-prom
        for ( int sc=0 ; sc <nrstrs;sc++){
            i2cerr=tid[lc].read_eeprompage(str[sc], 16, 0, (uint8_t) pagenr++);
            if(i2cerr) printf("eeprom read error %d addr %d  \n\r",i2cerr,lc);
        }
        
/*        for (int sc=0 ; sc < nrstrs;sc ++) { 
            str[sc][15]='\0'; //make sure it ends
            printf("%d:%15s ",sc,str[sc]);
        }
*/
        // temperature  correction
        str[3][7]='\0';
        float tempcor=atof(str[3]);
        if ( tempcor < -2.5 || tempcor > 2.5) tempcor=0.0; 
        float Tmp= tid[lc].get_temperature(i2cerr);      
        printf ("T=%f %f I2cerr %d addr %d", Tmp,Tmp+tempcor, i2cerr,lc);
        printf("\n\r");
       } // if
    } // for 
    printf("\n\r");
    i2cdev->wait_for_ms(500);
  } //while
    
}   
