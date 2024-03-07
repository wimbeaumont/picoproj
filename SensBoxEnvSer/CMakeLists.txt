# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/wimb/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(SensBoxEnvSer C CXX ASM)
set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(SensBoxEnvSer SensBoxEnvSer.cpp )
add_subdirectory(../PWM/src  ./PMW ) 
add_subdirectory(./src) 

target_compile_definitions(SensBoxEnvSer PUBLIC -D __PICO__)

pico_set_program_name(SensBoxEnvSer "SensBoxEnvSer")
pico_set_program_version(SensBoxEnvSer "0.1")

target_include_directories(SensBoxEnvSer PUBLIC ./include/ )
target_include_directories(SensBoxEnvSer PUBLIC ../PWM/include/)

pico_enable_stdio_uart(SensBoxEnvSer 1)
pico_enable_stdio_usb(SensBoxEnvSer 1)

# Add the standard library to the build
target_link_libraries(SensBoxEnvSer pico_stdlib   PerDev pwmlib)

# Add any user requested libraries
target_link_libraries(SensBoxEnvSer
        hardware_i2c
        hardware_timer
        hardware_watchdog
        hardware_clocks
        hardware_pwm
        pico_multicore

        )

pico_add_extra_outputs(SensBoxEnvSer)

