#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "PWM_PICO.h" 


int main() {
	// pico init 
    stdio_init_all();
    
	#ifndef PICO_DEFAULT_LED_PIN
	#warning pwm/led_fade example requires a board with a regular LED
	#else
	PWM_PICO pwm( PICO_DEFAULT_LED_PIN); 
    puts("Hello, world!");
	pwm.freq(10000,true);
	while(1) 
	for (float dc =0; dc<100; dc+=.5) {
		printf("%x, %x %x  %f  dc set to %f got %f\n\r" ,pwm.duty_u32, pwm.top ,pwm.pt_sys, pwm.div, dc, pwm.duty(dc));
		sleep_ms(100);
	}
	#endif
	printf("no led pin defined \n\r");
    return 0; // never reached 
}
