# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/wimb/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(VEML7700_tst C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(VEML7700_tst VEML7700_tst.cpp )
add_subdirectory(./src) 
target_compile_definitions(VEML7700_tst PUBLIC -D __PICO__)

pico_set_program_name(VEML7700_tst "VEML7700_tst")
pico_set_program_version(VEML7700_tst "0.1")

target_include_directories(VEML7700_tst PUBLIC ./include/)

pico_enable_stdio_uart(VEML7700_tst 1)
pico_enable_stdio_usb(VEML7700_tst 1)

# Add the standard library to the build
target_link_libraries(VEML7700_tst pico_stdlib PerDev)

# Add any user requested libraries
target_link_libraries(VEML7700_tst
        hardware_i2c
        )

pico_add_extra_outputs(VEML7700_tst)

