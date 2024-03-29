# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/wimb/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(LVDTchkctrl C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(LVDTchkctrl LVDTchkctrl.cpp )
add_subdirectory(../PWM/src  ./PMW ) 
add_subdirectory(./src) 
target_compile_definitions(LVDTchkctrl PUBLIC -D __PICO__)

target_include_directories(LVDTchkctrl PUBLIC ./include/)
target_include_directories(LVDTchkctrl PUBLIC ../PWM/include/)

pico_set_program_name(LVDTchkctrl "LVDTchkctrl")
pico_set_program_version(LVDTchkctrl "0.1")

pico_enable_stdio_uart(LVDTchkctrl 1)
pico_enable_stdio_usb(LVDTchkctrl 1)

# Add the standard library to the build
target_link_libraries(LVDTchkctrl pico_stdlib PerDev pwmlib )

# Add any user requested libraries
target_link_libraries(LVDTchkctrl
        hardware_i2c
        hardware_pwm
        hardware_adc
        pico_multicore
        )

pico_add_extra_outputs(LVDTchkctrl)

