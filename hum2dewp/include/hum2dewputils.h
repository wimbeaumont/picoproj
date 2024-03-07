#ifndef HUM2DEWPUTILS_H_
#define HUM2DEWPUTILS_H_

#include "math.h"
#include "float.h" 
#include "pico/types.h"
#include "pico/bootrom/sf_table.h"



const int HISSIZE=10;
const int HISSIZEH=10;

const float Vref=3.3; // voltage reference NTC circuit
const float VrefADC=3.0; // voltage reference pico ADC 
const float gNTC=0.7464265433589305 ; // gain for the R NTC calculation based on cal resistors
const float oNTC=0.22897459613882545; // offset for the R NTC calculation based on cal resistors
const float RsNTC= 68000;  // serie resistor NTC 
const float NTCa_bi=8.55E-04;
const float NTCb_bi=2.57E-04;
const float NTCc_bi=1.65E-07;


// routing to take the average of a ring counter , avoiding sum over all entries of the histogram 
float  avg(float newvalue, float* hist,int& ringcnt, int HSIZE , float& sum );

float adc2Hum(float adcv, float T );


// returns the NTC impedance as function of the input voltage  in ohm
inline float Rntc( float Vout,float g1,float o1,float Rs1, float Vreference) {
   float Vin= g1*Vout + o1;
   float R = Rs1 *Vin /( Vref-Vin) ;
   return  R;
}




// fill in the constants  to the Rntc function for the burn in station 
// the constants are defined global 
inline float Rntc( float Vout ) {
	return Rntc(Vout, gNTC,oNTC,RsNTC ,Vref);
}

//returns the temperature of the ntc give the impedance R   
// NTCa,b,c are the NTC paramters 
inline float  Tntc ( float R , float NTCa,float NTCb,float NTCc )  {  
	float lgR=log(R);
   return  -273 + 1/ ( NTCa + NTCb * lgR +NTCc*lgR*lgR*lgR);
}


// returns the Temperature given the NTC impedance  for the burn in station 
// the contants are defined global 
inline float Tntc( float R ) {
	return Tntc(R,NTCa_bi,NTCb_bi,NTCc_bi);
}

// returns the temperature ( average over last  10 measurements) 
// adcv the last read volage 
float adc2Tp( float adcv, float  &R );

inline float adc2Vsys( float adcv ){
	return 2*adcv;
}



// dew point calc 

// https://en.wikipedia.org/wiki/Dew_point#Calculating_the_dew_point
// rh relative humitdity in % ( so 70  = 70% ) 
// Tact actual temperature in [C]
// return dewpoint in [C] 
float dewpoint ( float rh , float Tact);
	


#endif 
