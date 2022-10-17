# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/wimb/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(RasCtrl C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(RasCtrl RasCtrl.cpp )
add_subdirectory(./src) 
target_compile_definitions(RasCtrl PUBLIC -D __PICO__)


pico_set_program_name(RasCtrl "RasCtrl")
pico_set_program_version(RasCtrl "0.1")

pico_enable_stdio_uart(RasCtrl 1)
pico_enable_stdio_usb(RasCtrl 1)

# Add the standard library to the build
target_link_libraries(
	RasCtrl 
	pico_stdlib PerDev
	hardware_i2c
    hardware_clocks
    pico_multicore)

# Add any user requested libraries
target_link_libraries(PerDev
        
        )

pico_add_extra_outputs(RasCtrl)

