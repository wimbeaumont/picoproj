/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PMW_PICO
#define __PMW_PICO

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

class PWM_PICO{
	public: 
	pwm_config config; // the structure 
	uint slicenr; 
	uint channr; 
	uint32_t top; // hee top (wrap) value 
	uint32_t pt_sys;
	uint32_t duty_u32;
	float div;
public: 	
	PWM_PICO( uint gpiopin);
	
	// set the periode time in us  of the PWM 
	// try to use the full 16 bits counter
	// start : 
	// returns the period time set in ns 
	uint period_ns( uint ptset, bool start ) ;
		
		
	// set the frequeny to int Hz.  use the preriod_ns class 
	// returns the freq set . 
	inline uint32_t freq(uint32_t freqset,bool start ){
		uint32_t pt_set= 1000000000 /freqset; // requested period time in ns 
		pt_set= period_ns(pt_set,start); 
		return 1000000000 / pt_set;
	}
	
	// this set the (compare ) level of the channel no check for TOP , so could be behind TOP value 
	void  set_duty_u32(uint32_t setduty ) ;
	
	// set duty cycle between 0 and 2**16 (full counter)  * TOP value / 2**16  
	// 
	uint16_t duty_u16(uint16_t setduty );
	
	// set the duty cycle calls duty_u32 
	float duty( float duty );
	
	void set_enabled(bool enable );
	
}; //end class PWM_PICO 


#endif
