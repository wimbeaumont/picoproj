file(GLOB FILES *.cpp *.h)

add_library(PerDev ${FILES})

target_link_libraries(PerDev  pico_stdlib)

target_include_directories(PerDev  PUBLIC ../include/)
