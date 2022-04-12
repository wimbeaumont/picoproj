file(GLOB FILES *.cc *.h)

add_library(pwmlib ${FILES})

target_link_libraries(pwmlib  pico_stdlib hardware_pwm
						hardware_clocks )

target_include_directories(pwmlib PUBLIC ../include/)
