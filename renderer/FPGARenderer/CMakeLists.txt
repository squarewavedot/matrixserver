project(FPGARenderer)

set(SOURCE_FILES
#        mpsse/mpsse.c
#        FPGARendererFTDI.cpp
        FPGARendererRPISPI.cpp FPGARendererRPISPI.h)


add_library(FPGARenderer SHARED ${SOURCE_FILES})
target_link_libraries(FPGARenderer PRIVATE renderer)
target_include_directories(FPGARenderer PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> $<INSTALL_INTERFACE:include/matrixapplication>)

set_target_properties(FPGARenderer PROPERTIES PUBLIC_HEADER "FPGARendererRPISPI.h")


install(TARGETS FPGARenderer
        EXPORT matrixapplication-targets
        LIBRARY
        DESTINATION lib
        COMPONENT FPGARenderer
        PUBLIC_HEADER
        DESTINATION include/matrixapplication
        COMPONENT FPGARenderer
        )