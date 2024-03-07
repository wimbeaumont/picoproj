# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/wimb/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(hum2dewp C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(hum2dewp hum2dewp.cpp )
add_subdirectory(../PWM/src  ./PMW ) 
add_subdirectory(./src) 

target_compile_definitions(hum2dewp PUBLIC -D __PICO__)


target_include_directories(hum2dewp PUBLIC ./include/)
target_include_directories(hum2dewp PUBLIC ../PWM/include/)


pico_set_program_name(hum2dewp "hum2dewp")
pico_set_program_version(hum2dewp "0.7")

pico_enable_stdio_uart(hum2dewp 1)
pico_enable_stdio_usb(hum2dewp 1)


# Add the standard library to the build
target_link_libraries(	hum2dewp 
						hum2dewputils
						pico_stdlib
						pico_multicore
						hardware_watchdog
						hardware_adc
						hardware_pwm
						pwmlib
					)

pico_add_extra_outputs(hum2dewp)

