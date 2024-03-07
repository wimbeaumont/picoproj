#include "hum2dewputils.h"

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
	if (hum < 12 ) { //corrections 
		hum = hum +.45*(12-hum);
		if (hum < .5) hum=.5;
	}
	if( T > -273) {
		hum = hum *( 1- 0.0024*(T-23));
	}
	return hum;
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

