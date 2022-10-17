This class is a higher level interface to control the PWM on the Raspberry Pi Pico . 
As the RP4020 has no DAC the PWM  block can be used to have a analogue output ( with of course  limitation) 

To understand the details of the PWM block  best to read the paragraph 4.5  of the RP2040 data sheet. 

Init with the class with  

PWM_PICO( uint gpiopin , float freq=0 );

where gpiopin the the pin you want to use ( this is the IO pin on the chip not the pin nr , ref  Figure 2 in the Raspberry Pi Pico data sheet) 
Each PWM module has two outputs. At the moment ( 1.3 ) there is not check if the module was already initalized by a pervious call. 

freq is the frequency , so hence the periode time (1/f)  on which the PWM will be initialized. 


The PWM used to memic a DAC. 

The basic is that a signal with a repeating pulse is average to a analogue output using a low pass filter. 
The simplest low pass filter is a RC filter. 
But the response of the RC filter will be effected by the load so for a more predictable result it is better to use a amplifier. 


Choice of the period time : 


