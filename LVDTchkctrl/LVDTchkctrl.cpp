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
 * V 0.40 : compile check with the correct pin + IO to switch on/ off the power for the actuator 
 * V 0.41 : checked with pico standalone 
 * V 0.43 : checking with full hw 
 * V 0.45 : version  used  . return also 0.0. after p, to be investigated 
 * V 0.46 : put sleep in the core 1 in the for loop 
 * V 0.50 : put temperature sensor in high resolution
 * C) Wim Beaumont Universiteit Antwerpen  2022
 *
 * License see
 * https://github.com/wimbeaumont/PeripheralDevices/blob/master/LICENSE
 */ 

#define LVDTCHKCTRLVER "0.51"

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/adc.h"  

#include "stdlib.h" // for atof  get_c
#include "PWM_PICO.h" 

#include "PicoI2CInterface.h"
PicoI2CInterface Pi2c;
PicoI2CInterface*  i2cdev = &Pi2c;

#include "AT30TSE75x.h"


const uint ActPP=15;  // Actuator power pin 
const uint PWMPIN=13;  // actuator (PWM) control pin 


void core1_entry() {
	PWM_PICO pwmled( PICO_DEFAULT_LED_PIN);  //GPIO 25 
	pwmled.freq(10000,true);
	float delta_l=.05 ;
	float dc_set;
	while(1) {
		for (float dc =0; dc<100; dc+=delta_l) {
			if( delta_l < 2.5)delta_l=1.05*delta_l; // increase the delta each time
			dc_set=pwmled.duty(dc);
			sleep_ms(200);
		}
		
	}
}




float adc2pos( uint16_t ADCread) {
	float pos=	50.f* (float) ADCread / (1<<12); //volt = ADCread * 3.3f / (1<<12); pos =volt *50/3.3 
	return pos;
}	

// switch on the power of the actuator pow = true, else off (pow=false ) 
void poweract( bool pow ) {
	if (pow )  gpio_put(ActPP,0); // inverted 
	else gpio_put(ActPP,0); 
}

int main(){
	
    stdio_init_all();
    //just for fun
    multicore_launch_core1(core1_entry);
    float version=atof(LVDTCHKCTRLVER);
    // The ADC init part 
    adc_init();
    gpio_disable_pulls (28);
    adc_select_input(2); //GPIO 28 
    
    // the temperature init 
	int i2cerr=-1;
	AT30TSE75x tid( i2cdev ,3);
	tid.set_resolution(12 , i2cerr,false,true ); // none volitaile but update 
	bool Tvalid = !tid.getInitStatus() ;
	float temperature=-274;  // invalid tempeature 
	// the PWM actuator control init 
	PWM_PICO pwm( PWMPIN);  // set PWM to gpio 13  
	pwm.freq(1000,true);
	puts("Hello, world!");
	
	// the actuator power control pin init 
	gpio_init(15);
    gpio_set_dir(15, GPIO_OUT);
	char  sel='n';
	float curpos=-1;
	while(1){
		scanf("%c",&sel);
		switch (sel) {
			case 'p' : // set position 
				float posreq;
				scanf("%f",&posreq);
				printf("%f\n",posreq);
				if(posreq > 50.0 ) posreq=50.0;if (posreq < 0.01)  posreq =0.01;
				pwm.duty(100* posreq/50.0); //dutycycle is in % 
				poweract(true);
				sleep_ms(4000);
				curpos=adc2pos(adc_read());
				poweract(false);
			break;
			case 'g' :
					printf("%5.2f\n",curpos);
			break;
			case 't' : // get temperature 
				if( Tvalid) temperature=tid.get_temperature(i2cerr);  
				if ( i2cerr < 0) temperature=-274;  
				printf("%5.2f\n",temperature);
			break;
			case 'v' : // get version
				printf("%5.2f\n",version);
			break;
			default:
				printf("0.0\n");
		}
	}

    return 0; //never reach 
}
