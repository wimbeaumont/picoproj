#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/adc.h"  
#include "hardware/watchdog.h"
#include "PWM_PICO.h" 
#include "math.h"

#define HUM2DEWPVER "0.61" 

const float Vref=3.3; // voltage reference NTC circuit
const float VrefADC=3.0; // voltage reference pico ADC 
const int HISSIZE=10;
const float gNTC=0.7464265433589305 ; // gain for the R NTC calculation based on cal resistors
const float oNTC=0.22897459613882545; // offset for the R NTC calculation based on cal resistors
const float RsNTC= 68000;  // serie resistor NTC 
const float NTCa_bi=8.55E-04;
const float NTCb_bi=2.57E-04;
const float NTCc_bi=1.65E-07;
enum ADCINP {HUNMIN=0, TEMPIN=1 , VSYSIN=2 };
// returns the voltage on the adcch 
float read_adc_ch( int adcch) {
	
	adc_select_input(adcch); 
	sleep_ms(50);
	return (float)adc_read() * VrefADC/(1<<12);
}
// returns the humidiy in % , if T > -273 C  temperature compensation is applied 
// T ambient temperature in C  
float adc2Hum(float T ){
	
	float vread=1000* read_adc_ch(HUNMIN); // [mV]
	float hum= 0.0375*vread-37.7;
	if( T > -273) {
		hum = hum *( 1- 0.0375*T-37.7);
	}
	return hum;
}


// returns the NTC impedance as function of the input voltage  in ohm
float Rntc( float Vout,float g1,float o1,float Rs1, float Vreference) {
   float Vin= g1*Vout + o1;
   float R = Rs1 *Vin /( Vref-Vin) ;
   return  R;
}


// fill in the constants for the burn in station 
float Rntc( float Vout ) {
	return Rntc(Vout, gNTC,oNTC,RsNTC ,Vref);
}

//returns the temperature of the ntc give the impedance R   
// NTCa,b,c are the NTC paramters 
float  Tntc ( float R , float NTCa,float NTCb,float NTCc )  {  
	float lgR=log(R);
   return  -273 + 1/ ( NTCa + NTCb * lgR +NTCc*lgR*lgR*lgR);
}


// returns the Temperature given the NTC impedance  for the burn in station 
float Tntc( float R ) {
	return Tntc(R,NTCa_bi,NTCb_bi,NTCc_bi);
}

// returns the temperature ( average over als 10 measurements) 
// adcv the last read volage 
float adc2Tp( float &adcv , float  &R ){
	static float his[HISSIZE];
	static int ringc=0;
	adcv=read_adc_ch(TEMPIN);
	his[ringc++]=adcv;
	if( ringc >=  HISSIZE) ringc=0;
	float avgs=0;
	for (int cnt=0;cnt<HISSIZE;cnt++) {
		avgs+=his[cnt];
	}
	avgs=avgs/HISSIZE;
	R= Rntc(avgs) ;
	return Tntc(R);
}


float adc2Vsys( void ){
	return 2*read_adc_ch(VSYSIN);
}



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





int main(){	

    stdio_init_all();
    // Enable the watchdog, requiring the watchdog to be updated every 5100ms or the chip will reboot
    // second arg is pause on debug which means the watchdog will pause when stepping through code
    watchdog_enable(5100, 1);
    
	// The ADC init part 
	float Hum; // humidity 
	float Tp; //temperature 
	float Vsys;// 5V power , ref for temperature 
    adc_init();
    //gpio_disable_pulls (26);  //Vsys 
    //gpio_disable_pulls (27);  // V
    //gpio_disable_pulls (28);
    adc_gpio_init(26);
    adc_gpio_init(27);
    adc_gpio_init(28);
    //just for fun
    multicore_launch_core1(core1_entry);
	for (uint i = 0; i < 25; i++) {
		printf("init %d\n", i);
		watchdog_update();
	}    

    if (watchdog_caused_reboot()) {
        printf("Rebooted by Watchdog ");
    } 
    printf("hum2dwp ver %s\n\r",HUM2DEWPVER);
    float VTout, Rntc_now;
    while(1) {
			watchdog_update();
			Vsys=adc2Vsys( );
			printf("Vsys= %f ", Vsys );
			Hum=adc2Hum(-300);
			printf("hum= %f ",Hum) ;
			Tp= adc2Tp(VTout, Rntc_now);
			printf("Tp= %f,R= %f  \n\r",Tp,Rntc_now );
	}

    return 0;
}