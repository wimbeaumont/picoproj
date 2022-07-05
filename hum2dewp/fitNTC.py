import math
from  scipy.optimize import curve_fit
import numpy as np
# this fits the expected curve from the "perfect" electronics to the 
# results of the measurement done with calibration resistors . 



def ln( a) :
   la=a
   if  np.isscalar( a) :
       la=[a]
   for v in la :
      try :
          result=math.log(v)
      except ValueError:
         result=0 
      except TypeError:
          print("typeerror a = ",v)
          result=0
          exit
   return result

#returns the resistor value of the NTC given the measured voltage (Vout) , gain (g1) and ofset (o1) 
def Rntc ( Vout,g1,o1) :
   Vin= g1*Vout + o1
   R = 68 *Vin /( Vref-Vin) 
   return  R

# returns the temperture given the resistor value  R in ohms 
def TntcR (R) :
   NTCa=8.55E-04
   NTCb=2.57E-04
   NTCc=1.65E-07
   return  -273 + 1/ ( NTCa + NTCb * ln(R) +NTCc*ln(R)*ln(R)*ln(R))
   
#returns the temperature given de measured voltage (Vout) , gain (g1) and ofset (o1) 
def Tntc ( Vout,g1,o1)  :  
   R=1000* Rntc(Vout,g1,o1)
   return TntcR( R) 
   


xdata=[0.0128,0.014,0.255,0.79,1.054,1.051,1.485,1.62,1.9,2.265,2.37,2.51,2.98]
#Vdata= [0.0128,0.014,0.255,0.79,1.054,1.051,1.485,1.62,1.9,2.265,2.37,2.51,2.98]
# values in kOhm
Rdata=[2.255,4.74,10,22.5,30.16,30.16,46.39,52.66,68,95.36,1.05E+02,120.66,196]


Vref=3.3  # assume correct 

#initial value 
g1 =0.7401   # gain factor Vout/ vin 
o1=  .2469   # offset  
p0=[g1,o1 ]
ydata=Rdata
print ( len( xdata) , len( ydata) )
lc=0
print ( g1, o1) 
for vout in xdata :
   Tcal= Rntc( vout,g1,o1)
   print (Tcal ,ydata[lc] )
   lc=lc+1


popt ,pcov =curve_fit ( Rntc,xdata, ydata, p0=p0 ) 

print( popt) 

g1e=popt[0]
o1e=popt[1]
print ( g1e, o1e) 
for vout in xdata :
   Restimate = Rntc( vout,g1e,o1e)
   print (Restimate, TntcR(1000*Restimate)  )
