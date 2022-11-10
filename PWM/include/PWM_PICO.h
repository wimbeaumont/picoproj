/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PMW_PICO
#define __PMW_PICO

// v1.0  used 
// v1.1  added freq in initiation and changed freq to set_frequncy
//       duty to set_dutycycle 
// v1.2  added vout functionality 
// v1.21 corrected  vout functionality 
// v1.3  added set_dutycycle_dec

#define PWM_PICO_VER "1.32"
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
	// vout variables 
	float vgain;  
	float voffset;
	float VrefPMC;

public: 	
	// initialize the PWM for gpiopin 
	// if frequency > 0 the frequency will set and the PWM will be enabled.
	// pull up is set  
	PWM_PICO( uint gpiopin , float freq=0 );
	
	// set the periode time in us  of the PWM 
	// try to use the full 16 bits counter
	// start : 
	// returns the period time set in ns 
	uint period_ns( uint ptset, bool start ) ;
		
		
	// set the frequeny to int Hz.  use the preriod_ns class 
	// returns the freq set . 
	inline uint32_t set_frequency(uint32_t freqset,bool start ){
		uint32_t pt_set= 1000000000 /freqset; // requested period time in ns 
		pt_set= period_ns(pt_set,start); 
		return 1000000000 / pt_set;
	}
	
	// this set the (compare ) level of the channel no check for TOP , so could be behind TOP value 
	void  set_duty_u32(uint32_t setduty ) ;
	
	// set duty cycle between 0 and 2**16 (full counter)  * TOP value / 2**16  
	// 
	uint16_t duty_u16(uint16_t setduty );
	
	// set the duty cycle in decimal ( 0.0 to 1.0 if duty > 1 the duty cycle will be set to max duty cycle  
	// if < 0  will be set to 0 
	//calls duty_u32 )
	float set_dutycycle_dec( float duty );
	
	// set the duty cycle in %  ( == 
	float set_dutycycle( float duty ) { return 100*set_dutycycle_dec( duty/100) ;};
	
	void set_enabled(bool enable );

	// initialize the vout function parameters,  during instantiation Voutmax =Valuemax = Voutpico( 3.3) , Voutmin=Valuemin=0;
	// change Vrefset in case other output voltage is used for the PWM by example in case of an open drain 
	void init_PWMVout( float Valuemin,  float Valuemax,float Voutmin,float Voutmax , float Vrefset=3.3 );

	//set the duty cycle so that the average output voltatge will be related to valuein based on the init_vout settings
	// if init_vout is not set , then vout = valuein 
	// @return duty cycle in % 
	float set_PWMVout ( float valuein); 
}; //end class PWM_PICO 


#endif
