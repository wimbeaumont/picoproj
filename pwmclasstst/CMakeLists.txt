# Generated Cmake Pico project file
# cmake -B build -S .

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/wimb/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pwmclasstst C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pwmclasstst pwmclasstst.cpp )
add_subdirectory(../PWM/src  ./PMW ) 

pico_set_program_name(pwmclasstst "pwmclasstst")
pico_set_program_version(pwmclasstst "0.2")

target_include_directories(pwmclasstst PUBLIC ../PWM/include/)

pico_enable_stdio_uart(pwmclasstst 1)
pico_enable_stdio_usb(pwmclasstst 1)


# Add the private library to the build
target_link_libraries(pwmclasstst pico_stdlib  pwmlib )


# Add the standard library to the build
target_link_libraries(pwmclasstst 
						pico_stdlib
						pico_multicore
						hardware_pwm
						hardware_clocks
						hardware_adc
						)

pico_add_extra_outputs(pwmclasstst)

