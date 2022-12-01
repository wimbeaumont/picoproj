#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "PWM_PICO.h" 
#include "hardware/adc.h" 

#define PWMCLASSTSTVER "1.8"
#define sensor_temp 4 
#ifndef PICO_DEFAULT_LED_PIN
	#warning pwm/led_fade example requires a board with a regular LED
#else

const uint PWMPIN=13;  // actuator (PWM) control pin 

void core1_entry() {
	PWM_PICO pwmled( PICO_DEFAULT_LED_PIN);  //GPIO 25 
	pwmled.set_frequency(10000,true);
	float delta_l=.05 ;
	float dc_set;
	while(1) {
		for (float dc =0; dc<100; dc+=delta_l) {
			if( delta_l < 2.5)delta_l=1.05*delta_l; // increase the delta each time
			dc_set=pwmled.set_dutycycle(dc);
			//printf("LED %x, %x %x  %f  dc set to %f got %f  \n\r" ,
			//	pwmled.duty_u32, pwmled.top ,pwmled.pt_sys, pwmled.div, dc, dc_set);
			sleep_ms(200);
		}
		
	}
}



int main() {
	// pico init 
    stdio_init_all();
    multicore_launch_core1(core1_entry);
    adc_init();
    //adc_gpio_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(sensor_temp);
	
	PWM_PICO pwm(PWMPIN); 
	uint8_t rp2040versionnr=rp2040_chip_version ();
	uint8_t rp2040Romversionnr=rp2040_rom_version();
    puts("Hello, world!");
	pwm.set_frequency(1000,true);
	while(1) {
		float delta=.05 ;
		uint16_t Tint=adc_read();
		float T= 27 - ((float) Tint*(3.3f/(1 << 12)) - 0.706)/0.001721;
		printf("********");
		printf("%s ,PWMclass  %s",PWMCLASSTSTVER, pwm.getVersion());
		printf("*** rpvers:%d, romver %d T= %f\n\r",
			rp2040versionnr,rp2040Romversionnr,T);
		sleep_ms(1000);
		for (float dc =0; dc<=101; dc+=delta ) {
			// if(dc > 100.0f) dc =100.0f; // check if high all the time 
			printf("%x, %x %x  %f  dc set to %f got %f  \n\r" ,
				pwm.duty_u32, pwm.top ,pwm.pt_sys, pwm.div, dc, pwm.set_dutycycle(dc));
			sleep_ms(50);
		}
	}
	
    return 0; // never reached 
}
#endif
