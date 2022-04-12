# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/wimb/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(ADC101_xx_tst C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(ADC101_xx_tst ADC101_xx_tst.cpp )
add_subdirectory(./src) 
target_compile_definitions(ADC101_xx_tst PUBLIC -D __PICO__)


pico_set_program_name(ADC101_xx_tst "ADC101_xx_tst")
pico_set_program_version(ADC101_xx_tst "0.1")

pico_enable_stdio_uart(ADC101_xx_tst 1)
pico_enable_stdio_usb(ADC101_xx_tst 1)

# Add any user requested libraries
target_link_libraries(ADC101_xx_tst pico_stdlib  PerDev)

# Add the standard library to the build
target_link_libraries(ADC101_xx_tst
        hardware_i2c
        )

pico_add_extra_outputs(ADC101_xx_tst)

