
/*
 * Project for getting the dewpoint out as voltage 
 * Humidity / temperature sensor is HTM2500LF
 * the output voltage is generated by the PWM 
 * 
 * 
 * History :
 * V 0.5  read the humidity and temperature 
 * V 0.6  added the voltage output function 
 *        humidity readout is low in respect to other probes.
 *        checked voltages seems to be ok. 
 * V 0.7  activated the temperature dependency for the humidity readout 
 *        corrected bug in the dP to voltage output function ( PWM class) 
 * V 0.8  average humidiy 
 * V 0.9  introduce ADC read class to unify histograming ( avaraging) added float optimalization for pico 
 * 		   not tested   class is tested. 
 * V 0.91  use "float.h" #include "pico/types.h" #include "pico/bootrom/sf_table.h" not tested 
 * V 0.92  not tested with hardware.   Preps for ADC read class, the functions ADC2xx for ADC input 
 * V 0.93   uses the ADC class to read  
 * V 0.95  to many changes go back to 9.2 
 * V 0.96  added extra PWM in core 2  for testing the output circuit 
 * V 1.00  functional with hardware 
 * V 1.01  remove multiplying with 1000 and 0.0375 => 37.5
 * 
 */



#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/adc.h"  
#include "hardware/watchdog.h"
#include "PWM_PICO.h" 
#include "math.h"
#include "float.h" 
#include "pico/types.h"
#include "pico/bootrom/sf_table.h"

#define HUM2DEWPVER "1.00" 

const float Vref=3.3; // voltage reference NTC circuit
const float VrefADC=3.0; // voltage reference pico ADC 
const int HISSIZE=10;
const int HISSIZEH=10;
const float gNTC=0.7464265433589305 ; // gain for the R NTC calculation based on cal resistors
const float oNTC=0.22897459613882545; // offset for the R NTC calculation based on cal resistors
const float RsNTC= 68000;  // serie resistor NTC 
const float NTCa_bi=8.55E-04;
const float NTCb_bi=2.57E-04;
const float NTCc_bi=1.65E-07;
enum ADCINP {HUMIN=0, TEMPIN=1 , VSYSIN=2 };



// routing to take the average of a ring counter , avoiding sum over all entries of the histogram 
float  avg(float newvalue, float* hist,int& ringcnt, int HSIZE , float& sum ) {
	int oldcnt=ringcnt+1;
	if (oldcnt >=  HSIZE) oldcnt=0;
	sum=sum-hist[oldcnt];
	ringcnt++;
	if ( ringcnt >= HSIZE )  ringcnt=0;
	hist[ringcnt]=newvalue;
	sum=sum+newvalue;
	return sum/HSIZE;
}
	
// returns the voltage on the adcch 
float read_adc_ch( int adcch) {
	
	adc_select_input(adcch); 
	sleep_ms(50);
	return (float)adc_read() * VrefADC/(1<<12);
}


class read_adc_CH {
	int ch;
	float* hist;
	float sumh;
	float hs; // histogram size 
	int ringcnt;
	float avg;
	float (*radc)(int);
	public :
		//constructor,  if histsize >1 a histogram (array)  of histsize is created
		// adcch , the ADC channel
		// readadcf  , pointer to the function to read the ADC channel 
		read_adc_CH(int adcch ,float (*readadcf)(int), int histsize=0) {
			radc=readadcf;
			ch=adcch;
			float Nadc=read_adc_ch(ch);
			hs=histsize; hist=NULL; ringcnt=0;sumh=0.0;
			if (hs > 1 && hs < 100 ) hist=new float[(uint32_t)hs];
		}//end constructor 
		~read_adc_CH() {  if (hist) delete hist; } 
		// reads the ADC ch and returns the value 
		float get_cur_adc_V(void) {
			return read_adc_ch(ch);
		}
		//reads the ADC ch  and adds this value to the sum of the histogram removes the oldest value 
		//returns the average of the histogram.  
		float get_adcV(void) {
			if (hist) {
				int oldcnt=ringcnt+1;
				if (oldcnt >=  hs) oldcnt=0;
				sumh=sumh-hist[oldcnt];
				ringcnt++;
				if ( ringcnt >= hs )  ringcnt=0;
				hist[ringcnt]=radc(ch);
				sumh=sumh+hist[ringcnt];
				avg = sumh/hs;
			}
			else { avg=read_adc_ch(ch);}
			return avg;
		}
};


				
// returns the humidiy in % , if T > -273 C  temperature compensation is applied 
// T ambient temperature in C  
float adc2Hum(float adcv, float T ){
	static float hisH[HISSIZEH];
	static int ringcH=0;
	float vread=adcv; // [V]
	// take the average of the last 10 readings 
	hisH[ringcH++]=vread;
	if( ringcH >=  HISSIZEH)ringcH=0;
	float avgs=0;
	for (int cnt=0;cnt<HISSIZEH;cnt++) {
		avgs+=hisH[cnt];
	}
	avgs=avgs/HISSIZEH;
	float hum= 37.5*avgs-37.7;
	if( T > -273) {
		hum = hum *( 1- 0.0024*(T-23));
	}
	return hum;
}



// returns the NTC impedance as function of the input voltage  in ohm
float Rntc( float Vout,float g1,float o1,float Rs1, float Vreference) {
   float Vin= g1*Vout + o1;
   float R = Rs1 *Vin /( Vref-Vin) ;
   return  R;
}


// fill in the constants  to the Rntc function for the burn in station 
// the constants are defined global 
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
// the contants are defined global 
float Tntc( float R ) {
	return Tntc(R,NTCa_bi,NTCb_bi,NTCc_bi);
}

// returns the temperature ( average over last  10 measurements) 
// adcv the last read volage 
float adc2Tp( float adcv, float  &R ){
	static float his[HISSIZE];
	static int ringc=0;
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


float adc2Vsys( float adcv ){
	return 2*adcv;
}

// dew point calc 

// https://en.wikipedia.org/wiki/Dew_point#Calculating_the_dew_point
// rh relative humitdity in % ( so 70  = 70% ) 
// Tact actual temperature in [C]
// return dewpoint in [C] 
float dewpoint ( float rh , float Tact){
	const float a=6.1121;
	const float b=18.676;
	const float c=257.14;
	const float d=234.5;
	//expT= exp((b-T/d)*(T/c+T))
	float expT=exp((b-Tact/d)*(Tact/(c+Tact))); 
	//Ym(T,RH)= ln( RH/100 * expT) eventual  ln(R/100) + (b-T/d)*(T/c+T)
	float Ym= log((rh/100)*expT);
	return c*Ym/(b-Ym);

}



void core1_entry() {
	PWM_PICO pwmled( PICO_DEFAULT_LED_PIN);  //GPIO 25 
	pwmled.set_frequency(10000,true);
	float delta_l=.05 ;
	float dc_set ;
	while(1) {
		for (float dc =0; dc<100; dc+=delta_l) {
			if( delta_l < 2.5)delta_l=1.05*delta_l; // increase the delta each time
			dc_set=pwmled.set_dutycycle(dc);
			sleep_ms(200);
		}
		
	}
}


int main(){	

    stdio_init_all();
    // Enable the watchdog, requiring the watchdog to be updated every 5100ms or the chip will reboot
    // second arg is pause on debug which means the watchdog will pause when stepping through code
    watchdog_enable(5100, 1);
     PWM_PICO  outT(10,10000); // have to check for  optimal frequency 
     PWM_PICO  outDP(11,10000);
	// The ADC init part 
	float Hum; // humidity 
	float Tp; //temperature 
	float Vsys;// 5V power , ref for temperature 
	float dp; //dewpoint 
    adc_init();
    //read_adc_CH  Hadc(HUMIN,&read_adc_ch,16);
    //read_adc_CH  Tadc(TEMPIN,&read_adc_ch,16);
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
    float  Rntc_now;
    outT.init_PWMVout(-40,50,0,3.3);
    outDP.init_PWMVout(-60,20,0,3.3);
    uint lc=0;
    while(1) {
			watchdog_update();
			Vsys=adc2Vsys(read_adc_ch(VSYSIN) );
			Tp= adc2Tp(read_adc_ch(TEMPIN), Rntc_now);
			Hum=adc2Hum(read_adc_ch(HUMIN ),Tp);
			dp=dewpoint(Hum,Tp);
			lc++;
			if( lc%100 == 0){
				printf("Vsys= %f ", Vsys );
				printf("H= %f ",Hum) ;
				printf("dp= %f ",dp);
				printf("T= %f ",Tp );
				printf("dT=%f ", outT.set_PWMVout(Tp));
				printf("dDP=%f", outDP.set_PWMVout(dp));
				printf("\n\r");
			}
	}

    return 0;
}
