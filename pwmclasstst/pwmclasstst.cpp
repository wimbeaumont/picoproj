#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "PWM_PICO.h" 
#include "hardware/adc.h" 

#define PWMCLASSTSTVER "1.4"
#define sensor_temp 4 


int main() {
	// pico init 
    stdio_init_all();
    adc_init();
    //adc_gpio_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(sensor_temp);
	#ifndef PICO_DEFAULT_LED_PIN
	#warning pwm/led_fade example requires a board with a regular LED
	#else
	PWM_PICO pwm( PICO_DEFAULT_LED_PIN); 
	uint8_t rp2040versionnr=rp2040_chip_version ();
	uint8_t rp2040Romversionnr=rp2040_rom_version();
    puts("Hello, world!");
	pwm.freq(10000,true);
	while(1) {
		float delta=.05 ;
		uint16_t Tint=adc_read();
		float T= 27 - ((float) Tint*(3.3f/(1 << 12)) - 0.706)/0.001721;
		printf( PWMCLASSTSTVER);
		printf("************** rpvers:%d, romver %d T= %f\n\r",
			rp2040versionnr,rp2040Romversionnr,T);
		sleep_ms(1000);
		for (float dc =0; dc<100; dc+=delta ) {
			if( delta < 2.5)delta=1.1*delta; // increase the delta each time
			printf("%x, %x %x  %f  dc set to %f got %f  \n\r" ,
				pwm.duty_u32, pwm.top ,pwm.pt_sys, pwm.div, dc, pwm.duty(dc));
			sleep_ms(50);
		}
	}
	#endif
	printf("no led pin defined \n\r");
    return 0; // never reached 
}
