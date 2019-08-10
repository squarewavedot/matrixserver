project(FPGARenderer)

set(SOURCE_FILES
#        mpsse/mpsse.c
#        FPGARendererFTDI.cpp
        FPGARendererRPISPI.cpp FPGARendererRPISPI.h)


add_library(FPGARenderer SHARED ${SOURCE_FILES})
target_link_libraries(FPGARenderer PRIVATE renderer )#bcm2835 wiringPi pigpio ftdi
target_include_directories(FPGARenderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS FPGARenderer
        LIBRARY
        DESTINATION lib
        COMPONENT FPGARenderer
        PUBLIC_HEADER
        DESTINATION include
        COMPONENT FPGARenderer
        )