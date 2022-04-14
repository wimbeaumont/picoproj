/** 
 * program controlling a linair actuator and reading tempeature
 * used for the LVDT test system  
 * this program uses 
 * https://github.com/wimbeaumont/peripheral_dev_tst.git
 * and related repositories , see README.md 
 * This program is on https://github.com/wimbeaumont/picoproj 
 * it targets the Raspberry Pi Pico board ( Version 2021) 
 * V 0.10 : coding 
 * V 0.20 : tested with the AT30TSE752 sensor and on board LED connected to PWM
 * V 0.30 : move LED PWM to second core ( for "fun") not tested 
 * V 0.40 : check with the correct pin + IO to switch on/ off the power for the actuator 
 * C) Wim Beaumont Universiteit Antwerpen  2022
 *
 * License see
 * https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
 */ 

#define LVDTCHKCTRLVER "0.30"

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
//#include "stdlib.h" // for atof 
#include "PWM_PICO.h" 

#include "PicoI2CInterface.h"
PicoI2CInterface Pi2c;
PicoI2CInterface*  i2cdev = &Pi2c;

#include "AT30TSE75x.h"


void core1_entry() {
	PWM_PICO pwmled( PICO_DEFAULT_LED_PIN); 
	while(1) {
		for (float dc =0; dc<100; dc+=.5) {
			pwmled.duty(dc);
		}
		sleep_ms(100);
	}
}

int main(){
	
    stdio_init_all();
    multicore_launch_core1(core1_entry);
	AT30TSE75x tid( i2cdev ,3);
	bool Tvalid = !tid.getInitStatus() ;
	int i2cerr;
	PWM_PICO pwm( 1); 
	puts("Hello, world!");
	pwm.freq(10000,true);
	float temperature=-273;
	while(1) 
	for (float dc =0; dc<100; dc+=.5) {
		printf("%x, %x %x  %f  dc set to %f got %f" ,pwm.duty_u32, pwm.top ,pwm.pt_sys, pwm.div, dc, pwm.duty(dc));
		sleep_ms(100);
		if( Tvalid) {
			temperature=tid.get_temperature(i2cerr); 
			if ( i2cerr < 0) printf(" Ti2cerr");
			else printf("T=%f",temperature);
		}
		else printf(" T= invalid");
		printf("\n\r");
	}
	
	


    return 0;
}
