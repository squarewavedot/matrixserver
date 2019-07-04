project(FPGARenderer)

set(SOURCE_FILES
        mpsse/mpsse.c
        FPGARendererFTDI.cpp
        FPGARendererRPISPI.cpp FPGARendererRPISPI.h)


add_library(FPGARenderer SHARED ${SOURCE_FILES})
target_link_libraries(FPGARenderer PRIVATE renderer ftdi)#bcm2835 wiringPi pigpio
target_include_directories(FPGARenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})