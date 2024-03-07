file(GLOB FILES *.cpp )

add_library(hum2dewputils ${FILES})

target_link_libraries(hum2dewputils  pico_stdlib hardware_i2c)
target_compile_definitions(hum2dewputils PUBLIC -D __PICO__)

target_include_directories(hum2dewputils PUBLIC ../include/)
