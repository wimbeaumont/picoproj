#include "PWM_PICO.h" 


	PWM_PICO::PWM_PICO( uint gpiopin) {
		gpio_set_function(gpiopin, GPIO_FUNC_PWM);
		config = pwm_get_default_config();
		channr=pwm_gpio_to_channel(gpiopin);
		slicenr=pwm_gpio_to_slice_num(gpiopin);
		
	}
	// set the periode time in us  of the PWM 
	// try to use the full 16 bits counter
	// start : 
	// returns the period time set in ns 
	uint PWM_PICO::period_ns( uint ptset, bool start ) {
		uint32_t syscllfreq= clock_get_hz (clk_sys);  //
		// get period time in ns 
		pt_sys= 1000000000/syscllfreq; // periode time sys clock in ns 
		top=  0xffffu; // start with max count range  
		uint32_t pt = (uint32_t)top * pt_sys;  // no divider 
		div =(float) ptset / (float)pt; 
		while ( div < 1) {
			top=((top+1)/2)-1 ; 
			pt = (uint32_t)top * pt_sys;
			div = ptset / pt; 
		}
		
		pwm_config_set_wrap (&config,top);
		pwm_config_set_clkdiv (&config , div);
		pwm_init (slicenr,&config,start);
		return ptset; // need recalculation from settings . 
	}

	// this set the (compare ) level of the channel no check for TOP , so could be behind TOP value 
	void  PWM_PICO::set_duty_u32(uint32_t setduty ) { 
		duty_u32=setduty ;
		pwm_set_chan_level (slicenr, channr , (uint16_t) setduty);
	}
		
	
	// set duty cycle between 0 and 2**16 (full counter)  * TOP value / 2**16  
	// 
	uint16_t PWM_PICO::duty_u16(uint16_t setduty ) { 
		uint32_t  setduty32  = ((uint16_t) config.top * setduty ) / (256*256 );
		set_duty_u32(setduty32);
		return (uint16_t) setduty32 ;
	}
	
	float PWM_PICO::duty( float duty ) {
		if( duty > 100.0) duty=100;
		if( duty < 0.0) duty=0.0;
		uint32_t setduty32 = ( uint32_t) ( duty * config.top /100 );
		set_duty_u32(setduty32);
		return  100*(float) setduty32 / (float)config.top;
	}

	
	void PWM_PICO::set_enabled(bool enable ) {
		pwm_set_enabled(slicenr, enable);
	
	}


