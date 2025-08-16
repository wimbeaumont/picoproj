file(GLOB FILES *.cpp )

add_library(PerDev ${FILES})

target_link_libraries(PerDev  pico_stdlib hardware_i2c)
target_compile_definitions(PerDev PUBLIC -D __PICO__)

target_include_directories(PerDev  PUBLIC ../include/)
