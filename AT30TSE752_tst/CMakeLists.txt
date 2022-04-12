# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/home/wimb/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(AT30TSE752_tst C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(AT30TSE752_tst AT30TSE752_tst.cpp )
add_subdirectory(./src) 
target_compile_definitions(AT30TSE752_tst PUBLIC -D __PICO__)

pico_set_program_name(AT30TSE752_tst "AT30TSE752_tst")
pico_set_program_version(AT30TSE752_tst "0.1")

pico_enable_stdio_uart(AT30TSE752_tst 1)
pico_enable_stdio_usb(AT30TSE752_tst 1)

# Add the standard library to the build
target_link_libraries(AT30TSE752_tst pico_stdlib PerDev)

# Add any user requested libraries
target_link_libraries(AT30TSE752_tst
        hardware_i2c
        hardware_clocks
        )

pico_add_extra_outputs(AT30TSE752_tst)

