Projects with the Raspberry Pi pico processor board 

https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html

This are idependent projects or uses the projects / libs from the 

   * wimbeaumont/PeripheralDevices
   * wimbeaumont/peripheral_dev_tst
   
The machine used to generate the pico  is  a virtual box machine , ubuntu 20  

   * apt  upgrade
   * apt install git
   * apt install python-is-python3
   * apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential
   * apt install libstdc++-arm-none-eabi-newlib
   * apt install geany 



installed is the pico c++ sdk package etc.  following the steps in this document https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf

in the .bashrc  : 

export PICO_SDK_PATH=/home/wimb/pico/pico-sdk

To generate the projects  I use pico-project-generator 

https://github.com/raspberrypi/pico-project-generator

to create a new project 

in dir picoproj

/home/wimb/pico/pico-project-generator/pico_project.py  -f i2c -uart -usb ADC101_xx_tst

cd ADC101_xx_tst 

mkdir src

mkdir inlcude 

echo  "build/*" > .gitignore

Each sub dir or "lib"  dir needs a CMakeList.txt file . 






Some "ptifalls"  

examples  scan I2C bus,  on a linux machine ( lubuntu 18 ) the serial port was not found.  Fixed it by putting hte printf in a while(1). 
